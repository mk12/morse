// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "transmit.h"

#include "circle.h"
#include "code.h"
#include "listen.h"
#include "util.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

// The maximum number of terminal columns to use.
#define MAX_COLUMNS 128

// The number of extra columns to leave, to avoid input going too far.
#define EXTRA_COLUMNS 5

// Time constants, in milliseconds.
#define TIME_BETWEEN_CHARS 500
#define TIME_BETWEEN_WORDS 1500

// Sleep time for the main loop, in microseconds.
#define SLEEP_TIME_US 1000

// Shows or hides the cursor in the terminal.
static void set_cursor(bool show) {
	if (show) {
		fputs("\x1B[?25h", stdout);
	} else {
		fputs("\x1B[?25l", stdout);
	}
}

// Cleanup to be performed before exiting. Restores the cursor and prints a
// couple blank lines to prevent the shell prompt from overwriting the output.
static void cleanup(void) {
	set_cursor(true);
	putchar('\n');
	putchar('\n');
}

// Handles SIGINT, the interrupt signal (Ctrl-C).
static void sigint_handler(int) __attribute__((noreturn));
static void sigint_handler(int sig) {
	(void)sig;
	cleanup();
	exit(0);
}

// Returns the number of terminal columns that should be used.
static int number_of_columns(void) {
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	int n = ws.ws_col - EXTRA_COLUMNS;
	return n > MAX_COLUMNS ? MAX_COLUMNS : n;
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

	// Set up the circular buffers.
	int buf_size = number_of_columns();
	char code_buf[buf_size];
	char text_buf[buf_size];
	struct Circle code_circ, text_circ;
	init_empty(&code_circ, code_buf, buf_size);
	init_empty(&text_circ, text_buf, buf_size);
	append(&code_circ, '*');

	// Begin the main loop.
	Code code = 0;
	int code_size = 0;
	bool done = false;
	long time = current_millis();
	enum { NONE, CHAR, WORD } wait_mode = NONE;
	while (!done) {
		// Check the state of the keyboard listener.
		long time_now = current_millis();
		enum ListenerState state = get_listener_state(time_now);
		switch (state) {
		case LS_EOF:
			done = true;
			continue;
		case LS_NONE:
			break;
		case LS_DOWN:
			insert(&code_circ, '.');
			wait_mode = NONE;
			code <<= 1;
			code_size++;
			break;
		case LS_REPEAT:
			insert(&code_circ, '-');
			code |= 1;
			break;
		case LS_HOLD:
		case LS_HOLD_R:
			break;
		case LS_UP:
			append(&code_circ, '*');
			time = time_now;
			wait_mode = CHAR;
			break;
		}

		// Check if enough time has passed to start a new character or word.
		long elapsed = time_now - time;
		switch (wait_mode) {
		case NONE:
			break;
		case CHAR:
			if (elapsed > TIME_BETWEEN_CHARS) {
				insert(&code_circ, ' ');
				append(&code_circ, '*');
				wait_mode = WORD;

				char ch = INVALID_CODE;
				if (code_size <= MAX_SIZE) {
					code = add_size(code, code_size);
					char decoded = code_to_char(code);
					if (decoded) {
						ch = decoded;
					}
				}
				append(&text_circ, ch);
				code = 0;
				code_size = 0;
			}
			break;
		case WORD:
			if (elapsed > TIME_BETWEEN_WORDS) {
				insert(&code_circ, '/');
				append(&code_circ, ' ');
				append(&code_circ, '*');
				wait_mode = NONE;
				append(&text_circ, ' ');
			}
			break;
		}

		// Print the contents of both buffers.
		putchar('\r');
		print_circle(&code_circ);
		fputs("   \n", stdout);
		print_circle(&text_circ);
		fputs("   \x1B[F", stdout);
		fflush(stdout);

		usleep(SLEEP_TIME_US);
	}

	cleanup();
	return 0;
}
