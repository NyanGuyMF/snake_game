c99 = gcc -std=c99 -Wall
SOURCES = src/snake.h src/snake.c src/foodlist.h src/foodlist.c src/game.h src/game.c src/main.c
LIBS = -lncurses

all: $(SOURCES)
	$(c99) $(SOURCES) $(LIBS) -o snake.bin

all-dbg: $(SOURCES)
	$(c99) $(SOURCES) $(LIBS) -g -o snake.bin
