#ifndef MAIN_SETTINGS
#define MAIN_SETTINGS
#include <SFML\Graphics.hpp>
#include "textures_settings.h"

#define FULL_HEALTH 50

const char SETTINGS_FILE[]			= "settings.ini";
const char SAVE_FILE[]				= "Save/save_game.sav";
const short CAMERA_VIEW_X_OFFSET	= 70;
const char RECORDS_FILE[]			= "rec.db";
const int MAX_RECORDS_COUNT			= 7;
const int CHAR_BUFER				= 1024; 

namespace EnemyCFG
{
	const int GUN_CRITICAL_DISTANCE = 300;
	const int GUN_MINIMAL_DISTANCE	= 60;

	const int BULLET_SPEED			= 320;
	const int BULLET_DAMAGE			= 1;
}

namespace PlayerCFG
{
	const int STONE_SCOREINCR		= 10;
	const int WILDFLOWER_SCOREDECR	= 20;
	const int WILDFLOWER_DAMAGE		= 20;
	const int HEATLHFLOWER_HEAL		= 10;
}

namespace FontsCFG
{
	const float SCORE_POS[]				= { 10, 0 };

	const sf::String game_difficult[]	= { L"Легко", L"Средне", L"Сложно", L"Pro" };
	const sf::String game_menu[]		= { 
		L"Новая игра", 
		L"Сложность", 
		L"Выйти из игры", 
		L"Продолжить", 
		L"Сохранить игру", 
		L"Загрузить игру", 
		L"Настройки", 
		L"Выйти в меню",
		L"Таблица рекордов",

		L"Вверх: ",
		L"Вниз: ",
		L"Влево: ",
		L"Вправо: ",
		L"ОК",
		L"Дефолт", 
		L"Управление"
	};

	const sf::String left_collect		= L"Осталось собрать: ";
	const sf::String score_string		= L"Ваши очки: ";
	const sf::String health_string		= L"Ваше здоровье: ";
	const sf::String game_over_string	= L"GAME OVER!\nНажмите ESC.";
}

namespace MissionCFG
{
	//const short DEFAULT_MISSION	= 0;
	const int MAX_GAME_DIFFICULT	= 4;
	const short MAX_MISSIONS_COUNT	= 3;
}

namespace MapCFG
{
	const int MAP_HEIGHT			= 25; // размер карты высота
	const int MAP_WIDTH				= 40; // размер карты ширина 
	const int MAP_TILE_SIZE			= 32; // размер тайла спрайта карты

	const char MAP_NOTHING			= ' ';
	const char MAP_STONE			= 's';
	const char MAP_CURB				= '0';
	const char MAP_WILDFLOWER		= 'f';
	const char MAP_HEATLHFLOWER		= 'h';
	const char MAP_BUSH				= 'b';
}

struct rec_table
{
	std::string name;
	int score;
};
#endif /* MAIN_SETTINGS */