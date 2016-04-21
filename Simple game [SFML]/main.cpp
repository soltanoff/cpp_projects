#define _CRT_SECURE_NO_WARNINGS
#include "game_interface.h"



int main()
{
	//
	Facade game;
	game.init_settings();
	game.init_entities();

	game.start_engine();
	/**/
	//sf::RenderWindow window(sf::VideoMode(1280, 800), "menu test");
	//menu(window);//вызов меню

	return 0;
}

/*sf::Texture hero_texture; // подкидываем текстурку
	hero_texture.loadFromFile("Sprites/hero.png"); 
	// Выбираем кусок текстурки, т.е. берем тайлсет 
	sf::Sprite hero_sprite; // создаем спрайт (тайл сет)
	hero_sprite.setTexture(hero_texture);    //в ряду   выбор ряда
	hero_sprite.setTextureRect(sf::IntRect(0*LION_POS, 1*LION_POS, 1*LION_POS, 1*LION_POS));// задаем тайлсет
	hero_sprite.setPosition(50.0,50.0); // задаем начальную позицию спрайта
	*/

/*
	sf::Image hero_image; // подкидываем пикчу
	hero_image.loadFromFile("Sprites/hero.png"); 

	sf::Texture hero_texture; // подкидываем текстурку
	hero_texture.loadFromImage(hero_image);
*/