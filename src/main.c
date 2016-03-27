// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "translate.h"
#include "transmit.h"
#include "util.h"

#include <stdio.h>
#include <unistd.h>

// The usage message for the program.
static const char *const usage_message = "usage: morse [-e | -d | -t]\n";

int main(int argc, char **argv) {
	setup_util(argv[0]);

	// Initialize the default mode.
	int mode = 'e';

	// Get command line options.
	int c;
	extern char *optarg;
	extern int optind, optopt;
	while ((c = getopt(argc, argv, "hedt")) != -1) {
		switch (c) {
		case 'h':
			fputs(usage_message, stdout);
			return 0;
		case 'e':
		case 'd':
		case 't':
			mode = c;
			break;
		case '?':
			fputs(usage_message, stderr);
			return 1;
		}
	}
	// Make sure all arguments were processed.
	if (optind != argc) {
		fputs(usage_message, stderr);
		return 1;
	}

	// Dispatch to the chosen subprogram.
	switch (mode) {
	case 'e':
		return encode();
	case 'd':
		return decode();
	case 't':
		return transmit();
	}
}
