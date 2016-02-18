// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef UTIL_H
#define UTIL_H

// The name of the executable. Set in main.
extern const char *prog_name;

// Prints the usage message to stdout if the status is zero, and to stderr if it
// is nonzero. Returns the status for convenience.
int print_usage(int status);

// Prints an error message to stderr. Returns 1 for convenience.
int print_error(const char *msg);

// Parse a string as an int. Exits on failure.
int parse_int(const char *str);

#endif
