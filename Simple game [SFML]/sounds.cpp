#include "sounds.h"


/*
sf::Sound beep_sound; sf::SoundBuffer beep_sound_buffer;

sf::Sound fail_sound; sf::SoundBuffer fail_sound_buffer;

sf::Sound heal_sound; sf::SoundBuffer heal_sound_buffer;
*/

void Sounds::install_sound(sf::SoundBuffer &sound_buffer, sf::Sound &sound, sf::String file_name)
{
	sound_buffer.loadFromFile("Sounds/" + file_name);
	sound.setBuffer(sound_buffer);
}

void Sounds::sounds_settings()
{
	install_sound(beep_sound_buffer, beep_sound, "blink.wav");
	install_sound(fail_sound_buffer, fail_sound, "fail.wav");
	install_sound(heal_sound_buffer, heal_sound, "heal.wav");
}