#ifndef G_SOUNDS
#define G_SOUNDS
#include <SFML/Audio.hpp>



extern sf::Sound beep_sound;
extern sf::Sound fail_sound;
extern sf::Sound heal_sound;

void install_sound(sf::Sound &sound, sf::String file_name);
void sounds_settings();
#endif /* G_SOUNDS */