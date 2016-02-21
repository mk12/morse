// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "listen.h"

#include "util.h"

#include <pthread.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// End-of-transmission character.
static const int EOT = 4;

// Threshold for keyboard repeat parameters, in milliseconds. The parameters are
// expected to be at least this accurate.
static const long kbd_threshold = 10;

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
	kbd_repeat_delay = t1 - t0;
	kbd_repeat_interval = t2 - t1;

	return 0;
}

bool spawn_listener(void) {
	pthread_t listen_thread;
	return pthread_create(&listen_thread, NULL, listen, NULL) == 0;
}

int get_listener_count(void) {
	static int count = 0;
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
		return EOF;
	}

	// Get the time passed since the last event.
	long time_now = current_millis();
	long elapsed = time_now - time;

	switch (mode) {
	case IDLE:
		if (new_press) {
			time = time_now;
			mode = DELAY;
		}
		return 0;
	case DELAY:
		if (new_press) {
			count = 2;
			time = time_now;
			mode = REPEAT;
			return 0;
		}
		if (elapsed < kbd_repeat_delay + kbd_threshold) {
			return 0;
		}
		mode = IDLE;
		return 1;
	case REPEAT:
		if (new_press) {
			count++;
			time = time_now;
			return 0;
		}
		if (elapsed < kbd_repeat_interval + kbd_threshold) {
			return 0;
		}
		mode = IDLE;
		return count;
	}
}
