#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <list>

#include "g_config.h"
#include "player.h"
#include "enemy.h"
#include "g_view.h"
#include "sounds.h"
#include "simple_map.h"
#include "game_fonts.h"
#include "missions.h"

#define DEBUG_OUTPUT 0

int main()
{
	char buffer[256];

	sf::Image player_image;
	player_image.loadFromFile("Sprites/hero.png");
	player_image.createMaskFromColor(sf::Color(41, 33, 59));

	sf::Image enemy_image;
	enemy_image.loadFromFile("Sprites/enemy.png");
	enemy_image.createMaskFromColor(sf::Color(255, 255, 255));

	sf::Image bullet_image;
	bullet_image.loadFromFile("Sprites/bullet.png");
	bullet_image.createMaskFromColor(sf::Color(255, 255, 255));

	//std::list<G_Character<float>*>  obj; // создаю список, сюда буду кидать объекты.например врагов.
	//std::list<G_Character<float>*>::iterator it; // итератор чтобы проходить по эл-там списка
	/* =================================================================== */
	G_Config* CONFIG = G_Config::instance();
	CONFIG->take_config();

	// «адаем размеры окна
	sf::RenderWindow window(sf::VideoMode(CONFIG->get_width(), CONFIG->get_height()), "Test",
		CONFIG->is_fullscreen() ? sf::Style::Fullscreen: sf::Style::Default);
	view.reset(sf::FloatRect(0.0, 0.0, CONFIG->get_width(), CONFIG->get_height())); // рестартим камеру и задаем стандартный размер
	
	fonts_settings(); // настраиваем текст
	sounds_settings(); // настраиваем звук
	set_mission_textbox(); // настраиваем окно задани€
	set_new_mission(CONFIG->get_default_mission());
	bool view_info(false);
	
	/* =================================================================== */
	Player<float> p(CONFIG->get_pos_x(), CONFIG->get_pos_y(), player_image);//, 6, 136, 89, 55, "hero.png");//80.0, 80.0, 96, 96, "hero.png");
	Enemy<float> enemy(CONFIG->get_pos_x()+300, CONFIG->get_pos_y(), "gun", enemy_image);
	//Bullet<float> bull(CONFIG->get_pos_x()+100, CONFIG->get_pos_y(),  bullet_image);
	/* =================================================================== */
	
	// «адаем карту
	sf::Texture map_texture; // текстура карты
	map_texture.loadFromFile("Sprites/map.png");

	sf::Sprite map_sprites; // создаЄм спрайт дл€ карты
	map_sprites.setTexture(map_texture); // заливаем текстуру спрайтом
	/* =================================================================== */
	
	sf::Clock system_clock; // создаем объект, который хранит врем€ (будет юзатьс€ дл€ прив€зки времени к "жизни" остальных объектов"
	float current_frame(0); // хранит текущий кадр
	bool is_view_map(false); // сообщает о том, что сейчас просматривают карту 
	while (window.isOpen())
	{
		/* =================================================================== */
		// «адаем скорость игры
		float game_speed = float(system_clock.getElapsedTime().asMicroseconds()); // цепл€ем прошедшее врем€ в милисекундах и делим на коснтанту, в итоге мы получаем скорость игры
		system_clock.restart(); // рестартим
		game_speed /= CONFIG->get_time_div();
		/* =================================================================== */

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
				case sf::Keyboard::Tab:
					is_view_map = !is_view_map;
					break;
				case sf::Keyboard::LShift:
					view_info = !view_info;
					break;
				case sf::Keyboard::Escape:
					//if (MessageBox(NULL,"¬ы уверенны что хотите выйти?", "¬ыход из игры", MB_YESNO) == IDYES)
						window.close();
					break;
				}
			}
		}
		/* =================================================================== */
		// ќбрабатываем событи€ клавиш
		if (p.is_alive())
		{
			if (!is_view_map) 
			{
				p.move(game_speed, current_frame, 0.1);
				//set_camera_view(p.get_x(), p.get_y()); // задаем слежку камеры за игроком
			}
			else
				view_map(game_speed); // активаци€ просмотра карты

			score_text.setString(score_string + itoa(p.get_score(), buffer, 10) + "\n" + health_string + itoa(p.get_health(), buffer, 10));
		}
		else 
		{
			view.rotate(0.01);

			score_text.setPosition(CONFIG->get_width() / 2.0 - 100, CONFIG->get_height() / 2.0 - 40);
			score_text.setString(score_string + itoa(p.get_score(), buffer, 10) + "\n" + game_over_string);
		}
		enemy.search_enemy(p);
		//view_control(game_speed); // демонстраци€ возможностей камеры
		window.setView(view); // задаем параметры камеры ƒќ очистки экрана
		window.clear();

		draw_map(window, map_sprites); // ќтрисовка карты
		if (view_info) get_mission_text(window, mission_text, CONFIG->get_width() / 2.0, CONFIG->get_height() / 2.0, CONFIG->get_default_mission());
		window.draw(p.get_sprite());
		//window.draw(bull.get_sprite());
		window.draw(enemy.get_sprite());
		enemy.update(game_speed);
		//window.draw((*(enemy.enemy_bullets.begin()))->get_sprite());
		/*
		for (it = enemy.enemy_bullets; it != enemy.enemy_bullets.end(); it++)
		{
			window.draw((*it)->get_sprite()); //рисуем объекты (сейчас это только враги)
		}/**/
		/*for (it = obj.begin(); it != obj.end(); it++)
		{
			window.draw((*it)->get_sprite()); //рисуем объекты (сейчас это только враги)
		}*/

		window.draw(score_text);
		window.display();
	}
 
	return 0;
}

/*sf::Texture hero_texture; // подкидываем текстурку
	hero_texture.loadFromFile("Sprites/hero.png"); 
	// ¬ыбираем кусок текстурки, т.е. берем тайлсет 
	sf::Sprite hero_sprite; // создаем спрайт (тайл сет)
	hero_sprite.setTexture(hero_texture);    //в р€ду   выбор р€да
	hero_sprite.setTextureRect(sf::IntRect(0*LION_POS, 1*LION_POS, 1*LION_POS, 1*LION_POS));// задаем тайлсет
	hero_sprite.setPosition(50.0,50.0); // задаем начальную позицию спрайта
	*/

/*
	sf::Image hero_image; // подкидываем пикчу
	hero_image.loadFromFile("Sprites/hero.png"); 

	sf::Texture hero_texture; // подкидываем текстурку
	hero_texture.loadFromImage(hero_image);
*/