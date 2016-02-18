// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "interact.h"
#include "translate.h"
#include "util.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	// Set the program name.
	prog_name = argv[0];

	// Initialize options to default values.
	int mode = 'e';
	int time_unit = 0;

	// Get command line options.
	int c;
	extern char *optarg;
	extern int optind, optopt;
	while ((c = getopt(argc, argv, "hedi:")) != -1) {
		switch (c) {
		case 'h':
			return print_usage(0);
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
		return print_usage(1);
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
