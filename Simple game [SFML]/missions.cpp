#include "missions.h"



sf::Image quest_image;
sf::Texture quest_texture;
sf::Sprite mission_textbox_sprite;
//short mission_number;

void set_mission_textbox()
{
	quest_image.loadFromFile("Sprites/mission_background.png");
	quest_image.createMaskFromColor(sf::Color(255, 255, 255));
	quest_texture.loadFromImage(quest_image);
	mission_textbox_sprite.setTexture(quest_texture);
	mission_textbox_sprite.setTextureRect(sf::IntRect(0, 0, 510, 340)); 
	mission_textbox_sprite.setScale(1.0f, 0.95f);
}

sf::String get_mission_info(short mission_number)
{
	switch(mission_number)
	{
	case 0:
		return L"Вы должны собрать все камни\nдля того, чтобы построить дом.\nБудьте осторожны в лесу много\nрастений как вредных, так и нет.";
	default:
		return "";
	}
}

void get_mission_text(sf::RenderWindow &window, sf::Text &text, int x, int y, short mission_number)
{
	text.setPosition(x - mission_textbox_sprite.getTextureRect().width / 2.0 + 30, 
		y - mission_textbox_sprite.getTextureRect().height / 2.0 + 20);
	mission_textbox_sprite.setPosition(x - mission_textbox_sprite.getTextureRect().width / 2.0, 
		y - mission_textbox_sprite.getTextureRect().height / 2.0);
	
	text.setString(L"Текущее задание:\n" + get_mission_info(mission_number) + 
		L"\n\nНажмите на левый Shift\nдля отключения подсказки.");
	window.draw(mission_textbox_sprite);
	window.draw(text);
}

void set_new_mission(short mission_number)
{
	switch(mission_number)
	{
	case 0:
		create_random_items(MAP_STONE, 7);
		create_random_items(MAP_WILDFLOWER, 11);
		create_random_items(MAP_BUSH, 14);
		create_random_items(MAP_HEATLHFLOWER, 3);
		break;
	default:
		break;
	}
}