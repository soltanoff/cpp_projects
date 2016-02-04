#ifndef G_MISSIONS
#define G_MISSIONS
#include <SFML\Graphics.hpp>


extern sf::Texture quest_texture;
extern sf::Sprite mission_textbox_sprite;

void set_mission_textbox();
void get_mission_text(sf::RenderWindow &window, sf::Text &text, int x, int y, short mission_number);
#endif /* G_MISSIONS */