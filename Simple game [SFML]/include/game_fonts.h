#ifndef GAME_FONTS
#define GAME_FONTS
#include <SFML\Graphics.hpp>



extern sf::Font main_font;
extern sf::Font mission_font;
extern sf::Text score_text;
extern sf::Text mission_text;;
extern sf::String score_string;
extern sf::String health_string;
extern sf::String game_over_string;

void fonts_settings();
#endif /* GAME_FONTS */