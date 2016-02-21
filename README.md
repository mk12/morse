# Morse

This program is a Morse code utility. It has three modes:

1. **Encode**: Convert regular text to Morse code.
2. **Decode**: Convert Morse code back to regular text.
3. **Transmit**: Pretend you're operating a telegraph.

It supports ASCII letters, numbers, and most punctuation. See [code.c](src/code.c) for the full list.

## Build

I use the [tup build system][1] for this project. On OS X, it's easy:

1. `brew install homebrew/fuse/tup`
2. `cd /path/to/morse`
3. `tup`
4. `bin/morse`

[1]: http://gittup.org/tup/

## Usage

The encode and decode modes operate on streams, so you can use them interactively or in pipelines.

```sh
$ morse -h
usage: morse [-e | -d | -t]
$ echo "The quick brown fox" | morse -e
- .... . / --.- ..- .. -.-. -.- / -... .-. --- .-- -. / ..-. --- -..-
$ echo "- .... . / --.- ..- .. -.-. -.-" | morse -d
THE QUICK
$ echo "- .... .. ... / .. ... / -- --- .-. ... . / -.-. --- -.. ." | morse -d
THIS IS MORSE CODE
```

## Transmit mode

In transmit mode, you use the keyboard to enter Morse code. A short tap will produce a dot, and a longer press will produce a dash. The dots and dashes, as well as the decoded characters, will appear on screen as you enter them.

As there are no key release events in ANSI terminals, this part was difficult to implement. Instead of detecting when a key is released, I rely on key repeats. When you run `morse -t`, it will ask you to hold down a key for a second to calibrate.

This program should probably use ncurses, but it doesn't. I use a pthread for non-blocking input, and ANSI escape codes to overwite previous output. I make no guarantees about portability. In fact, I would be pleasantly surprised if it works for anyone else.

## License

© 2016 Mitchell Kember

Morse is available under the MIT License; see [LICENSE](LICENSE.md) for details.
