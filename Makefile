CC = gcc
LDFLAGS = -Wall `pkg-config --cflags --libs allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_audio-5 allegro_acodec-5`
OBJ = obj/gfx.o obj/main.o obj/playfield.o obj/game.o obj/tetriminos.o obj/mino_queue.o obj/input.o obj/config.o obj/utils.o obj/music.o
BIN = build/riktris
COMPILE_OPTS= -Wall -x c --std=c99

.DEFAULT_GOAL := build

obj/main.o:
	$(CC) $(COMPILE_OPTS) -c src/main.c -o obj/main.o

obj/playfield.o:
	$(CC) $(COMPILE_OPTS) -c src/playfield.c -o obj/playfield.o

obj/game.o:
	$(CC) $(COMPILE_OPTS) -c src/game.c -o obj/game.o

obj/tetriminos.o:
	$(CC) $(COMPILE_OPTS) -c src/tetriminos.c -o obj/tetriminos.o

obj/mino_queue.o:
	$(CC) $(COMPILE_OPTS) -c src/mino_queue.c -o obj/mino_queue.o

obj/gfx.o:
	$(CC) $(COMPILE_OPTS) -c src/gfx.c -o obj/gfx.o

obj/input.o:
	$(CC) $(COMPILE_OPTS) -c src/input.c -o obj/input.o

obj/config.o:
	$(CC) $(COMPILE_OPTS) -c src/config.c -o obj/config.o

obj/utils.o:
	$(CC) $(COMPILE_OPTS) -c src/utils.c -o obj/utils.o

obj/music.o:
	$(CC) $(COMPILE_OPTS) -c src/music.c -o obj/music.o

clean:
	rm -rf obj/*.o

build: clean $(OBJ)
	$(CC) -Wall -o $(BIN) $(OBJ) $(LDFLAGS)

run: clean build
	./build/riktris

debug: $(OBJ)
	$(CC) -g -O0 -o $(BIN) $(OBJ) $(LDFLAGS)
