// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "io.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char *usage_msg = "usage: morse [-d]\n";

int main(int argc, char **argv) {
	if (argc > 2) {
		fputs(usage_msg, stderr);
		return 1;
	}
	if (argc == 1) {
		encode();
		return 0;
	}
	if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		fputs(usage_msg, stdout);
		return 0;
	}
	if (strcmp(argv[1], "-d") == 0) {
		decode();
		return 0;
	}
	fputs(usage_msg, stderr);
	return 1;
}
