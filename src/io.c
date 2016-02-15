// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "io.h"

#include "code.h"

#include <stdio.h>

static const char not_decodable = '?';

static char transform(char ch) {
	if (ch >= 'a' && ch <= 'z') {
		return ch + 'A' - 'a';
	} else {
		return ch;
	}
}

void encode(void) {
	int input;
	while ((input = getchar()) != EOF) {
		char ch = transform((char)input);
		if (valid_char(ch)) {
			print_dots_dashes(char_to_code(ch));
			putchar(' ');
		} else {
			putchar(ch);
		}
	}
}

void decode(void) {
	int ch = 0;
	int size = 0;
	Code code = 0;
	for (;;) {
		ch = getchar();
		if (ch == '.') {
			code <<= 1;
			size++;
		} else if (ch == '-') {
			code <<= 1;
			code |= 1;
			size++;
		} else if (size > 0) {
			if (size > MAX_SIZE) {
				putchar(not_decodable);
			} else {
				code = add_size(code, size);
				char dec = code_to_char(code);
				putchar(dec ? dec : not_decodable);
			}
			if (ch == '\n') {
				putchar('\n');
			}
			code = 0;
			size = 0;
		} else if (ch == EOF) {
			break;
		} else {
			putchar(ch);
		}
	}
}
