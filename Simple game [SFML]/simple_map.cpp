#include "simple_map.h"



sf::String Map::simple_map_structure[MapCFG::MAP_HEIGHT] = {
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                           00         0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                            0         0",
	"0              0                       0",
	"0              0                       0",
	"0              0                       0",
	"0              0                       0",
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
void Map::create_random_items(const char map_item, short count)
{
	srand((unsigned int)time(0));
	int rand_x = 0;
	int rand_y = 0;
	int temp_count = count;

	while(temp_count != 0)
	{
		rand_x = rand() % (MapCFG::MAP_HEIGHT - 1);
		rand_y = rand() % (MapCFG::MAP_WIDTH - 1);
		if (simple_map_structure[rand_x][rand_y] == MapCFG::MAP_NOTHING)
		{
			simple_map_structure[rand_x][rand_y] = map_item;
			temp_count--;
		}
	}
}

void Map::draw_map(sf::RenderWindow &window)//, sf::Sprite map_sprites)
{
	for(int i = 0; i < MapCFG::MAP_HEIGHT; i++)
	{
		for(int j = 0; j < MapCFG::MAP_WIDTH; j++)
		{
			switch(simple_map_structure[i][j])
			{
			case MapCFG::MAP_NOTHING: // просто пол
				map_sprites.setTextureRect(sf::IntRect(0, 0, MapCFG::MAP_TILE_SIZE, MapCFG::MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MapCFG::MAP_STONE: // s - камень
				map_sprites.setTextureRect(sf::IntRect(MapCFG::MAP_TILE_SIZE, 0, MapCFG::MAP_TILE_SIZE, MapCFG::MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MapCFG::MAP_CURB: // 0 - бордюр
				map_sprites.setTextureRect(sf::IntRect(2 * MapCFG::MAP_TILE_SIZE, 0, MapCFG::MAP_TILE_SIZE, MapCFG::MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MapCFG::MAP_WILDFLOWER: // f - дикий цветок
				map_sprites.setTextureRect(sf::IntRect(3 * MapCFG::MAP_TILE_SIZE, 0, MapCFG::MAP_TILE_SIZE, MapCFG::MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MapCFG::MAP_HEATLHFLOWER: // h - цветок жизни
				map_sprites.setTextureRect(sf::IntRect(4 * MapCFG::MAP_TILE_SIZE, 0, MapCFG::MAP_TILE_SIZE, MapCFG::MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case MapCFG::MAP_BUSH: // b - куст
				map_sprites.setTextureRect(sf::IntRect(5 * MapCFG::MAP_TILE_SIZE, 0, MapCFG::MAP_TILE_SIZE, MapCFG::MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			}

			map_sprites.setPosition((float)(j * MapCFG::MAP_TILE_SIZE), (float)(i * MapCFG::MAP_TILE_SIZE));
			window.draw(map_sprites);
		}
	}
}

void Map::map_reserve(sf::String *map_copy)
{
	for (int i = 0; i < MapCFG::MAP_HEIGHT; i++)
		map_copy[i] = simple_map_structure[i];
}

void Map::load_map(sf::String *map_copy)
{
	for (int i = 0; i < MapCFG::MAP_HEIGHT; i++)
		simple_map_structure[i] = map_copy[i];
}