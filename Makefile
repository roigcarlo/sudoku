BINARIES=sudoku_cpp sudoku_c

CC=g++

CFLAGS=-Wall -O3

all: $(BINARIES)

clean:
	rm -f $(BINARIES)

