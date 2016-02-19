// Copyright 2016 Mitchell Kember. Subject to the MIT License.

#ifndef KEYBOARD_H
#define KEYBOARD_H

// ...
extern enum { KBD_UP, KDB_DOWN, KDB_EOF } kbd_input;

// ...
void set_stdin_unbuffered(void);

// ...
void calibrate_keyboard(void);

// ...
void *listen_keyboard(void *);

#endif
