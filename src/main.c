#define DEBUGMODE
#define ALLEGRO_STATICLINK

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <unistd.h> // MAC OS X only
#endif

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/file.h>
#include <allegro5/allegro_image.h>
#include <allegro5/debug.h>
#include "globals.h"
#include "game.h"
#include "gfx.h"
#include "config.h"
#include "music.h"
#include "sfx.h"

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
int main()
{
	// Seed random
	srand(time(NULL));

	must_init(al_init(), "allegro lib");
	must_init(al_init_image_addon(), "image addon");
	must_init(al_install_keyboard(), "keyboard");

	// For MAC OS X bundle to find the resources
#ifdef __APPLE__
	chdir(al_path_cstr(al_get_standard_path(ALLEGRO_RESOURCES_PATH), '/'));
#endif

	// ALLEGRO_PATH* path = al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	// al_change_directory(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
	// al_destroy_path(path);

	// Load all game graphics into memory
	init_gfx();

	must_init(al_install_audio(), "audio");
	must_init(al_init_acodec_addon(), "audio codecs");
	must_init(al_reserve_samples(16), "reserve samples");

	// Load all game sounds into memory
	init_sfx();

	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
	ALLEGRO_DISPLAY *disp = al_create_display(WINDOW_W, WINDOW_H);
	ALLEGRO_FONT *font = al_create_builtin_font();

	ALLEGRO_TRANSFORM t;

	const float scale_factor_x = ((float)al_get_display_width(disp)) / WINDOW_W;
	const float scale_factor_y = ((float)al_get_display_height(disp)) / WINDOW_H;

	// Scale the display to show emulated WINDOW_W and WINDOW_H resolution
	al_identity_transform(&t);
	al_scale_transform(&t, scale_factor_x, scale_factor_y);
	al_use_transform(&t);

#if defined(_WIN32) || defined(_WIN64)
	al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_DIRECT3D_INTERNAL);
#else
	al_set_new_display_flags(ALLEGRO_PROGRAMMABLE_PIPELINE | ALLEGRO_OPENGL);
#endif

	al_set_window_title(disp, "Riktris");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(disp));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	// play_music();
	game_main_loop(queue, font);

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	destroy_config();
	destroy_gfx();

	return 0;
}
