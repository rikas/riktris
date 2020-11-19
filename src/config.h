#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_FILE "configuration.cfg"

#include <allegro5/allegro5.h>

typedef struct Config {
  ALLEGRO_CONFIG *file;
  int ghost;

  int key_left;
  int key_right;
  int key_down;
  int key_hard_drop;
  int key_rotate_right;
  int key_rotate_left;
  int key_hold;
  int key_pause;
} Config;

extern Config *config;
void config_load();
void destroy_config();
int get_config_int(const char *section, const char *key, int def);

#endif
