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

	// Begin the main loop.
	char buf[1024] = {0};
	int index = 0;
	for (;;) {
		int count = get_listener_count();
		if (count == EOF) {
			break;
		}
		if (count != 0) {
			if (count == 1) {
				buf[index++] = '.';
			} else {
				buf[index++] = '-';
			}
		}
		putchar('\r');
		fputs(buf, stdout);
		fflush(stdout);
		usleep(1000);
	}

	set_cursor(true);
	return 0;
}
