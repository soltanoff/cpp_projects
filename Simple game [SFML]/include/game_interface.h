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
	int _game_difficulty;
	bool _next_mission;
	bool _go_to_menu;
	bool _exit_program;
	bool _restart;
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
		_mission_number = 1;
		_game_difficulty = 0;
		__current_frame = 0;
		__game_speed = 0;

		_restart = false;
		_go_to_menu = false;
		_next_mission = false;
		_exit_program = false;
		_is_init_settings = false;
	}
	/* ============================================================================================================================= */
	bool enemy_init()
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

		int a = ENEMY_SPAWN::ENEMY_POS_INDEX[_game_difficulty][0]; // _mission_number - 1
		int b = ENEMY_SPAWN::ENEMY_POS_INDEX[_game_difficulty][1];

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
	bool menu(sf::RenderWindow &window) ;
	void map_reserve(sf::String *map_copy);
	void load_map(sf::String *map_copy);
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
				if (MessageBox(NULL, "Вы уверенны что хотите выйти?", "Выход из игры", MB_YESNO) == IDYES)
					_window->close();
				_exit_program = true;
				break;
			case sf::Keyboard::R:
				if (MessageBox(NULL, "Вы уверенны что перезапустить уровень?", "Перезапуск уровня", MB_YESNO) == IDYES)
					_restart = true;
				break;
			}
			if (_next_mission && event_.key.code == sf::Keyboard::Space)
			{
				_next_mission = false;
				
				_mission_number++;
				if (_mission_number > MAX_MISSIONS_COUNT)
					_go_to_menu = true;
				else
					set_new_mission(_mission_number);
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

		score_text.setString(score_string + itoa(_p->get_score(), __buffer, 10) 
			+ "\t\t\t" + left_collect + itoa(stone_count, __buffer, 10) +"\n" + 
			health_string + itoa(_p->get_health(), __buffer, 10));
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
			_mission_number = _CONFIG->get_default_mission();
			//set_new_mission(_mission_number);
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
		//enemy_init(3);
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
bool Facade<T>::menu(sf::RenderWindow &window) 
{
	sf::Texture menuBackground, menuText;
	menuBackground.loadFromFile("Sprites/test.png");

	sf::Image menuTxt_img;
	menuTxt_img.loadFromFile("Sprites/mission_background1.png");
	menuTxt_img.createMaskFromColor(sf::Color(255, 255, 255));
	menuText.loadFromImage(menuTxt_img);

	sf::Sprite menuBg(menuBackground), menuTxt(menuText);
	
	//sf::Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	
	bool isMenu = 1;
	int menuNum = 0;
	mn_new_game.setPosition(100, 30);
	mn_change_diff.setPosition(100, 90);
	mn_exit.setPosition(100, 150);

	menuBg.setPosition(80, 140);
	menuTxt.setPosition(80, 20);
	menuTxt.scale(0.9, 0.52);
	//menuBg.setOrigin(1280 / 2.0, (630 + 130)/2.0);
	//////////////////////////////МЕНЮ///////////////////
//	mn_difficult.setColor(sf::Color::White);
//	mn_difficult.setPosition(_CONFIG->get_width(), 20);

	while (isMenu)
	{
		//mn_difficult.setString(L"Уровень сложности: " + game_difficult[_game_difficulty]);
		mn_change_diff.setString(L"Уровень сложности: " + game_difficult[_game_difficulty]);

		mn_new_game.setColor(sf::Color::White);
		mn_change_diff.setColor(sf::Color::White);
		mn_exit.setColor(sf::Color::White);
		menuNum = 0;
		window.clear(sf::Color(129, 181, 221));
 
		if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_new_game.setColor(sf::Color::Blue); menuNum = 1; }
		if (sf::IntRect(100, 90, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_change_diff.setColor(sf::Color::Blue); menuNum = 2; }
		if (sf::IntRect(100, 150, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_exit.setColor(sf::Color::Blue); menuNum = 3; }

		while (window.pollEvent(event_))
		{
			if (event_.type == sf::Event::Closed)
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (menuNum == 1)	isMenu = false;//если нажали первую кнопку, то выходим из меню 
				if (menuNum == 2)	
				{ /*window.draw(about); window.display(); while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));*/ 
					if (_game_difficulty == MAX_GAME_DIFFICULT - 1) _game_difficulty = 0;
					else _game_difficulty++;
				}
				if (menuNum == 3)	{ window.close(); isMenu = false; _exit_program = true; }
			}
		}
 
		menuBg.rotate(0.01);

		window.draw(menuBg);
		window.draw(menuTxt);
		window.draw(mn_new_game);
		window.draw(mn_change_diff);
		window.draw(mn_exit);
		
		window.display();
	}
	return true;
	////////////////////////////////////////////////////
}

template<typename T>
void Facade<T>::map_reserve(sf::String *map_copy)
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		map_copy[i] = simple_map_structure[i];
}

template<typename T>
void Facade<T>::load_map(sf::String *map_copy)
{
	for (int i = 0; i < MAP_HEIGHT; i++)
		simple_map_structure[i] = map_copy[i];
}

/*
ДОБАВИТЬ НОВУЮ СТИЛИСТИКУ
Сделать рефакторинг функции
Рейсталинг меню
*/

template<typename T>
bool Facade<T>::start_engine()
{
	try
	{
		if (_is_init_settings)
		{
			sf::String copy_map[MAP_HEIGHT];
			int health(FULL_HEALTH);
			int score(0);

			while (!_exit_program)
			{
				if (!_restart)
				{
					menu(*_window);
					enemy_init();

					__view_info = true;

					map_reserve(copy_map);
				}
				else
				{
					_p->set_score(score);
					_p->set_health(health);
					load_map(copy_map);
					_restart = false;
				}

				set_new_mission(_mission_number);
				while (_window->isOpen())
				{
					/* =================================================================== */
					this->set_game_speed();
					this->event_handler();
					/* =================================================================== */
					//view_control(game_speed); // демонстрация возможностей камеры
					_window->setView(view); // задаем параметры камеры ДО очистки экрана
					_window->clear();

					draw_map((*_window), __map_sprites); // Отрисовка карты
					//window.draw(bull.get_sprite());
					/* =================================================================== */
					/* =================================================================== */
					g_list<T> *temp = _enemy_list;
				
					while(temp)
					{
						temp->enemy->enemy_action((*_window), (*_p), __game_speed, true);//__view_info);
						//if (!__view_info) temp->enemy->shot((*_window));
						temp = temp->next;
					}
					/* =================================================================== */
					if (!__view_info) player_actions();
					_window->draw(_p->get_sprite());
					/* =================================================================== */
					if (__view_info)
					{
						if (_next_mission)
							get_mission_complete_text(
								(*_window), 
								mission_text, 
								_CONFIG->get_width() / 2.0, 
								_CONFIG->get_height() / 2.0, 
								this->_mission_number);
						else
							get_mission_text(
								(*_window), 
								mission_text, 
								_CONFIG->get_width() / 2.0, 
								_CONFIG->get_height() / 2.0, 
								this->_mission_number);
					}

					if (stone_count == 0 && !_next_mission)
					{
						_next_mission = true;
						__view_info = true;

						map_reserve(copy_map);
						score = _p->get_score();
						health = _p->get_health();
					}

					if (_go_to_menu)
					{
						_go_to_menu = false;
						break;
					}

					if (_restart)
						break;
					
					_window->draw(score_text);
					_window->display();
				}
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