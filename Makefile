LIBS  = -lncurses
CFLAGS = -Wall

SRC=$(wildcard *.c)

game: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
