// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "code.h"

#include <assert.h>
#include <stdio.h>

#define CODE_LIMIT 256
#define CHAR_LIMIT 128

#define N_SIZE_BITS 3
#define SIZE_MASK 0b111

static const Code code_table[CHAR_LIMIT] = {
	['0'] = 0b11111101,
	['1'] = 0b01111101,
	['2'] = 0b00111101,
	['3'] = 0b00011101,
	['4'] = 0b00001101,
	['5'] = 0b00000101,
	['6'] = 0b10000101,
	['7'] = 0b11000101,
	['8'] = 0b11100101,
	['9'] = 0b11110101,
	['A'] = 0b01010,
	['B'] = 0b1000100,
	['C'] = 0b1010100,
	['D'] = 0b100011,
	['E'] = 0b0001,
	['F'] = 0b0010100,
	['G'] = 0b110011,
	['H'] = 0b0000100,
	['I'] = 0b00010,
	['J'] = 0b0111100,
	['K'] = 0b101011,
	['L'] = 0b0100100,
	['M'] = 0b11010,
	['N'] = 0b10010,
	['O'] = 0b111011,
	['P'] = 0b0110100,
	['Q'] = 0b1101100,
	['R'] = 0b010011,
	['S'] = 0b000011,
	['T'] = 0b1001,
	['U'] = 0b001011,
	['V'] = 0b0001100,
	['W'] = 0b011011,
	['X'] = 0b1001100,
	['Y'] = 0b1011100,
	['Z'] = 0b1100100
};

static const char char_table[CODE_LIMIT] = {
	[0b11111101] = '0',
	[0b01111101] = '1',
	[0b00111101] = '2',
	[0b00011101] = '3',
	[0b00001101] = '4',
	[0b00000101] = '5',
	[0b10000101] = '6',
	[0b11000101] = '7',
	[0b11100101] = '8',
	[0b11110101] = '9',
	   [0b01010] = 'A',
	 [0b1000100] = 'B',
	 [0b1010100] = 'C',
	  [0b100011] = 'D',
	    [0b0001] = 'E',
	 [0b0010100] = 'F',
	  [0b110011] = 'G',
	 [0b0000100] = 'H',
	   [0b00010] = 'I',
	 [0b0111100] = 'J',
	  [0b101011] = 'K',
	 [0b0100100] = 'L',
	   [0b11010] = 'M',
	   [0b10010] = 'N',
	  [0b111011] = 'O',
	 [0b0110100] = 'P',
	 [0b1101100] = 'Q',
	  [0b010011] = 'R',
	  [0b000011] = 'S',
	    [0b1001] = 'T',
	  [0b001011] = 'U',
	 [0b0001100] = 'V',
	  [0b011011] = 'W',
	 [0b1001100] = 'X',
	 [0b1011100] = 'Y',
	 [0b1100100] = 'Z'
};

Code add_size(Code bits, int size) {
	assert(size <= SIZE_MASK);
	return (bits << N_SIZE_BITS) | (Code)size;
}

Code char_to_code(char ch) {
	return code_table[(int)ch];
}

char code_to_char(Code code) {
	return char_table[code];
}

void print_dots_dashes(Code code) {
	const int size = code & SIZE_MASK;
	Code mask = 1 << (N_SIZE_BITS + size - 1);
	while (mask > SIZE_MASK) {
		putchar(code & mask ? '-' : '.');
		mask >>= 1;
	}
}
