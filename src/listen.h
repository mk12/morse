// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef LISTEN_H
#define LISTEN_H

#include <stdbool.h>

// Prompts the user to hold down a key and calibrates to the keyboard repeat
// settings. This must be called once before spawning the listener thread.
// Returns EOF if an end-of-file condition occurred; otherwise, returns 0.
int calibrate_listener(void);

// Spawns the keyboard listener thread. Returns true on success.
bool spawn_listener(void);

// Gets the listener's repeat count. Must have already spawned the listener.
// Returns 0 initially; after a key has been pressed and released, the next call
// returns the number of key repeats that occurred, including the intial press.
// Subsequent calls return 0 until a key is pressed and released again. Returns
// EOF if an end-of-file condition is encountered.
int get_listener_count(void);

#endif
