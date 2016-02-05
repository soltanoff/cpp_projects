#ifndef G_MISSIONS
#define G_MISSIONS
#include <SFML\Graphics.hpp>
#include "simple_map.h"

const short DEFAULT_MISSION = 0;

extern sf::Texture quest_texture;
extern sf::Sprite mission_textbox_sprite;
//extern short mission_number;

void set_mission_textbox();
void set_new_mission(short mission_number);
void get_mission_text(sf::RenderWindow &window, sf::Text &text, int x, int y, short mission_number);
#endif /* G_MISSIONS */