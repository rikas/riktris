#include "music.h"

void play_music()
{
	ALLEGRO_AUDIO_STREAM *stream = al_load_audio_stream("data/sfx/tetris_song.ogg", 3, 2048);
	must_init(stream, "music");

	al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_LOOP);
	al_attach_audio_stream_to_mixer(stream, al_get_default_mixer());
}
