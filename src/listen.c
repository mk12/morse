// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "listen.h"

#include "util.h"

#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// End-of-transmission character.
static const int EOT = 4;

// Threshold for keyboard repeat parameters, in milliseconds.
static const long kbd_repeat_delay_thresh = 5;
static const long kbd_repeat_interval_thresh = 40;

// Keyboard repeat parameters, in milliseconds.
static long kbd_repeat_delay;
static long kbd_repeat_interval;

// Keyboard input flags representing whether there is new input and whether an
// end-of-file condition has been encountered. Both must only be accessed after
// locking the mutex.

// Flags representing whether the listener has detected a new key press and
// whether it has encountered an end-of-file condition. Both must only be
// accessed after locking the mutex.
static bool listener_new = false;
static bool listener_eof = false;
static pthread_mutex_t listener_mutex = PTHREAD_MUTEX_INITIALIZER;

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
		int ch = getchar();
		bool eof = ch == EOF || ch == EOT;

		// Synchronize and write to the global flags.
		pthread_mutex_lock(&listener_mutex);
		listener_new = true;
		listener_eof = eof;
		pthread_mutex_unlock(&listener_mutex);

		if (eof) {
			break;
		}
		usleep(10);
	}
	return NULL;
}

int calibrate_listener(void) {
	use_unbuffered_input();

	// Prompt the user and wait for three characters.
	fputs("Press and hold any key... ", stdout);
	int ch;
	ch = getchar();
	if (ch == EOF || ch == EOT) {
		return EOF;
	}
	long t0 = current_millis();
	ch = getchar();
	if (ch == EOF || ch == EOT) {
		return EOF;
	}
	long t1 = current_millis();
	ch = getchar();
	if (ch == EOF || ch == EOT) {
		return EOF;
	}
	long t2 = current_millis();

	// Wait for the user to hit return.
	fputs("\nCalibrating complete.\nPress return to continue... ", stdout);
	while ((ch = getchar()) != '\n') {
		if (ch == EOF || ch == EOT) {
			return EOF;
		}
	}

	// Set the keyboard parameters.
	kbd_repeat_delay = t1 - t0 + kbd_repeat_delay_thresh;
	kbd_repeat_interval = t2 - t1 + kbd_repeat_interval_thresh;

	return 0;
}

bool spawn_listener(void) {
	pthread_t listen_thread;
	return pthread_create(&listen_thread, NULL, listen, NULL) == 0;
}

enum ListenerState get_listener_state(long time_now) {
	static long time = 0;
	static enum { IDLE, DELAY, REPEAT } mode = IDLE;

	// Synchronize and check for new input from the listener thread.
	bool new_press = false;
	bool eof = false;
	pthread_mutex_lock(&listener_mutex);
	if (listener_new) {
		new_press = true;
		eof = listener_eof;

		// Reset the flag now that we've seen it.
		listener_new = false;
	}
	pthread_mutex_unlock(&listener_mutex);
	if (eof) {
		return LS_EOF;
	}

	// Get the time passed since the last event.
	long elapsed = time_now - time;

	switch (mode) {
	case IDLE:
		if (new_press) {
			time = time_now;
			mode = DELAY;
			return LS_DOWN;
		}
		return LS_NONE;
	case DELAY:
		if (new_press) {
			time = time_now;
			mode = REPEAT;
			return LS_REPEAT;
		}
		if (elapsed < kbd_repeat_delay) {
			return LS_HOLD;
		}
		time = time_now;
		mode = IDLE;
		return LS_UP;
	case REPEAT:
		if (new_press) {
			time = time_now;
			return LS_HOLD_R;
		}
		if (elapsed < kbd_repeat_interval) {
			return LS_HOLD_R;
		}
		time = time_now;
		mode = IDLE;
		return LS_UP;
	}
}
