CC = gcc
LDFLAGS = -Wall `pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5`
OBJ = main.o playfield.o game.o tetriminos.o mino_queue.o
BIN = riktris

.DEFAULT_GOAL := build

main.o: main.c
	$(CC) -c main.c

field.o: playfield.c
	$(CC) -c playfield.c

game.o: game.c
	$(CC) -c game.c

tetriminos.o: tetriminos.c
	$(CC) -c tetriminos.c

mino_queue.o: mino_queue.c
	$(CC) -c mino_queue.c

build: $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

debug: main.o playfield.o game.o tetriminos.o
	$(CC) -g -O0 -o $(BIN) $(OBJ) $(LDFLAGS)
