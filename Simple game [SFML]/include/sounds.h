#ifndef G_SOUNDS
#define G_SOUNDS
#include <SFML/Audio.hpp>


class Sounds
{
private:
	sf::Sound beep_sound; sf::SoundBuffer beep_sound_buffer;
	sf::Sound fail_sound; sf::SoundBuffer fail_sound_buffer;
	sf::Sound heal_sound; sf::SoundBuffer heal_sound_buffer;
	void install_sound(sf::SoundBuffer &sound_buffer, sf::Sound &sound, sf::String file_name);
	void sounds_settings();
public:
	Sounds() { sounds_settings(); }

	void play_beep() { beep_sound.play(); }
	void play_fail() { fail_sound.play(); }
	void play_heal() { heal_sound.play(); }
};
#endif /* G_SOUNDS */