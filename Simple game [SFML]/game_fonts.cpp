#include "game_fonts.h"



sf::Font main_font;	
sf::Text score_text; // ������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)

sf::String score_string = ":3 Score: ";

void fonts_settings()
{
	main_font.loadFromFile("Font/main_font.TTF");
    //score_text.setColor(sf::Color::Red); 
	score_text.setPosition(10.0f, 0.0f);
	score_text.setFont(main_font);
	score_text.setString("");
	score_text.setCharacterSize(30);
	score_text.setStyle(sf::Text::Bold); // ������ � ������������ �����. �� ��������� �� "�����":)) � �� ������������
}