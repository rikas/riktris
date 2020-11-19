#ifndef __INPUT_H
#define __INPUT_H

#include <allegro5/allegro5.h>
#include "config.h"

#define KEY_SEEN       1
#define KEY_RELEASED   2

typedef struct Input {
  unsigned char pressed_keys[ALLEGRO_KEY_MAX];
  int first_press_delay;
  int keypress_delay[ALLEGRO_KEY_MAX];
} Input;


extern Input *input;

void input_init();
void set_key_delay(int keycode, unsigned int delay);
bool key_down(int keycode);
bool key_up(int keycode);
int wait_for_keypress(ALLEGRO_EVENT_QUEUE *event_queue);
bool key_pressed(int keycode);
bool delayed_press(int keycode);
void input_refresh();
#endif
