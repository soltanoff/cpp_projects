#ifndef GAME_FONTS
#define GAME_FONTS
#include <SFML\Graphics.hpp>
#include "main_settings.h"


extern sf::Font main_font;
extern sf::Font mission_font;
extern sf::Text score_text;
extern sf::Text mission_text;;

extern sf::Text records_table_name;
extern sf::Text records_table_score;
extern sf::Text user_nick;

extern sf::Text mn_new_game;
extern sf::Text mn_continue;
extern sf::Text mn_save;
extern sf::Text mn_load;
extern sf::Text mn_settings;
extern sf::Text mn_change_diff;
extern sf::Text mn_exit;
extern sf::Text mn_back_to_menu;
extern sf::Text mn_record_table;

extern sf::Text mn_key_up;
extern sf::Text mn_key_down;
extern sf::Text mn_key_left;
extern sf::Text mn_key_right;

extern sf::Text mn_key_up_value;
extern sf::Text mn_key_down_value;
extern sf::Text mn_key_left_value;
extern sf::Text mn_key_right_value;

extern sf::Text mn_accept;
extern sf::Text mn_caption;
extern sf::Text mn_text;
extern sf::Text mn_cancel;

extern sf::Text mn_setting_caption;

void fonts_settings();
void install_text(sf::Text &text, sf::Font &font, sf::String font_name);
#endif /* GAME_FONTS */