#ifndef G_MISSIONS
#define G_MISSIONS
#include <SFML\Graphics.hpp>
#include "simple_map.h"

//const short DEFAULT_MISSION = 0;

extern sf::Texture quest_texture;
extern sf::Sprite mission_textbox_sprite;
//extern short mission_number;

void set_mission_textbox();
void set_new_mission(short mission_number);
void get_mission_text(sf::RenderWindow &window, sf::Text &text, int x, int y, short mission_number);

const int MAX_GAME_DIFFICULT = 4;

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