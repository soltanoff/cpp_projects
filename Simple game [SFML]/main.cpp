#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>

#include "player.h"
#include "simple_map.h"

#define DEBUG_OUTPUT 0

const short LION_POS = 96;
float LION_HORIZONT_STEP = 0.1;
float LION_VERTICAL_STEP = 0.1;

const long WINDOW_WIDTH = 680;
const long WINDOW_HEIGHT = 480;
const long TIME_DIV = 800;

int main()
{
	/* =================================================================== */
	// Задаем размеры окна
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Test");
	/* =================================================================== */

	Player p(50.0, 50.0, LION_POS, LION_POS, "hero.png");
	/* =================================================================== */
	// Задаем карту
	sf::Texture map_texture; // текстура карты
	map_texture.loadFromFile("Sprites/map.png");

	sf::Sprite map_sprites; // создаём спрайт для карты
	map_sprites.setTexture(map_texture); // заливаем текстуру спрайтом
	/* =================================================================== */
	sf::Clock system_clock; // создаем объект, который хранит время (будет юзаться для привязки времени к "жизни" остальных объектов"
	float current_frame(0); // хранит текущий кадр
	while (window.isOpen())
	{
		/* =================================================================== */
		// Задаем скорость игры
		float game_speed = float(system_clock.getElapsedTime().asMicroseconds()); // цепляем прошедшее время в милисекундах и делим на коснтанту, в итоге мы получаем скорость игры
		system_clock.restart(); // рестартим
		game_speed /= TIME_DIV;
		/* =================================================================== */
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		/* =================================================================== */
		// Обрабатываем события клавиш
		p.move(game_speed, current_frame, 0.1);
		/* =================================================================== */
		// Биндинг точки выхода
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (MessageBox(NULL,"Вы уверенны что хотите выйти?", "Выход из игры", MB_YESNO) == IDYES)
				window.close();
		}
		/* =================================================================== */

		window.clear();
		draw_map(window, map_sprites); // Отрисовка карты
		window.draw(p.get_sprite());
		window.display();
	}
 
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