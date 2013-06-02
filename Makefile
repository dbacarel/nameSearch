
CC=g++

CFLAGS= -g -O3

DEPS = nameSearch.h

all: nameSearch


nameSearch: nameSearch.o
	$(CC) -o nameSearch nameSearch_main.cc nameSearch.o 

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< 

clean:
	rm -rf *o 



