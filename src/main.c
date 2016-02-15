// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "io.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char *usage_msg = "usage: morse [-e | -d | -i]\n";

int main(int argc, char **argv) {
	if (argc != 2 || strlen(argv[1]) != 2 || *argv[1] != '-') {
		fputs(usage_msg, stderr);
		return 1;
	}
	switch (argv[1][1]) {
	case 'h':
		fputs(usage_msg, stdout);
		break;
	case 'e':
		encode();
		break;
	case 'd':
		decode();
		break;
	case 'i':
		interactive();
		break;
	default:
		fputs(usage_msg, stderr);
		return 1;
	}
	return 0;
}
