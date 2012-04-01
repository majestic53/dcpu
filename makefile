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

build: dcpu.o mem128.o reg16.o

dcpu: build $(SRC)$(MAIN).cpp
	$(CC) $(FLAG) -o $(APP) $(SRC)$(MAIN).cpp $(SRC)dcpu.o $(SRC)mem128.o $(SRC)reg16.o

dcpu.o: $(SRC)dcpu.cpp $(SRC)dcpu.hpp
	$(CC) $(FLAG) -c $(SRC)dcpu.cpp -o $(SRC)dcpu.o

mem128.o: $(SRC)mem128.cpp $(SRC)mem128.hpp
	$(CC) $(FLAG) -c $(SRC)mem128.cpp -o $(SRC)mem128.o

reg16.o: $(SRC)reg16.cpp $(SRC)reg16.hpp
	$(CC) $(FLAG) -c $(SRC)reg16.cpp -o $(SRC)reg16.o
