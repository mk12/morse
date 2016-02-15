// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "interact.h"

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
static const int time_between_words = 7;

// End of transmission character.
static const int EOT = 4;

// The unit time interval, in milliseconds.
static int time_unit = 100;

// Used to communicate keyboard input between threads.
static int kbd_input = 0;
static bool kbd_acknowledged = false;

// Make standard input unbuffered by turning off canonical mode.
static void setup_unbuffered_input(void) {
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
			if (kbd_acknowledged) {
				waiting = false;
			}
		} else {
			const int ch = getchar();
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
static time_t elapsed_ms(struct timeval *start) {
	struct timeval end;
	gettimeofday(&end, NULL);
	time_t sec = end.tv_sec - start->tv_sec;
	long micro = end.tv_usec - start->tv_usec;
	return (sec * 1000 + micro / 1000);
}

int interact(void) {
	setup_unbuffered_input();
	pthread_t listen_thread;
	if (pthread_create(&listen_thread, NULL, listen, NULL)) {
		fputs("morse: error creating thread\n", stderr);
		return 1;
	}

	enum { IDLE, DOT, DASH, CHAR, WORD } mode = IDLE;
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int ch;
	for (int i = 0;; i++) {
		const int input = kbd_input;
		if (input == EOF || input == EOT) {
			break;
		} else if (input) {
			printf("Thread got a key: %c/%ld\n", input, input);
			kbd_input = 0;
			kbd_acknowledged = true;
		}
		usleep(1000);
	}
	return 0;
}
