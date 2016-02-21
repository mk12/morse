// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "transmit.h"

#include "circle.h"
#include "listen.h"
#include "util.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Size of the circular buffer.
#define BUF_SIZE 10

// Time constants, in milliseconds.
#define TIME_BETWEEN_CHARS 500
#define TIME_BETWEEN_WORDS 1500

// Shows or hides the cursor in the terminal.
static void set_cursor(bool show) {
	if (show) {
		fputs("\x1B[?25h", stdout);
	} else {
		fputs("\x1B[?25l", stdout);
	}
}

// Handles SIGINT, the interrupt signal (Ctrl-C).
static void sigint_handler(int) __attribute__((noreturn));
static void sigint_handler(int sig) {
	(void)sig;
	set_cursor(true);
	exit(0);
}

int transmit(void) {
	// Hide the cursor, and make sure it comes back before exiting.
	set_cursor(false);
	signal(SIGINT, sigint_handler);

	// Set up the keyboard listener.
	long time_unit = calibrate_listener();
	if (time_unit == EOF) {
		return 0;
	}
	if (!spawn_listener()) {
		print_error("error creating thread");
		return 1;
	}

	// Set up the circular buffer.
	char buf[BUF_SIZE] = {'*'};
	struct Circle circ = { .buf = buf, .size = BUF_SIZE, .index = 0 };

	// Begin the main loop.
	bool done = false;
	long time = current_millis();
	enum { NONE, CHAR, WORD } wait_mode = NONE;
	while (!done) {
		long time_now = current_millis();
		enum ListenerState state = get_listener_state(time_now);
		switch (state) {
		case LS_EOF:
			done = true;
			continue;
		case LS_NONE:
			break;
		case LS_DOWN:
			if (peek(&circ) == '_') {
				insert(&circ, ' ');
				advance(&circ);
			}
			insert(&circ, '.');
			wait_mode = NONE;
			break;
		case LS_REPEAT:
			insert(&circ, '-');
			break;
		case LS_HOLD:
		case LS_HOLD_R:
			break;
		case LS_UP:
			append(&circ, '*');
			time = time_now;
			wait_mode = CHAR;
			break;
		}

		long elapsed = time_now - time;
		switch (wait_mode) {
		case NONE:
			break;
		case CHAR:
			if (elapsed > TIME_BETWEEN_CHARS) {
				insert(&circ, ' ');
				append(&circ, '*');
				wait_mode = WORD;
			}
			break;
		case WORD:
			if (elapsed > TIME_BETWEEN_WORDS) {
				insert(&circ, '/');
				append(&circ, ' ');
				append(&circ, '*');
				wait_mode = NONE;
			}
			break;
		}

		putchar('\r');
		print_circle(&circ);
		putchar(' ');
		putchar(' ');
		usleep(100);
	}

	set_cursor(true);
	return 0;
}
