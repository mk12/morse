// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "util.h"

#include <stdlib.h>
#include <sys/time.h>

// The name of the executable.
static const char *prog_name = NULL;

// The time when the program started.
static struct timeval startup_tv;

void setup_util(const char *program_name) {
	prog_name = program_name;
	gettimeofday(&startup_tv, NULL);
}

void print_usage(FILE *out) {
	fprintf(out, "usage: %s [-e | -d | -t]\n", prog_name);
}

void print_error(const char *msg) {
	fprintf(stderr, "%s: %s\n", prog_name, msg);
}

long current_millis(void) {
	struct timeval now;
	gettimeofday(&now, NULL);
	long sec = now.tv_sec - startup_tv.tv_sec;
	long micro = now.tv_usec - startup_tv.tv_usec;
	return sec * 1000 + micro / 1000;
}
