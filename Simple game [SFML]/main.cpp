#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>

#include "player.h"
#include "g_view.h"
#include "sounds.h"
#include "simple_map.h"
#include "game_fonts.h"


#define DEBUG_OUTPUT 0

const short LION_POS = 96;
const short LION_H = 58;
const short LION_W = 96;

float LION_HORIZONT_STEP = 0.1;
float LION_VERTICAL_STEP = 0.1;

const long WINDOW_WIDTH = 1280;//680;
const long WINDOW_HEIGHT = 800;//480;
const long TIME_DIV = 800;

int main()
{
	char buffer[256];
	/* =================================================================== */
	// Задаем размеры окна
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Test");
	view.reset(sf::FloatRect(0.0, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT)); // рестартим камеру и задаем стандартный размер

	fonts_settings(); // настраиваем текст
	sounds_settings(); // настраиваем звук
	/* =================================================================== */

	Player p(80.0, 80.0, 96, 96, "hero.png");
	/* =================================================================== */
	// Задаем карту
	sf::Texture map_texture; // текстура карты
	map_texture.loadFromFile("Sprites/map.png");

	sf::Sprite map_sprites; // создаём спрайт для карты
	map_sprites.setTexture(map_texture); // заливаем текстуру спрайтом
	/* =================================================================== */
	sf::Clock system_clock; // создаем объект, который хранит время (будет юзаться для привязки времени к "жизни" остальных объектов"
	float current_frame(0); // хранит текущий кадр
	bool is_view_map(false); // сообщает о том, что сейчас просматривают карту 
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
			is_view_map = !is_view_map;

		if (!is_view_map) 
		{
			p.move(game_speed, current_frame, 0.1);
			set_camera_view(p.get_x(), p.get_y()); // задаем слежку камеры за игроком
		}
		else
			view_map(game_speed); // активация просмотра карты
			
		//view_control(game_speed); // демонстрация возможностей камеры
		/* =================================================================== */
		// Биндинг точки выхода
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (MessageBox(NULL,"Вы уверенны что хотите выйти?", "Выход из игры", MB_YESNO) == IDYES)
				window.close();
		}
		/* =================================================================== */
		score_text.setString(score_string + itoa(p.get_score(), buffer, 10));
		window.setView(view); // задаем параметры камеры ДО очистки экрана
		window.clear();

		draw_map(window, map_sprites); // Отрисовка карты
		window.draw(p.get_sprite());
		window.draw(score_text);
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