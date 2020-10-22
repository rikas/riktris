CC = gcc
FLAGS = -Wall `pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5`
OBJ = main.o playfield.o game.o
BIN = riktris

.DEFAULT_GOAL := build

main.o: main.c
	$(CC) -c main.c

field.o: playfield.c
	$(CC) -c playfield.c

game.o: game.c
	$(CC) -c game.c

build: main.o playfield.o
	$(CC) -o $(BIN) $(OBJ) $(FLAGS)
