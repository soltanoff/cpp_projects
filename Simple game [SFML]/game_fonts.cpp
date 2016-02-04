#include "game_fonts.h"



sf::Font main_font;	
sf::Font mission_font;
sf::Text score_text; // создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
sf::Text mission_text;

sf::String score_string = L"Ваши очки: ";
sf::String health_string = L"Ваше здоровье: ";
sf::String game_over_string = L"GAME OVER!\nНажмите ESC.";

void install_text(sf::Text &text, sf::Font &font, sf::String font_name)
{
	font.loadFromFile("Font/" + font_name);
    //score_text.setColor(sf::Color::Red); 
	text.setPosition(10.0f, 0.0f);
	text.setFont(font);
	text.setString("");
	text.setCharacterSize(30);
	text.setStyle(sf::Text::Bold); // жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
}

void fonts_settings()
{
	install_text(score_text, main_font, "main_font.TTF");//times_new_roman
	install_text(mission_text, mission_font, "times.TTF");
}