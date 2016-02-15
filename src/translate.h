// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef TRANSLATE_H
#define TRANSLATE_H

// Reads characters from standard input, encodes them, and writes the result to
// standard output as dots and dashes. Returns 0 on sucess, 1 on failure.
int encode(void);

// Reads dots and dashes from standard input, decodes them, and writes the
// result to standard output as characters. Returns 0 on success, 1 on failure.
int decode(void);

#endif
