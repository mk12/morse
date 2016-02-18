// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef INTERACT_H
#define INTERACT_H

// Allows the user to input Morse code by tapping two keys alternately. Decodes
// the signal and prints the characters interactively. The time_unit determines
// the necessary tapping speed; it is the duration of a "dot" in milliseconds.
// Returns 0 on success, 1 on failure.
int interact(int time_unit);

#endif
