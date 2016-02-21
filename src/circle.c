// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#include "circle.h"

#include <stdio.h>

void insert(struct Circle *c, char ch) {
	c->buf[c->index] = ch;
}

void advance(struct Circle *c) {
	c->index = (c->index + 1) % c->size;
}

void append(struct Circle *c, char ch) {
	advance(c);
	insert(c, ch);
}

char peek(const struct Circle *c) {
	return c->buf[c->index];
}

void print_circle(const struct Circle *c) {
	int size = c->size;
	int next = c->index + 1;
	if (next < size && c->buf[next]) {
		fwrite(c->buf + next, 1, (size_t)(size - next), stdout);
	}
	fwrite(c->buf, 1, (size_t)(c->index + 1), stdout);
}
