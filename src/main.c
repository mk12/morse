// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "translate.h"
#include "transmit.h"
#include "util.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
	setup_util(argv[0]);

	// Initialize options to default values.
	int mode = 'e';
	int time_unit = 0;

	// Get command line options.
	int c;
	extern char *optarg;
	extern int optind, optopt;
	while ((c = getopt(argc, argv, "hedt:")) != -1) {
		switch (c) {
		case 'h':
			print_usage(stdout);
			return 0;
		case 't':
			if (!parse_int(&time_unit, optarg)) {
				return 1;
			}
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
		print_usage(stderr);
		return 1;
	}

	// Dispatch to the chosen subprogram.
	switch (mode) {
	case 'e':
		return encode();
	case 'd':
		return decode();
	case 't':
		return transmit(time_unit);
	}
}
