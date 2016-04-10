#ifndef G_GAME_INTERFACE
#define G_GAME_INTERFACE
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "player.h"
#include "enemy.h"
#include "g_config.h"
#include "g_view.h"
#include "sounds.h"
#include "simple_map.h"
#include "game_fonts.h"
#include "missions.h"

#include <list>




template <typename T>
struct g_list
{
	Enemy<T> *enemy;
	g_list<T> *next;
};

// Фасад.
template <typename T>
class Facade
{
private:
	/* ============================================================================================================================= */
	char __buffer[256];
	bool _is_init_settings;
	int _mission_number;
	/* ============================================================================================================================= */
	G_Config *_CONFIG;
	//sf::Event event;
	sf::RenderWindow *_window;
	/* ============================================================================================================================= */
	// Задаем карту
	sf::Texture __map_texture;	// текстура карты
	sf::Sprite __map_sprites;		// создаём спрайт для карты
	/* ============================================================================================================================= */
	bool __view_info;
	bool __start_game;			// двинулся ли персонаж?
	sf::Clock __system_clock;		// создаем объект, который хранит время (будет юзаться для привязки времени к "жизни" остальных объектов"
	float __current_frame;		// хранит текущий кадр
	float __game_speed;			// скорость игры
	bool __is_view_map;			// сообщает о том, что сейчас просматривают карту 
	/* ============================================================================================================================= */
	sf::Image __player_image;
	sf::Image __enemy_image;
	/* ============================================================================================================================= */
	Player<T> *_p;
	int _enemy_count;
	g_list<T> *_enemy_list;
	/* ============================================================================================================================= */
	sf::Event event_;
	/* ============================================================================================================================= */
	bool event_handler();//(sf::Event &event_)
	bool player_actions();
	/* ============================================================================================================================= */
public:
	/* ============================================================================================================================= */
	Facade()
	{
		_CONFIG = NULL;
		_window = NULL;
		_p = NULL;
//		_enemy = NULL;
//		_enemy_2 = NULL;
		_enemy_list = NULL;

		__start_game = __view_info = __is_view_map = false;
		_mission_number = 4;
		__current_frame = 0;
		__game_speed = 0;

		_is_init_settings = false;
	}
	/* ============================================================================================================================= */
	bool enemy_init(int count)
	{
		
		if (_enemy_list)
		{
			g_list<T> *temp = _enemy_list;
			while(temp)
			{
				delete temp->enemy;
				temp = temp->next;
			}
			_enemy_list = NULL;
		}

		int a = ENEMY_SPAWN::ENEMY_POS_INDEX[_mission_number - 1][0];
		int b = ENEMY_SPAWN::ENEMY_POS_INDEX[_mission_number - 1][1];

		_enemy_list = new g_list<T>();
		_enemy_list->enemy = new Enemy<T>(
			ENEMY_SPAWN::ENEMY_POS[a][0], 
			ENEMY_SPAWN::ENEMY_POS[a][1], 
			"gun",
			__enemy_image);

		_enemy_list->next = NULL;

		g_list<T> *plist = _enemy_list;
		for(int i = a + 1; i <= b; i++)
		{
			g_list<T> *temp = new g_list<T>();
			temp->enemy = new Enemy<T>(
				ENEMY_SPAWN::ENEMY_POS[i][0], 
				ENEMY_SPAWN::ENEMY_POS[i][1], 
				"gun", 
				__enemy_image);

			temp->next = NULL;
			plist->next = temp;
			plist = plist->next;
		}

		return true;
	}
	bool init_settings();
	bool init_entities();
	/* ============================================================================================================================= */
	void set_game_speed();
	bool start_engine();
	/* ============================================================================================================================= */
	~Facade()
	{
		delete _p;
//		delete _enemy;
//		delete _enemy_2;

		delete _CONFIG;
		delete _window;
	}
	/* ============================================================================================================================= */
};

template<typename T>
bool Facade<T>::event_handler()//(sf::Event &event_)
{
	while (_window->pollEvent(event_))
	{
		if (event_.type == sf::Event::Closed)
			_window->close();

		if (event_.type == sf::Event::KeyPressed)
		{
			switch(event_.key.code)
			{
			case sf::Keyboard::Tab:
				__is_view_map = !__is_view_map;
				break;
			case sf::Keyboard::LShift:
				__view_info = !__view_info;
				break;
			case sf::Keyboard::Escape:
				if (MessageBox(NULL,"Вы уверенны что хотите выйти?", "Выход из игры", MB_YESNO) == IDYES)
					_window->close();
				break;
			}
		}
	}
	return true;
}

template<typename T>
bool Facade<T>::player_actions()
{
	if (_p->is_alive())
	{
		if (!__is_view_map) 
		{
			_p->move(__game_speed, __current_frame, 0.1);
			//set_camera_view(p.get_x(), p.get_y()); // задаем слежку камеры за игроком
		}
		else
			view_map(__game_speed); // активация просмотра карты

		score_text.setString(score_string + itoa(_p->get_score(), __buffer, 10) + "\n" + health_string + itoa(_p->get_health(), __buffer, 10));
	}
	else 
	{
		view.rotate(0.01);

		score_text.setPosition(_CONFIG->get_width() / 2.0 - 100, _CONFIG->get_height() / 2.0 - 40);
		score_text.setString(score_string + itoa(_p->get_score(), __buffer, 10) + "\n" + game_over_string);
	}
	return true;
}

template<typename T>
bool Facade<T>::init_settings()
{
	if (!_is_init_settings)
	{
		try
		{
			_CONFIG = G_Config::instance();
			_CONFIG->take_config();

			__player_image.loadFromFile("Sprites/hero.png");
			__player_image.createMaskFromColor(sf::Color(41, 33, 59));

			__enemy_image.loadFromFile("Sprites/enemy.png");
			__enemy_image.createMaskFromColor(sf::Color(255, 255, 255));

			bullet_image.loadFromFile("Sprites/bullet.png");
			bullet_image.createMaskFromColor(sf::Color(255, 255, 255));
			// Задаем размеры окна
			_window = new sf::RenderWindow(sf::VideoMode(_CONFIG->get_width(), 
				_CONFIG->get_height()), 
				"Test",
				_CONFIG->is_fullscreen() ? sf::Style::Fullscreen: sf::Style::Default);
			// рестартим камеру и задаем стандартный размер
			view.reset(sf::FloatRect(0.0, 0.0, _CONFIG->get_width(), _CONFIG->get_height()));


			__map_texture.loadFromFile("Sprites/map.png");
			__map_sprites.setTexture(__map_texture); // заливаем текстуру спрайтом


			fonts_settings(); // настраиваем текст
			sounds_settings(); // настраиваем звук
			set_mission_textbox(); // настраиваем окно задания
			set_new_mission(_CONFIG->get_default_mission());
			_is_init_settings = true;
		}
		catch(...)
		{
			MessageBox(NULL,"Ошибка инициализации. Проверьте порядок инициализации настроек.", "Init error.", MB_OK);
			return false;
		}
	}
	else
	{
		MessageBox(NULL,"Настройки были установлены.", "Init error.", MB_OK);
		return false;
	}
	return true;
}

template<typename T>
bool Facade<T>::init_entities()
{
	if (_is_init_settings)
	{
		/* =================================================================== */
		_p = new Player<T>(_CONFIG->get_pos_x(), _CONFIG->get_pos_y(), __player_image);//, 6, 136, 89, 55, "hero.png");//80.0, 80.0, 96, 96, "hero.png");
		enemy_init(3);
		//_enemy = new Enemy<T>(_CONFIG->get_pos_x()+300, 40, "gun", __enemy_image);
		//_enemy_2 = new Enemy<T>(40, 100, "gun", __enemy_image);
		//Bullet<float> bull(CONFIG->get_pos_x()+100, CONFIG->get_pos_y(),  bullet_image);
		/* =================================================================== */
		return true;
	}
	else
	{
		MessageBox(NULL,"Ошибка инициализации. Проверьте порядок инициализации настроек.", "Init error.", MB_OK);
		return false;
	}
}

template<typename T>
void Facade<T>::set_game_speed()
{
	/* =================================================================== */
	// Задаем скорость игры
	__game_speed = float(__system_clock.getElapsedTime().asMicroseconds()); // цепляем прошедшее время в милисекундах и делим на коснтанту, в итоге мы получаем скорость игры
	__system_clock.restart(); // рестартим
	__game_speed /= _CONFIG->get_time_div();
	/* =================================================================== */
}

template<typename T>
bool Facade<T>::start_engine()
{
	try
	{
		if (_is_init_settings)
		{
			while (_window->isOpen())
			{
				this->set_game_speed();
				this->event_handler();
				/* =================================================================== */
				player_actions();

				//view_control(game_speed); // демонстрация возможностей камеры
				_window->setView(view); // задаем параметры камеры ДО очистки экрана
				_window->clear();

				draw_map((*_window), __map_sprites); // Отрисовка карты
				if (__view_info) 
					get_mission_text(
						(*_window), 
						mission_text, 
						_CONFIG->get_width() / 2.0, 
						_CONFIG->get_height() / 2.0, 
						_CONFIG->get_default_mission());

				_window->draw(_p->get_sprite());
				//window.draw(bull.get_sprite());

				//_enemy_list.
				g_list<T> *temp = _enemy_list;
				while(temp)
				{
					temp->enemy->search_enemy((*_p));
					_window->draw(temp->enemy->get_sprite());
					temp->enemy->enemy_action((*_window), (*_p), __game_speed);
					temp->enemy->update(__game_speed);
					temp->enemy->shot((*_window));
					temp = temp->next;
				}
				/**/

				/*_enemy->search_enemy((*_p));
				_window->draw(_enemy->get_sprite());
				_enemy->enemy_action((*_window), (*_p), __game_speed);
				//_enemy->update(__game_speed);
				//_enemy->shot((*_window));

				_enemy_2->search_enemy((*_p));
				_window->draw(_enemy_2->get_sprite());
				_enemy_2->enemy_action((*_window), (*_p), __game_speed);
				//_enemy_2->update(__game_speed);
				//_enemy_2->shot((*_window));
				*/
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
				_window->draw(score_text);
				_window->display();
			}
			return true;
		}
		else
		{
			MessageBox(NULL,"Настройки не были проинициализированы.", "Game error.", MB_OK);
			return false;
		}
	}
	catch(...)
	{
		MessageBox(NULL,"Ошибка движка. Let's debug.", "Engine error.", MB_OK);
		return false;
	}
}
#endif /* G_GAME_INTERFACE */