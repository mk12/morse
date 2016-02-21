// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef CIRCLE_H
#define CIRCLE_H

// A circular buffer of characters.
struct Circle {
	char *buf;
	int size;
	int index;
};

// Initializes an empty circular buffer.
void init_empty(struct Circle *c, char *buf, int size);

// Returns the character at the current position of the circular buffer.
char peek(const struct Circle *c);

// Inserts a character into the current position of the circular buffer.
void insert(struct Circle *c, char ch);

// Advances the current position of the circular buffer.
void advance(struct Circle *c);

// Equivalent to advancing and then inserting.
void append(struct Circle *c, char ch);

// Prints the circular buffer to stdout.
void print_circle(const struct Circle *c);

#endif
