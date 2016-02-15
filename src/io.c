// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "io.h"

#include "code.h"

#include <stdio.h>

void encode(void) {
	int ch;
	while ((ch = getchar()) != EOF) {
		if (ch >= 'a' && ch <= 'z') {
			ch += 'A' - 'a';
		}
		if (ch >= 'A' && ch <= 'Z') {
			print_dd(char_to_code((char)ch));
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
			code = add_size(code, size);
			char dec = code_to_char(code);
			putchar(dec == '\0' ? '?' : dec);
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

		if (size > MAX_SIZE) {
			putchar('?');
			code = 0;
			size = 0;
		}
	}
}
