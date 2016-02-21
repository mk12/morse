// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>

// Performs necessary setup. Must be called once when the program starts.
void setup_util(const char *program_name);

// Prints the usage message to the given output stream.
void print_usage(FILE *out);

// Prints an error message to stderr.
void print_error(const char *msg);

// Parse a string as an int. Returns true on success.
bool parse_int(int *out, const char *str);

// Returns the time in milliseconds since the program started.
long current_millis(void);

#endif
