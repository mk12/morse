// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "transmit.h"

#include "listen.h"
#include "util.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
static void sigint_handler(int unused) {
	(void)unused;
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

	// ...
	char buf[1024] = {0};
	int index = 0;
	long time = current_millis();
	long count;
	enum { IDLE, CHAR, WORD } mode = IDLE;

	// Begin the main loop.
	while ((count = get_listener_count()) != EOF) {
		long time_now = current_millis();
		long elapsed = time_now - time;
		if (count != 0) {
			if (count == 1) {
				buf[index++] = '.';
			} else {
				buf[index++] = '-';
			}
			time = time_now;
			mode = CHAR;
		} else {
			switch (mode) {
			case IDLE:
				break;
			case CHAR:
				if (elapsed > 700) {
					buf[index++] = ' ';
					mode = WORD;
					time = time_now;
				}
				break;
			case WORD:
				if (elapsed > 700) {
					buf[index++] = '/';
					buf[index++] = ' ';
					mode = IDLE;
					time = time_now;
				}
				break;
			}
		}
		if (time == time_now) {
			putchar('\r');
			fputs(buf, stdout);
			fflush(stdout);
		}
		usleep(1000);
	}

	set_cursor(true);
	return 0;
}
