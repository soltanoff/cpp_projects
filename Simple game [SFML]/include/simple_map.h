#ifndef SIMPLE_MAP
#define SIMPLE_MAP
#include <SFML\Graphics.hpp>
#include "main_settings.h"


class Map
{
private:
	static sf::String simple_map_structure[MapCFG::MAP_HEIGHT];

	sf::Texture map_texture;	// текстура карты
	sf::Sprite map_sprites;		// создаём спрайт для карты

public:
	Map() 
	{
		map_texture.loadFromFile("Sprites/map.png");
		map_sprites.setTexture(map_texture); // заливаем текстуру спрайтом
	}

	static sf::String* get_map() { return simple_map_structure; }
	void create_random_items(const char map_item, short count);
	void draw_map(sf::RenderWindow &window);//, sf::Sprite map_sprites);

	void map_reserve(sf::String *map_copy);
	void load_map(sf::String *map_copy);
};
#endif /* SIMPLE_MAP */