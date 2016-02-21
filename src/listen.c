// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "listen.h"

#include "util.h"

#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// End-of-transmission character.
static const int EOT = 4;

// Threshold for keyboard repeat parameters, in milliseconds. The parameters are
// expected to be at least this accurate.
static const long kbd_threshold = 10;

// Keyboard repeat parameters, in milliseconds.
static long kbd_repeat_delay;
static long kbd_repeat_interval;

// The most recent keyboard input.
static int kbd_input = 0;

// Make standard input unbuffered by turning off canonical mode.
static void use_unbuffered_input(void) {
	struct termios ctrl;
	tcgetattr(STDIN_FILENO, &ctrl);
	ctrl.c_lflag &= (unsigned long)~ICANON;
	tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);
}

// Entry function for the keyboard listener thread.
static void *listen(void *arg) {
	(void)arg;

	for (;;) {
		// Get a character (this blocks).
		const int ch = getchar();
		if (ch == EOF || ch == EOT) {
			kbd_input = EOF;
			break;
		}
		kbd_input = ch;
		usleep(10);
	}
	return NULL;
}

void calibrate_listener(void) {
	use_unbuffered_input();

	// Prompt the user and wait for three characters.
	fputs("Press and hold the space bar for 1 second: ", stdout);
	getchar();
	long t0 = current_millis();
	getchar();
	long t1 = current_millis();
	getchar();
	long t2 = current_millis();
	fputs("\nCalibrating complete!\nPress enter to continue... ", stdout);
	char ch;
	while ((ch = getchar()) != EOF && ch != '\n');

	// Set the keyboard parameters.
	kbd_repeat_delay = t1 - t0;
	kbd_repeat_interval = t2 - t1;
	printf("delay: %ld\nrepeat: %ld\n", t1-t0,t2-t1);
}

bool spawn_listener(void) {
	pthread_t listen_thread;
	return pthread_create(&listen_thread, NULL, listen, NULL) == 0;
}

int get_listener_state(long time_now) {
	static int input = 0;
	static long time = 0;
	static enum { IDLE, DELAY, REPEAT } mode = IDLE;

	// Read the keyboard input set by the listener thread, and reset it.
	int kbdi = kbd_input;
	if (kbdi == EOF) {
		return EOF;
	}
	kbd_input = 0;

	// Check if a new character has been received, and save it.
	bool pressed_now = kbdi != 0;
	if (pressed_now) {
		input = kbdi;
	}

	// Get the time passed since the last event.
	long elapsed = time_now - time;

	switch (mode) {
	case IDLE:
		if (pressed_now) {
			time = time_now;
			mode = DELAY;
			return input;
		} else {
			return 0;
		}
	case DELAY:
		if (pressed_now) {
			if (elapsed < kbd_repeat_delay - kbd_threshold) {
				time = time_now;
				return input;
			} else {
				time = time_now;
				mode = REPEAT;
				return input;
			}
		} else {
			if (elapsed < kbd_repeat_delay + kbd_threshold) {
				return input;
			} else {
				mode = IDLE;
				return 0;
			}
		}
	case REPEAT:
		if (pressed_now) {
			if (elapsed < kbd_repeat_interval - kbd_threshold) {
				printf("Hmm... that was fast\n");
			}
			time = time_now;
			return input;
		} else {
			if (elapsed < kbd_repeat_interval + kbd_threshold) {
				return input;
			} else {
				mode = IDLE;
				return 0;
			}
		}
	}
}
