# Bitmap Toy Project (bmpt)

A program that play with bitmap images.

## Build

- Requires `clang` and `make`. You can use `gcc` by editing `CC` in `Makefile`.
- In the root of this repo, run:

```sh
$ make
```

- This will build the `bmpt` program to `bin`.

## Usage

- Print a bitmap image in your terminal

```sh
$ bmpt <path to file>
```

- Print some images that I've made

```sh
$ bmpt
```

## Development

- Run the tests in `./src/test.c`

```sh
$ make test
```
