#include "simple_map.h"



sf::String simple_map_structure[MAP_HEIGHT] = {
	"0000000000000000000000000000000000000000",
	"0        b        s                    0",
	"0   b          sssb ss      ss    ss   0",
	"0         f       s                    0",
	"0                       f     s        0",
	"0    f   s        f                    0",
	"0              s         h             0",
	"0                     b          s     0",
	"0    s         b          s s          0",
	"0          h       s             f     0",
	"0                     b                0",
	"0               f              s       0",
	"0     f   h            f    f      s   0",
	"0                  b      s            0",
	"0        s                b      b     0",
	"0                        s  s       s  0",
	"0                h   f                 0",
	"0         h                            0",
	"0                   s               s  0",
	"0     f             s      bf          0",
	"0                  ss s       b  s     0",
	"0             f           f            0",
	"0   f                  h               0",
	"0         s        f         f    h    0",
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