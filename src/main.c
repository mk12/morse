// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "interact.h"
#include "translate.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// String constants.
static const char *prog_name; // set in main
static const char *const usage_msg = "usage: morse [-e | -d | -i time_unit]\n";

// Default parameter values.
static const int default_mode = 'e';
static const int default_time_unit = 100;

// Parse a string as an int. Exits on failure.
static int parse_int(const char *str) {
	char *end;
	int n = (int)strtol(str, &end, 0);
	if (*end) {
		fprintf(stderr, "%s: %s: not an integer\n", prog_name, str);
		exit(1);
	}
	return n;
}

int main(int argc, char **argv) {
	// Set the program name to be used in error messages.
	prog_name = argv[0];

	// Initialize parameters to default values.
	int mode = default_mode;
	int time_unit = default_time_unit;

	// Get command line options.
	int c;
	extern char *optarg;
	extern int optind, optopt;
	while ((c = getopt(argc, argv, "hedi:")) != -1) {
		switch (c) {
		case 'h':
			fputs(usage_msg, stdout);
			return 0;
		case 'i':
			time_unit = parse_int(optarg);
			// fall through
		case 'e':
		case 'd':
			mode = c;
			break;
		case '?':
			return 1;
		}
	}
	// Make sure all arguments were processed.
	if (optind != argc) {
		fputs(usage_msg, stderr);
		return 1;
	}

	// Dispatch to the chosen subprogram.
	switch (mode) {
	case 'e':
		return encode();
	case 'd':
		return decode();
	case 'i':
		return interact(time_unit);
	}
}
