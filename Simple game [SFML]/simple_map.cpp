#include "simple_map.h"



sf::String simple_map_structure[MAP_HEIGHT] = {
	"0000000000000000000000000000000000000000",
	"0                 s                    0",
	"0              ssss ss      ss    ss   0",
	"0                 s                    0",
	"0                             s        0",
	"0        s                             0",
	"0              s                       0",
	"0                                s     0",
	"0    s                    s s          0",
	"0                  s                   0",
	"0                                      0",
	"0                              s       0",
	"0                                  s   0",
	"0                         s            0",
	"0        s                s            0",
	"0                        s  s       s  0",
	"0                                      0",
	"0                                      0",
	"0                   s               s  0",
	"0                   s                  0",
	"0                  ss s          s     0",
	"0                                      0",
	"0                                      0",
	"0         s                            0",
	"0000000000000000000000000000000000000000",
};

void draw_map(sf::RenderWindow &window, sf::Sprite map_sprites)
{
	for(short i = 0; i < MAP_HEIGHT; i++)
	{
		for(short j = 0; j < MAP_WIDTH; j++)
		{
			switch(simple_map_structure[i][j])
			{
			case ' ':
				map_sprites.setTextureRect(sf::IntRect(0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case 's':
				map_sprites.setTextureRect(sf::IntRect(MAP_TILE_SIZE, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			case '0':
				map_sprites.setTextureRect(sf::IntRect(2 * MAP_TILE_SIZE, 0, MAP_TILE_SIZE, MAP_TILE_SIZE)); //если встретили символ пробел, то рисуем 1й квадратик
				break;
			}

			map_sprites.setPosition(j * MAP_TILE_SIZE, i * MAP_TILE_SIZE);
			window.draw(map_sprites);
		}
	}
}