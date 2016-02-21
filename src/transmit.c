// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "transmit.h"

#include "listen.h"
#include "util.h"

#include <signal.h>
#include <stdio.h>
#include <time.h>

// Time intervals for interactive mode, expressed in multiples of the unit.
static const int time_dot = 1;
static const int time_dash = 3;
static const int time_between_signals = 1;
static const int time_between_letters = 3;

static void int_handler(int unused) {
	(void)unused;
	printf("\e[?25h");
	exit(0);
}

int transmit(int time_unit) {
	printf("\e[?25l");
	signal(SIGINT, int_handler);
	calibrate_listener();
	if (!spawn_listener()) {
		print_error("error creating thread");
		return 1;
	}

	char buf[512] = {0};
	int bufind=0;

	// Initialize the state.
	enum { IDLE, DOT, DASH, CHAR, WORD } mode = IDLE;
	int prev_char = -5;
	long time = current_millis();

	// Begin the main loop.
	for (;;) {
		long time_now = current_millis();
		long elapsed = time_now - time;
		switch (get_listener_state(time_now)) {
		case EOF:
	printf("\e[?25h");
			return 0;
		default:
			switch (mode) {
			case IDLE:
				mode = DOT;
				time = time_now;
				break;
			case DOT:
				if (elapsed > time_unit * time_dot) {
					mode = DASH;
					time = time_now;
				}
				break;
			case DASH:
				if (elapsed > time_unit * time_dash) {
					mode = IDLE;
					time = time_now;
				}
				break;
			case CHAR:
				mode = DOT;
				time = time_now;
				break;
			case WORD:
				mode = DOT;
				time = time_now;
				break;
			}
			break;
		case 0:
			switch (mode) {
			case IDLE:
				break;
			case DOT:
				buf[bufind++] = '.';
				mode = CHAR;
				time = time_now;
				break;
			case DASH:
				buf[bufind++] = '-';
				mode = CHAR;
				time = time_now;
				break;
			case CHAR:
				if (elapsed > time_unit * time_between_signals) {
					buf[bufind++] = ' ';
					mode = WORD;
					time = time_now;
				}
				break;
			case WORD:
				if (elapsed > time_unit * time_between_letters) {
					buf[bufind++] = '/';
					buf[bufind++] = ' ';
					mode = IDLE;
					time = time_now;
				}
				break;
			}
			break;
		}
		putchar('\r');
		fputs(buf, stdout);
		usleep(10);
	}
	printf("\e[?25h");
	return 0;
}
