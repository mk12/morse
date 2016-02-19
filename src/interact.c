// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "interact.h"

#include "util.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// Time intervals for interactive mode, expressed in multiples of the unit.
static const int time_dot = 1;
static const int time_dash = 3;
static const int time_between_signals = 1;
static const int time_between_letters = 3;

// End of transmission character.
static const int EOT = 4;

// Used to communicate keyboard input between threads.
static enum { KBD_UP, KBD_DOWN, KBD_EOF } kbd_input = KBD_UP;

// Make standard input unbuffered by turning off canonical mode.
static void use_unbuffered_input(void) {
	struct termios ctrl;
	tcgetattr(STDIN_FILENO, &ctrl);
	ctrl.c_lflag &= (unsigned long)~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);
}

// Entry function for the keyboard listening thread.
static void *listen(void *arg) {
	(void)arg;

	for (;;) {
		// Get a character (this blocks).
		const int ch = getchar();
		if (ch == EOF || ch == EOT) {
			kbd_input = KBD_EOF;
			break;
		}
		usleep(100);
	}
}

// Returns the time elapsed in milliseconds since the given start time.
static time_t elapsed_ms(const struct timeval *start) {
	struct timeval end;
	gettimeofday(&end, NULL);
	time_t sec = end.tv_sec - start->tv_sec;
	long micro = end.tv_usec - start->tv_usec;
	return (sec * 1000 + micro / 1000);
}

int interact(int time_unit) {
	// Configure standard input.
	use_unbuffered_input();

	char buf[512] = {'\r', 0};
	int bufind=1;
	
	// Create the keyboard listener thread.
	pthread_t listen_thread;
	if (pthread_create(&listen_thread, NULL, listen, NULL)) {
		return print_error("error creating thread");
	}

	// Initialize the state.
	enum { IDLE, DOT, DASH, CHAR, WORD } mode = IDLE;
	struct timeval tv;
	int prev_char = -5;
	gettimeofday(&tv, NULL);

	// Begin the main loop.
	for (;;) {
		time_t elapsed = elapsed_ms(&tv);
		switch (kbd_input) {
		case KBD_EOF:
			return 0;
		case KBD_DOWN:
			switch (mode) {
			case IDLE:
				mode = DOT;
				gettimeofday(&tv, NULL);
				break;
			case DOT:
				if (elapsed > time_unit * time_dot) {
					mode = DASH;
					gettimeofday(&tv, NULL);
				}
				break;
			case DASH:
				if (elapsed > time_unit * time_dash) {
					mode = IDLE;
					gettimeofday(&tv, NULL);
				}
				break;
			case CHAR:
				mode = DOT;
				gettimeofday(&tv, NULL);
				break;
			case WORD:
				mode = DOT;
				gettimeofday(&tv, NULL);
				break;
			}
			break;
		case KBD_UP:
			switch (mode) {
			case IDLE:
				break;
			case DOT:
				buf[bufind++] = '.';
				mode = CHAR;
				gettimeofday(&tv, NULL);
				break;
			case DASH:
				buf[bufind++] = '-';
				mode = CHAR;
				gettimeofday(&tv, NULL);
				break;
			case CHAR:
				if (elapsed > time_unit * time_between_signals) {
					buf[bufind++] = ' ';
					mode = WORD;
					gettimeofday(&tv, NULL);
				}
				break;
			case WORD:
				if (elapsed > time_unit * time_between_letters) {
					buf[bufind++] = '/';
					buf[bufind++] = ' ';
					mode = IDLE;
					gettimeofday(&tv, NULL);
				}
				break;
			}
			break;
		}
		fputs(buf, stdout);
		usleep(100);
	}
	return 0;
}
