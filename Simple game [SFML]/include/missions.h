#ifndef G_MISSIONS
#define G_MISSIONS
#include <SFML\Graphics.hpp>
#include "simple_map.h"
#include "main_settings.h"


class Mission
{
private:
	sf::Image quest_image;
	sf::Texture quest_texture;
	sf::Sprite mission_textbox_sprite;
	static short stone_count; // статик потому что юзается в Player при map_iteraction
	//extern short mission_number;
	int mission_number;
	
	void set_mission_textbox();
public:
	Mission(): mission_number(1) { set_mission_textbox(); /* настраиваем окно задания */ };

	void set_mission_number(int value) { mission_number = value; }
	
	void set_new_mission(Map *game_map);//, short mission_number);
	void mission_incr() { mission_number++; }
	static void stone_count_decr() { stone_count--; }

	sf::String get_mission_info();//short mission_number);
	int get_mission_number() { return mission_number; }
	static short get_stone_count() { return stone_count; }
	static void set_stone_count(int value) { stone_count = value; }
	void get_mission_text(sf::RenderWindow &window, sf::Text &text, int x, int y);//, short mission_number);
	void get_mission_complete_text(sf::RenderWindow &window, sf::Text &text, int x, int y);//, short mission_number);
	bool is_the_end() { return (mission_number > MissionCFG::MAX_MISSIONS_COUNT); }
};


namespace ENEMY_SPAWN
{// 1280 x 800
	const int ENEMY_POS_INDEX[][2] = {
			{ 0,	1 },
			{ 2,	5 },
			{ 6,	11 },
			{ 12,	19 }
	};
	const int ENEMY_POS[][2] = {
			{ 45,	400 },	{ 1232, 400 },	// 1 lvl left, right

			{ 45,	400 },	{ 1232,	400 },	// 2 lvl left, right
			{ 640,	45 },	{ 640,	750 },	// 2 lvl top, bot

			{ 45,	400 },	{ 1232, 400 },	// 3 lvl left, right
			{ 320,	45 },	{ 960,	45 },	// 3 lvl top
			{ 320,	750 },	{ 960,	750 },	// 3 lvl bot

			{ 45,	200 },	{ 1232, 200 },	// 4 lvl left
			{ 45,	600 },	{ 1232, 600 },  // 4 lvl right
			{ 320,	45 },	{ 960,	45 },	// 4 lvl top
			{ 320,	750 },	{ 960,	750 },	// 4 lvl bot
	};
};
#endif /* G_MISSIONS */