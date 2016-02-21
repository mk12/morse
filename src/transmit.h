// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef TRANSMIT_H
#define TRANSMIT_H

// Allows the user to input Morse code by tapping keys on the keyboard. Prints
// the code, the decoded characters, and timing visuals in an interactive,
// ncurses-like manner. The time_unit is the expected duration of a "dot" in
// milliseconds. Returns 0 on success, 1 on failure.
int transmit(int time_unit);

#endif
