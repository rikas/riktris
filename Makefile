CC = gcc
LDFLAGS = -Wall `pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5`
OBJ = obj/gfx.o obj/main.o obj/playfield.o obj/game.o obj/tetriminos.o obj/mino_queue.o obj/input.o obj/config.o
BIN = build/riktris

.DEFAULT_GOAL := build

obj/main.o:
	$(CC) -c src/main.c -o obj/main.o

obj/playfield.o:
	$(CC) -c src/playfield.c -o obj/playfield.o

obj/game.o:
	$(CC) -c src/game.c -o obj/game.o

obj/tetriminos.o:
	$(CC) -c src/tetriminos.c -o obj/tetriminos.o

obj/mino_queue.o:
	$(CC) -c src/mino_queue.c -o obj/mino_queue.o

obj/gfx.o:
	$(CC) -c src/gfx.c -o obj/gfx.o

obj/input.o:
	$(CC) -c src/input.c -o obj/input.o

obj/config.o:
	$(CC) -c src/config.c -o obj/config.o

clean:
	rm -rf obj/*.o

build: clean $(OBJ)
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)

run: clean build
	./build/riktris

debug: $(OBJ)
	$(CC) -g -O0 -o $(BIN) $(OBJ) $(LDFLAGS)
