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
/* static const int time_between_words = 7; */

// End of transmission character.
static const int EOT = 4;

// Used to communicate keyboard input between threads.
static int kbd_input = 0;
static bool kbd_acknowledged = false;

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

	bool waiting = false;
	for (;;) {
		if (waiting) {
			// Complete the handshake.
			if (kbd_acknowledged) {
				waiting = false;
			}
		} else {
			// Get a character (this blocks).
			const int ch = getchar();
			putchar('\b');

			// Initiate the handshake.
			kbd_acknowledged = false;
			kbd_input = ch;
			if (ch == EOF || ch == EOT) {
				return NULL;
			}
			waiting = true;
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
		const int input = kbd_input;
		time_t elapsed = elapsed_ms(&tv);
		if (input == EOF || input == EOT) {
			break;
		}
		if (input) {
			kbd_input = 0;
			kbd_acknowledged = true;
		}
		if (input && input != prev_char) {
			switch (mode) {
			case IDLE:
				mode = DOT;
				break;
			case DOT:
				buf[bufind++] = '.';
				mode = CHAR;
				break;
			case DASH:
				buf[bufind++] = '-';
				mode = CHAR;
				break;
			case CHAR:
				mode = DOT;
				break;
			case WORD:
				mode = DOT;
				break;
			}
			gettimeofday(&tv, NULL);
			prev_char = input;
		} else {
			switch (mode) {
			case IDLE:
				break;
			case DOT:
				if (elapsed > time_unit * time_dot) {
					mode = DASH;
				}
				break;
			case DASH:
				if (elapsed > time_unit * time_dash) {
					mode = IDLE;
				}
				break;
			case CHAR:
				if (elapsed > time_unit * time_between_signals) {
					buf[bufind++] = ' ';
					mode = WORD;
				}
				break;
			case WORD:
				if (elapsed > time_unit * time_between_letters) {
					buf[bufind++] = '/';
					buf[bufind++] = ' ';
					mode = IDLE;
				}
				break;
			}
		}
		fputs(buf, stdout);
		usleep(100);
	}
	return 0;
}
