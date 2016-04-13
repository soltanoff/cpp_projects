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

extern sf::String game_menu[3];
extern sf::String game_difficult[4];

extern sf::Text mn_new_game;
extern sf::Text mn_change_diff;
extern sf::Text mn_exit;
//extern sf::Text mn_difficult;
void fonts_settings();
#endif /* GAME_FONTS */