#include "game_fonts.h"



sf::Font main_font;	
sf::Font mission_font;
sf::Text score_text; // создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
sf::Text mission_text;

sf::Text records_table_name;
sf::Text records_table_score;
sf::Text user_nick;

sf::Text mn_new_game;
sf::Text mn_change_diff;
sf::Text mn_exit;
sf::Text mn_continue;
sf::Text mn_save;
sf::Text mn_load;
sf::Text mn_settings;
sf::Text mn_back_to_menu;
sf::Text mn_record_table;

sf::Text mn_key_up;
sf::Text mn_key_down;
sf::Text mn_key_left;
sf::Text mn_key_right;

sf::Text mn_key_up_value;
sf::Text mn_key_down_value;
sf::Text mn_key_left_value;
sf::Text mn_key_right_value;

sf::Text mn_accept;
sf::Text mn_caption;
sf::Text mn_text;
sf::Text mn_cancel;

sf::Text mn_setting_caption;
// const sf::String game_menu[]		=  { L"Новая игра", L"Сложность", L"Выход", L"Продолжить", L"Сохранить игру", L"Загрузить игру", L"Настройки" };

void install_text(sf::Text &text, sf::Font &font, sf::String font_name)
{
	font.loadFromFile("Font/" + font_name);
    //score_text.setColor(sf::Color::Red); 
	text.setPosition(FontsCFG::SCORE_POS[0], FontsCFG::SCORE_POS[1]);
	text.setFont(font);
	text.setString("");
	text.setCharacterSize(30);
	text.setStyle(sf::Text::Bold); // жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
}

void fonts_settings()
{
	install_text(score_text, main_font, "main_font.TTF");//times_new_roman
	install_text(mission_text, mission_font, "times.TTF");

	records_table_name.setColor(sf::Color::Black);
	install_text(records_table_name, mission_font, "times.TTF");
	
	records_table_score.setColor(sf::Color::Black);
	install_text(records_table_score, mission_font, "times.TTF");

	user_nick.setColor(sf::Color::Blue);
	install_text(user_nick, mission_font, "times.TTF");
	user_nick.setString("your_nick");

	install_text(mn_new_game, mission_font, "times.TTF");
	install_text(mn_change_diff, mission_font, "times.TTF");
	install_text(mn_exit, mission_font, "times.TTF");
	install_text(mn_continue, mission_font, "times.TTF");
	install_text(mn_save, mission_font, "times.TTF");
	install_text(mn_load, mission_font, "times.TTF");
	install_text(mn_settings, mission_font, "times.TTF");
	install_text(mn_back_to_menu, mission_font, "times.TTF");
	
	///////////////////////////////////////////////////////////////
	install_text(mn_record_table, mission_font, "times.TTF");
	mn_record_table.setString("Write: ");
	///////////////////////////////////////////////////////////////

	install_text(mn_key_up, mission_font, "times.TTF");
	install_text(mn_key_down, mission_font, "times.TTF");
	install_text(mn_key_left, mission_font, "times.TTF");
	install_text(mn_key_right, mission_font, "times.TTF");

	install_text(mn_key_up_value, mission_font, "times.TTF");
	install_text(mn_key_down_value, mission_font, "times.TTF");
	install_text(mn_key_left_value, mission_font, "times.TTF");
	install_text(mn_key_right_value, mission_font, "times.TTF");

	install_text(mn_accept, mission_font, "times.TTF");
	install_text(mn_caption, mission_font, "times.TTF");
	install_text(mn_text, mission_font, "times.TTF");
	install_text(mn_cancel, mission_font, "times.TTF");

	install_text(mn_setting_caption, mission_font, "times.TTF");
	mission_text.setColor(sf::Color::Black);

	mn_new_game.setString(FontsCFG::game_menu[0]);
	mn_change_diff.setString(FontsCFG::game_menu[1]);
	mn_exit.setString(FontsCFG::game_menu[2]);
	mn_continue.setString(FontsCFG::game_menu[3]);
	mn_save.setString(FontsCFG::game_menu[4]);
	mn_load.setString(FontsCFG::game_menu[5]);
	mn_settings.setString(FontsCFG::game_menu[6]);
	mn_back_to_menu.setString(FontsCFG::game_menu[7]);
	mn_record_table.setString(FontsCFG::game_menu[8]);

	mn_key_up.setString(FontsCFG::game_menu[9]);
	mn_key_down.setString(FontsCFG::game_menu[10]);
	mn_key_left.setString(FontsCFG::game_menu[11]);
	mn_key_right.setString(FontsCFG::game_menu[12]);
	
	mn_accept.setString(FontsCFG::game_menu[13]);
	mn_cancel.setString(FontsCFG::game_menu[14]);

	mn_setting_caption.setString(FontsCFG::game_menu[15]);
}