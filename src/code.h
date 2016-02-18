// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef CODE_H
#define CODE_H

#include <stdbool.h>

#define MAX_SIZE 7

// A Code is a Morse code representation of one character. It uses off bits (0)
// to represent dots and on bits (1) to represent dashes. The number of bits
// that contain this information is stored in the lower 3 bits. The actual
// information is stored in the remaning upper bits.
typedef unsigned int Code;

// Encodes size information together with Morse code bits.
Code add_size(Code bits, int size);

// Converts a character to Morse code. Returns zero if it could not convert.
Code char_to_code(char ch);

// Converts Morse code to a character. Returns zero if it could not convert.
char code_to_char(Code code);

// Prints Morse code as a series of dots and dashes.
void print_dots_dashes(Code code);

#endif
