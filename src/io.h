// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef IO_H
#define IO_H

// Reads characters from standard input, encodes them, and writes the result to
// standard output as dots and dashes.
void encode(void);

// Reads dots and dashes from standard input, decodes them, and writes the
// result to standard output as characters.
void decode(void);

// Allows the user to input Morse code by tapping a key. Decodes the signal and
// prints the characters interactively.
void interactive(void);

#endif
