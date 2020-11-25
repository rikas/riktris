#include "input.h"

Input *input;

static int get_key_delay(int keycode)
{
	if (keycode == config->key_down)
	{
		return 3;
	}

	if (keycode == config->key_right || keycode == config->key_left)
	{
		return 2;
	}

	return 10000;
}

static int get_key_first_delay(int keycode)
{
	if (keycode == config->key_down)
	{
		return 0;
	}

	if (keycode == config->key_right || keycode == config->key_left)
	{
		return 15;
	}

	return 10000;
}

bool key_up(int keycode)
{
	input->keypress_delay[keycode] = 0;

	return input->pressed_keys[keycode] &= KEY_RELEASED;
}

bool key_down(int keycode)
{
	return input->pressed_keys[keycode] = KEY_SEEN | KEY_RELEASED;
}

bool key_pressed(int keycode)
{
	return input->pressed_keys[keycode];
}

void set_key_delay(int keycode, unsigned int delay)
{
	input->keypress_delay[keycode] = delay;
}

static bool in_cooldown(int keycode)
{
	return input->keypress_delay[keycode] > 0;
}

bool delayed_press(int keycode)
{
	int delay = 0;

	if (key_pressed(keycode) && !in_cooldown(keycode))
	{
		if ((input->pressed_keys[keycode] & KEY_RELEASED) == KEY_RELEASED)
		{
			delay = get_key_first_delay(keycode);
		}
		else
		{
			delay = get_key_delay(keycode);
		}
		set_key_delay(keycode, delay);
		return true;
	}
	else
	{
		return false;
	}
}

int wait_for_keypress(ALLEGRO_EVENT_QUEUE *event_queue)
{
	ALLEGRO_EVENT event;
	do
	{
		al_wait_for_event(event_queue, &event);
	} while (event.type != ALLEGRO_EVENT_KEY_DOWN);

	return event.keyboard.keycode;
}

void input_init()
{
	input = (Input *)malloc(sizeof(Input));
	memset(input->keypress_delay, 0, sizeof(input->keypress_delay));
	memset(input->pressed_keys, 0, sizeof(input->pressed_keys));
}

// void input_destroy()
// {
//   free(input);
// }

// Will decrement the keypress delay and will reset the pressed key states
void input_refresh()
{
	for (int keycode = 0; keycode < ALLEGRO_KEY_MAX; keycode++)
	{
		input->pressed_keys[keycode] &= KEY_SEEN;

		if (input->keypress_delay[keycode] > 0)
		{
			input->keypress_delay[keycode]--;
		}
	}
}
