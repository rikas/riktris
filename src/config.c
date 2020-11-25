#include "config.h"

Config *config;

void config_load()
{
  config = (Config *)malloc(sizeof(Config));
  config->file = al_load_config_file(CONFIG_FILE);
  must_init(config->file, "config file");

  config->key_left = get_config_int("keys", "key_left", ALLEGRO_KEY_LEFT);
  config->key_right = get_config_int("keys", "key_right", ALLEGRO_KEY_RIGHT);
  config->key_down = get_config_int("keys", "key_down", ALLEGRO_KEY_DOWN);
  config->key_rotate_right = get_config_int("keys", "key_rotate_right", ALLEGRO_KEY_UP);
  config->key_rotate_left = get_config_int("keys", "key_rotate_left", ALLEGRO_KEY_Z);
  config->key_hard_drop = get_config_int("keys", "key_hard_drop", ALLEGRO_KEY_SPACE);
  config->key_hold = get_config_int("keys", "key_hold", ALLEGRO_KEY_C);
}

int get_config_int(const char *section, const char *key, int def)
{
  const char *value = al_get_config_value(config->file, section, key);

  return (value) ? atoi(value) : def;
}

// void set_config_int(const char *section, const char *key, int value)
// {
//   char buf[32];

//   sprintf(buf, "%d", value);
//   al_set_config_value(config->file, section, key, buf);
// }

// void save_config()
// {
//   al_save_config_file(CONFIG_FILE, config->file);
// }

void destroy_config()
{
  if (!config) {
    return;
  }

  al_destroy_config(config->file);
  free(config);
}
