#include "missions.h"


/*
sf::Image quest_image;
sf::Texture quest_texture;
sf::Sprite mission_textbox_sprite;*/
short Mission::stone_count;
//short mission_number;

void Mission::set_mission_textbox()
{
	quest_image.loadFromFile("Sprites/mission_background.png");
	quest_image.createMaskFromColor(sf::Color(255, 255, 255));
	quest_texture.loadFromImage(quest_image);
	mission_textbox_sprite.setTexture(quest_texture);
	mission_textbox_sprite.setTextureRect(sf::IntRect(0, 0, 510, 340)); 
	mission_textbox_sprite.setScale(1.0f, 0.95f);
}

sf::String Mission::get_mission_info()//short mission_number)
{
	switch(mission_number)
	{
	case 1:
		return L"Уровень №1:\nВы должны собрать 7 камней\nдля того, чтобы построить дом.\nБудьте осторожны в лесу много\nрастений как вредных, так и нет.";
	case 2:
		return L"Уровень №2:\nВы должны собрать 10 камней\nдля того, чтобы построить дом.\nБудьте осторожны в лесу много\nрастений как вредных, так и нет.";
	case 3:
		return L"Уровень №3:\nВы должны собрать 14 камней\nдля того, чтобы построить дом.\nБудьте осторожны в лесу много\nрастений как вредных, так и нет.";
	default:
		return "";
	}
}

void Mission::get_mission_text(sf::RenderWindow &window, sf::Text &text, int x, int y)//, short mission_number)
{
	text.setPosition(
		(float)(x - mission_textbox_sprite.getTextureRect().width / 2.0 + 30), 
		(float)(y - mission_textbox_sprite.getTextureRect().height / 2.0 + 20)
		);
	mission_textbox_sprite.setPosition(
		(float)(x - mission_textbox_sprite.getTextureRect().width / 2.0), 
		(float)(y - mission_textbox_sprite.getTextureRect().height / 2.0)
		);
	mission_textbox_sprite.setScale(1.0, 1.0);
	text.setString(//L"Текущее задание: " + 
		get_mission_info() + //mission_number) + 
		L"\n\nНажмите на левый Shift\nдля отключения подсказки.");
	window.draw(mission_textbox_sprite);
	window.draw(text);
}

void Mission::get_mission_complete_text(sf::RenderWindow &window, sf::Text &text, int x, int y)//, short mission_number)
{
	sf::Vector2f pos;

	if (mission_number < MissionCFG::MAX_MISSIONS_COUNT) 
	{
		pos.x = (float)(x - mission_textbox_sprite.getTextureRect().width / 2.0 - 50);
		pos.y = (float)(y - mission_textbox_sprite.getTextureRect().height / 2.0 + 80);
		text.setPosition(pos.x + 30, pos.y + 20);
		mission_textbox_sprite.setPosition(pos);
		mission_textbox_sprite.setScale((float)1.2, (float)0.54);
		text.setString(L"Поздравляю!\n\nВы собрали нужное количество камней!\nВы переходите на следующий уровень.");
	}
	else
	{
		pos.x = (float)(x - mission_textbox_sprite.getTextureRect().width / 2.0 + 80);
		pos.y = (float)(y - mission_textbox_sprite.getTextureRect().height / 2.0 + 80);
		text.setPosition(pos.x + 30, pos.y + 20);
		mission_textbox_sprite.setPosition(pos);
		mission_textbox_sprite.setScale((float)0.6, (float)0.54);
		text.setString(L"Поздравляю!\n\nВы прошли игру!\n");
	}
	window.draw(mission_textbox_sprite);
	window.draw(text);
}

void Mission::set_new_mission(Map *game_map)//, short mission_number)
{
	switch(mission_number)
	{
	case 1: 
		stone_count = 7;
		game_map->create_random_items(MapCFG::MAP_STONE, 7);
		game_map->create_random_items(MapCFG::MAP_WILDFLOWER, 11);
		game_map->create_random_items(MapCFG::MAP_BUSH, 14);
		game_map->create_random_items(MapCFG::MAP_HEATLHFLOWER, 3);
		break;
	case 2:
		stone_count = 10;
		game_map->create_random_items(MapCFG::MAP_STONE, 12);
		game_map->create_random_items(MapCFG::MAP_WILDFLOWER, 14);
		game_map->create_random_items(MapCFG::MAP_BUSH, 14);
		game_map->create_random_items(MapCFG::MAP_HEATLHFLOWER, 3);
		break;
	case 3:
		stone_count = 14;
		game_map->create_random_items(MapCFG::MAP_STONE, 14);
		game_map->create_random_items(MapCFG::MAP_WILDFLOWER, 15);
		game_map->create_random_items(MapCFG::MAP_BUSH, 10);
		game_map->create_random_items(MapCFG::MAP_HEATLHFLOWER, 2);
		break;
	default:
		break;
	}
}