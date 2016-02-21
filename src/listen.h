// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef LISTEN_H
#define LISTEN_H

#include <stdbool.h>

// The set of states the listener can report.
enum ListenerState {
	LS_EOF,     // end-of-file
	LS_NONE,    // no keys pressed
	LS_DOWN,    // key pressed
	LS_HOLD,    // key held down
	LS_REPEAT,  // key repeated for the first time
	LS_HOLD_R,  // key held down and repeating
	LS_UP       // key released
};

// Prompts the user to hold down a key and calibrates to the keyboard repeat
// settings. This must be called once before spawning the listener thread.
// Returns EOF if an end-of-file condition occurred; otherwise, returns 0.
int calibrate_listener(void);

// Spawns the keyboard listener thread. Returns true on success.
bool spawn_listener(void);

// Gets the listener's state, given the current program time in milliseconds.
// Must have already spawned the listener.
enum ListenerState get_listener_state(long time_now);

#endif
