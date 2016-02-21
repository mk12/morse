// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef LISTEN_H
#define LISTEN_H

#include <stdbool.h>

// This module simulates the ability to check if a key is pressed or released.
// It does this under the assumption that a pressed key will, after a delay,
// repeat its character input into stdin at fixed intervals.

// Prompts the user to hold down a key and calibrates to the repeat rate. This
// must be called once before spawning the listener thread.
void calibrate_listener(void);

// Spawns the keyboard listener thread. Returns true on success.
bool spawn_listener(void);

// Gets the current state of the listener, given the time since the program
// started in milliseconds. Must have already spawned the listener thread.
// Returns the character of the key currently being pressed; returns 0 if there
// is none. Returns EOF on an end-of-file condition.
int get_listener_state(long time_now);

#endif
