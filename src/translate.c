// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "translate.h"

#include "code.h"

#include <stdbool.h>
#include <stdio.h>

// The character to print when given invalid Morse code.
static const char not_decodable = '?';

// Transforms a character, if possible, to make it encodable.
static char transform(char ch) {
	if (ch >= 'a' && ch <= 'z') {
		return ch + 'A' - 'a';
	} else {
		return ch;
	}
}

int encode(void) {
	int input;
	enum { FIRST, NEW_LETTER, NEW_WORD } mode = FIRST;
	while ((input = getchar()) != EOF) {
		switch (input) {
		case ' ':
		case '\t':
			mode = NEW_WORD;
			break;
		case '\n':
			putchar('\n');
			mode = FIRST;
			break;
		default:;
			char ch = transform((char)input);
			Code code = char_to_code(ch);
			// Print the letter or word separator.
			switch (mode) {
			case FIRST:
				break;
			case NEW_WORD:
				putchar(' ');
				putchar('/');
				// fall through
			case NEW_LETTER:
				putchar(' ');
				break;
			}
			// Print dots and dashes.
			if (code) {
				print_dots_dashes(code);
			} else {
				putchar(input);
			}
			mode = NEW_LETTER;
			break;
		}
	}
	return 0;
}

int decode(void) {
	int ch = 0;
	int size = 0;
	Code code = 0;
	for (;;) {
		ch = getchar();
		if (ch == EOF) {
			break;
		} else if (ch == '.') {
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
				// Decode and print the character.
				code = add_size(code, size);
				char dec = code_to_char(code);
				putchar(dec ? dec : not_decodable);
			}
			if (ch != ' ') {
				putchar(ch);
			}
			code = 0;
			size = 0;
		} else {
			putchar(ch);
		}
	}
	return 0;
}
