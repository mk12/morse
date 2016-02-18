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
			if (mode != FIRST) {
				mode = NEW_WORD;
			}
			break;
		case '\n':
			if (mode != FIRST) {
				putchar('\n');
				mode = FIRST;
			}
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
			// Print dots and dashes if possible.
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
	int input;
	bool first = true;
	int size = 0;
	Code code = 0;
	while ((input = getchar()) != EOF) {
		switch (input) {
		case '.':
			// Encode a dot.
			code <<= 1;
			size++;
			break;
		case '-':
			// Encode a dash.
			code <<= 1;
			code |= 1;
			size++;
			break;
		case ' ':
		case '\t':
		case '\n':
			if (size > 0) {
				if (size > MAX_SIZE) {
					// The sequence of dots and dashes was too long.
					putchar(not_decodable);
				} else {
					// Decode and print the character if possible.
					code = add_size(code, size);
					char dec = code_to_char(code);
					putchar(dec ? dec : not_decodable);
				}
				first = false;
				code = 0;
				size = 0;
			}
			// Print a newline if there has been output on this line.
			if (!first && input == '\n') {
				putchar('\n');
				first = true;
			}
			break;
		case '/':
			// Print the word separator.
			putchar(' ');
			first = false;
			break;
		default:
			// Print the input character unchanged.
			putchar(input);
			first = false;
			break;
		}
	}
	return 0;
}
