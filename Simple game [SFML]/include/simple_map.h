#ifndef SIMPLE_MAP
#define SIMPLE_MAP
#include <SFML\Graphics.hpp>



const int MAP_HEIGHT = 25; // размер карты высота
const int MAP_WIDTH = 40; // размер карты ширина 
const int MAP_TILE_SIZE = 32; // размер тайла спрайта карты

const char MAP_NOTHING = ' ';
const char MAP_STONE = 's';
const char MAP_CURB = '0';
const char MAP_WILDFLOWER = 'f';
const char MAP_HEATLHFLOWER = 'h';
const char MAP_BUSH = 'b';

extern sf::String simple_map_structure[MAP_HEIGHT];

void create_random_items(const char map_item, short count);
void draw_map(sf::RenderWindow &window, sf::Sprite map_sprites);
#endif /* SIMPLE_MAP */