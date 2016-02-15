// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "translate.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// The usage message for this program.
static const char *usage_msg = "usage: morse [-e | -d | -i]\n";

int main(int argc, char **argv) {
	if (argc != 2 || strlen(argv[1]) != 2 || *argv[1] != '-') {
		fputs(usage_msg, stderr);
		return 1;
	}
	switch (argv[1][1]) {
	case 'h':
		fputs(usage_msg, stdout);
		return 0;
	case 'e':
		return encode();
	case 'd':
		return decode();
	case 'i':
		return interactive();
	default:
		fputs(usage_msg, stderr);
		return 1;
	}
}
