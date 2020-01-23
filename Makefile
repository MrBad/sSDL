DFLAGS=
INCLUDE=.
LIBS=-lSDL2
CC=gcc
OFLAGS=-c
CFLAGS=-g3 -Wall -Wextra -std=c99 -pedantic -I$(INCLUDE) $(DFLAGS)
MAKE=make

TARGET=main
OBJECTS=main.o

all: $(TARGET) tester test

$(TARGET): $(OBJECTS) Makefile
	$(CC) $(CFLAGS) $(DFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%o: %.c Makefile
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

run: $(TARGET) *.o *.c *.h
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) tester tester.o _main.o

tester: tester.o main.o
	strip -N main main.o -o _main.o
	$(CC) -o $@ tester.o _main.o $(LDFLAGS) $(LIBS)

test:
	./tester
