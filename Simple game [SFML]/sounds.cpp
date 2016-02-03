#include "sounds.h"



sf::Sound beep_sound;
sf::SoundBuffer sound_buffer;

void sounds_settings()
{
	sound_buffer.loadFromFile("Sounds/blink.wav");
	beep_sound.setBuffer(sound_buffer);
	beep_sound.setPlayingOffset(sf::seconds(2));
	beep_sound.play();
}