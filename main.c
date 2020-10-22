#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/file.h>
#include <allegro5/allegro_image.h>
#include "globals.h"
#include "playfield.h"
#include "game.h"

bool running = true;

const unsigned int ROTATIONS = 4;

// Because a tetromino is basically a set of 'big pixels' that can be either on or off, it is quite
// suitable and efficient to represent it as a bitmask rather than a matrix of integers.

// Example for the S shape:

// X . . .     1 0 0 0
// X X . .  =  1 1 0 0  =  1000110001000000 (in binary)  =  0x8C40 (in hexadecimal)
// . X . .     0 1 0 0
// . . . .     0 0 0 0

// . X X .     0 1 1 0
// X X . .  =  1 1 0 0  =  0110110000000000 (in binary)  =  0x6C00 (in hexadecimal)
// . . . .     0 0 0 0
// . . . .     0 0 0 0
unsigned int TETRIMINOS[7][ROTATIONS] = {
	{0x4E00, 0x4640, 0x0E40, 0x4C40}, // T
	{0x8C40, 0x6C00, 0x8C40, 0x6C00}, // S
	{0x4C80, 0xC600, 0x4C80, 0xC600}, // Z
	{0x4444, 0x0F00, 0x4444, 0x0F00}, // I
	{0x44C0, 0x8E00, 0xC880, 0xE200}, // J
	{0x88C0, 0xE800, 0xC440, 0x2E00}, // L
	{0xCC00, 0xCC00, 0xCC00, 0xCC00}  // O
};

void draw_piece(unsigned int shape, ALLEGRO_BITMAP *mino);

int main()
{
	// Seed random
	srand(time(NULL));

	if (!al_init())
	{
		fprintf(stderr, "Failed to initialize Allegro.\n");
		return 1;
	}

	if (!al_init_image_addon())
	{
		fprintf(stderr, "Couldn't initialize image addon.\n");
		return 1;
	}

	// Install keyboard
	al_install_keyboard();

	al_init_primitives_addon();

	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	ALLEGRO_DISPLAY *disp = al_create_display(WINDOW_W * 2, WINDOW_H * 2);
	ALLEGRO_FONT *font = al_create_builtin_font();

	ALLEGRO_TRANSFORM t;

	const float scale_factor_x = ((float)al_get_display_width(disp)) / WINDOW_W;
	const float scale_factor_y = ((float)al_get_display_height(disp)) / WINDOW_H;

	// Scale the display to show emulated WINDOW_W and WINDOW_H resolution
	al_identity_transform(&t);
	al_scale_transform(&t, scale_factor_x, scale_factor_y);
	al_use_transform(&t);

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	Playfield *playfield = playfield_init();

	ALLEGRO_EVENT event;

	al_start_timer(timer);

	char str[20];
	char rot_str[20];
	int rotation;
	int tetrimino_num = rand() % 7;

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}

void draw_piece(unsigned int shape, ALLEGRO_BITMAP *mino)
{
	unsigned int x, y, tx, ty;

	for (x = 0; x < ROTATIONS; x++)
	{
		for (y = 0; y < ROTATIONS; y++)
		{
			tx = x * TETRIMINO_W;
			ty = y * TETRIMINO_W;

			if (shape & (0x8000 >> (y * ROTATIONS + x)))
			{
				al_draw_bitmap(mino, tx, ty, 0);
			}
		}
	}
}
