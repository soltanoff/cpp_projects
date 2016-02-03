#ifndef SIMPLE_MAP
#define SIMPLE_MAP
#include <SFML\Graphics.hpp>



const short MAP_HEIGHT = 25; // размер карты высота
const short MAP_WIDTH = 40; // размер карты ширина 
const short MAP_TILE_SIZE = 32; // размер тайла спрайта карты
 
extern sf::String simple_map_structure[MAP_HEIGHT];

void draw_map(sf::RenderWindow &window, sf::Sprite map_sprites);
#endif /* SIMPLE_MAP */