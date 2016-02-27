// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>

// Performs necessary setup. Must be called once when the program starts.
void setup_util(const char *program_name);

// Prints an error message to stderr.
void print_error(const char *msg);

// Returns the time in milliseconds since the program started.
long current_millis(void);

#endif
