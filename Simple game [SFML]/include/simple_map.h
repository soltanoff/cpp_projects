#include <SFML\Graphics.hpp>

const short MAP_HEIGHT = 25; // размер карты высота
const short MAP_WIDTH = 40; // размер карты ширина 
const short MAP_TILE_SIZE = 32; // размер тайла спрайта карты
 
sf::String simple_map_structure[MAP_HEIGHT] = {
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0   s                                  0",
	"0                                      0",
	"0                             s        0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                  s                   0",
	"0                                      0",
	"0                                      0",
	"0                                      0",
	"0                         s            0",
	"0        s                s            0",
	"0                        s  s          0",
	"0                                      0",
	"0                                      0",
	"0                   s                  0",
	"0                   s                  0",
	"0                  ss s                0",
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