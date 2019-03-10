LIBS  = -lncurses
CFLAGS = -Wall

SRC=$(wildcard *.c)

main: $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)
