// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "code.h"

#include <assert.h>
#include <stdio.h>

#define N_CODES 26
#define CODE_LIMIT 128
#define N_SIZE_BITS 3
#define SIZE_MASK 0b111

static const Code code_table[N_CODES] = {
	// D = dot/dash 0/1
	// S = size
	//DDDDSSS
	  0b01010, // A
	0b1000100, // B
	0b1010100, // C
	 0b100011, // D
	   0b0001, // E
	0b0010100, // F
	 0b110011, // G
	0b0000100, // H
	  0b00010, // I
	0b0111100, // J
	 0b101011, // K
	0b0100100, // L
	  0b11010, // M
	  0b10010, // N
	 0b111011, // O
	0b0110100, // P
	0b1101100, // Q
	 0b010011, // R
	 0b000011, // S
	   0b1001, // T
	 0b001011, // U
	0b0001100, // V
	 0b011011, // W
	0b1001100, // X
	0b1011100, // Y
	0b1100100  // Z
};

static const char char_table[CODE_LIMIT] = {
	// D = dot/dash 0/1
	// S = size
	// DDDDSSS
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

bool valid_char(char ch) {
	return ch >= 'A' && ch <= 'Z';
}

Code add_size(Code bits, int size) {
	assert(size <= SIZE_MASK);
	return (bits << N_SIZE_BITS) | (Code)size;
}

Code char_to_code(char ch) {
	assert(ch >= 'A' && ch <= 'Z');
	return code_table[ch - 'A'];
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
