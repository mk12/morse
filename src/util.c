// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "util.h"

#include <stdio.h>
#include <stdlib.h>

const char *prog_name = NULL;

int print_usage(int status) {
	FILE *f = status ? stderr : stdout;
	fprintf(f, "usage: %s [-e | -d | -i time_unit]\n", prog_name);
	return status;
}

int print_error(const char *msg) {
	fprintf(stderr, "%s: %s\n", prog_name, msg);
	return 1;
}

int parse_int(const char *str) {
	char *end;
	int n = (int)strtol(str, &end, 0);
	if (*end) {
		fprintf(stderr, "%s: %s: not an integer\n", prog_name, str);
		exit(1);
	}
	return n;
}
