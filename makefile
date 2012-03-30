# dcpu Makefile
# Copyright (C) 2012 David Jolly

CC=g++
APP=dcpu
MAIN=main
SRC=src/
FLAG=-std=c++0x -O3 -funroll-all-loops

all: build dcpu

clean:
	rm -f $(SRC)*.o $(APP)

build: _cpu.o _memory.o _register.o

dcpu: build $(SRC)$(MAIN).cpp
	$(CC) $(FLAG) -o $(APP) $(SRC)$(MAIN).cpp $(SRC)_cpu.o $(SRC)_memory.o $(SRC)_register.o

_cpu.o: $(SRC)_cpu.cpp $(SRC)_cpu.hpp
	$(CC) $(FLAG) -c $(SRC)_cpu.cpp -o $(SRC)_cpu.o

_memory.o: $(SRC)_memory.cpp $(SRC)_memory.hpp
	$(CC) $(FLAG) -c $(SRC)_memory.cpp -o $(SRC)_memory.o

_register.o: $(SRC)_register.cpp $(SRC)_register.hpp
	$(CC) $(FLAG) -c $(SRC)_register.cpp -o $(SRC)_register.o
