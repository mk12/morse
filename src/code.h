// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef CODE_H
#define CODE_H

#include <stdbool.h>

#define MAX_SIZE 7

typedef unsigned int Code;

bool valid_char(char ch);
Code add_size(Code bits, int size);
Code char_to_code(char ch);
char code_to_char(Code code);
void print_dots_dashes(Code code);

#endif
