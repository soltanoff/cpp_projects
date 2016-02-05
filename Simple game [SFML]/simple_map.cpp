#include "simple_map.h"



sf::String simple_map_structure[MAP_HEIGHT] = {
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0000000000000000000000000000000000000000",
};
/*
const char MAP_NOTHING = ' ';
const char MAP_STONE = 's';
const char MAP_CURB = '0';
const char MAP_WILDFLOWER = 'f';
const char MAP_HEATLHFLOWER = 'h';
const char MAP_BUSH = 'b';
*/
void create_random_items(const char map_item, short count)
{
	srand(time(0));
	int rand_x = 0;
	int rand_y = 0;
	int temp_count = count;

	while(temp_count != 0)
	{
		rand_x = rand() % (MAP_HEIGHT - 1);
		rand_y = rand() % (MAP_WIDTH - 1);
		if (simple_map_structure[rand_x][rand_y] == MAP_NOTHING)
		{
			simple_map_structure[rand_x][rand_y] = map_item;
			temp_count--;
		}
	}
}

void draw_map(sf::RenderWindow &window, sf::Sprite map_sprites)
{
	for(short i = 0; i < MAP_HEIGHT; i++)
	{
		for(short j = 0; j < MAP_WIDTH; j++)
		{
			switch(simple_map_structure[i][j])
			{
			case MAP_NOTHING: // просто пол
				map_sprites.setTextureRect(sf::IntRect(0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MAP_STONE: // s - камень
				map_sprites.setTextureRect(sf::IntRect(MAP_TILE_SIZE, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MAP_CURB: // 0 - бордюр
				map_sprites.setTextureRect(sf::IntRect(2 * MAP_TILE_SIZE, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MAP_WILDFLOWER: // f - дикий цветок
				map_sprites.setTextureRect(sf::IntRect(3 * MAP_TILE_SIZE, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MAP_HEATLHFLOWER: // h - цветок жизни
				map_sprites.setTextureRect(sf::IntRect(4 * MAP_TILE_SIZE, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MAP_BUSH: // b - куст
				map_sprites.setTextureRect(sf::IntRect(5 * MAP_TILE_SIZE, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			}

			map_sprites.setPosition(j * MAP_TILE_SIZE, i * MAP_TILE_SIZE);
			window.draw(map_sprites);
		}
	}
}