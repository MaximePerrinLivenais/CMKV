CC=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -pedantic

SRC=$(wildcard *.cc)
OBJ=${SRC:.cc=.o}

all: a.out

a.out: $(OBJ)
	$(CC) $(CXXFLAGS) $^ -o $@

.PHONY: clean

clean:
	$(RM) $(OBJ) a.out
