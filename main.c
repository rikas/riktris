#define DEBUGMODE
#define ALLEGRO_STATICLINK
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h> // MAC OS X only
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/file.h>
#include <allegro5/allegro_image.h>
#include "globals.h"
#include "game.h"

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
	// Seed random
	srand(time(NULL));

	if (!al_init())
	{
		fprintf(stderr, "Failed to initialize Allegro.\n");
		return 1;
	}

	// For MAC OS X bundle to find the resources
	chdir(al_path_cstr(al_get_standard_path(ALLEGRO_RESOURCES_PATH), '/'));

	if (!al_init_image_addon())
	{
		fprintf(stderr, "Couldn't initialize image addon.\n");
		return 1;
	}

	// Install keyboard
	al_install_keyboard();

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
	al_start_timer(timer);

	game_main_loop(queue, font);

	al_destroy_font(font);
	al_destroy_display(disp);
	al_destroy_timer(timer);
	al_destroy_event_queue(queue);

	return 0;
}
