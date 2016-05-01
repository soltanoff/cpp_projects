﻿#ifndef G_GAME_INTERFACE
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
#include "entity_observer.h"
#include <fstream>
#include <string>


// Фасад.
class Facade
{
private:
	/* ============================================================================================================================= */
	char __buffer[256];
	bool _is_init_settings;
	int _game_difficulty;
	bool _next_mission;
	bool _go_to_menu;
	bool _go_to_mini_menu;
	bool _go_to_settings;
	bool _exit_program;
	bool _isMessageBox;
	bool _restart;
	bool _load;
	/* ============================================================================================================================= */
	G_Config *_CONFIG;
	Map game_map;
	Mission game_mission;
	sf::RenderWindow *_window;
	/* ============================================================================================================================= */
	bool __view_info;
	bool __start_game;			// двинулся ли персонаж?
	sf::Clock __system_clock;	// создаем объект, который хранит время (будет юзаться для привязки времени к "жизни" остальных объектов"
	float __current_frame;		// хранит текущий кадр
	float __game_speed;			// скорость игры
	bool __is_view_map;			// сообщает о том, что сейчас просматривают карту 
	/* ============================================================================================================================= */
	sf::Image __player_image;
	sf::Image __enemy_image;
	/* ============================================================================================================================= */
	Player *_p;
	G_Subject _entity_list;
	/* ============================================================================================================================= */
	sf::Event event_;
	/* ============================================================================================================================= */
	bool event_handler();
	bool rotate_screen();
	/* ============================================================================================================================= */
	void set_game_speed();
	bool init_enemy();
	/* ============================================================================================================================= */
	sf::String temp[4];
	sf::Keyboard::Key up, down, left, right;
	/* ============================================================================================================================= */
	bool settings_menu(sf::RenderWindow &window);
	sf::Keyboard::Key Facade::write(sf::String &key, sf::RenderWindow &window);
	struct gs_struct
	{
		float x;
		float y;
		int health;
		int score;
		int mission_number;
		int difficult;
		short stone_count;
		char map[MapCFG::MAP_HEIGHT][MapCFG::MAP_WIDTH + 2];
		char copy_map[MapCFG::MAP_HEIGHT][MapCFG::MAP_WIDTH + 2];
		int rr_score;
		int rr_health;
	} save_params;
	struct rr_struct
	{
		sf::String copy_map[MapCFG::MAP_HEIGHT];
		int health;
		int score;
		sf::String default_map[MapCFG::MAP_HEIGHT];
	} rr_params;
	/* ============================================================================================================================= */
	bool Facade::save_game();
	bool Facade::load_game();
	/* ============================================================================================================================= */
	bool Facade::load_procedure();
	bool Facade::restart_procedure();
	bool Facade::begining_procedure();
	bool Facade::main_procedure();
	/* ============================================================================================================================= */
public:
	/* ============================================================================================================================= */
	Facade();
	~Facade();
	/* ============================================================================================================================= */
	bool init_settings();
	bool init_entities();
	/* ============================================================================================================================= */
	bool start_engine();
	bool menu(sf::RenderWindow &window);
	bool mini_menu(sf::RenderWindow &window);
	/* ============================================================================================================================= */
	bool write_line(sf::Text &text, sf::Event event_);//sf::RenderWindow &window);
	//bool draw_message(sf::RenderWindow &window);
	bool draw_message(sf::RenderWindow &window, sf::String caption, sf::String main_text, bool &result);

	sf::Texture MSBX_text;
	sf::Image MSBX_img;
	sf::Sprite MSBX_sprite;

	sf::Texture menuBackground, menuText;
	sf::Image menuTxt_img;
	sf::Sprite menuBg, menuTxt;
	sf::View view2;
};

Facade::Facade()
{
	menuBackground.loadFromFile("Sprites/test.png");

	menuTxt_img.loadFromFile("Sprites/mission_background.png");
	menuTxt_img.createMaskFromColor(sf::Color(255, 255, 255));
	menuText.loadFromImage(menuTxt_img);

	menuBg.setTexture(menuBackground);
	menuTxt.setTexture(menuText);

	MSBX_img.loadFromFile("Sprites/mission_background.png");
	MSBX_img.createMaskFromColor(sf::Color(255, 255, 255));
	MSBX_text.loadFromImage(MSBX_img);

	MSBX_sprite.setTexture(MSBX_text);

	_CONFIG = NULL;
	_window = NULL;
	_p = NULL;
//		_enemy = NULL;
//		_enemy_2 = NULL;
	//_enemy_list = NULL;

	__start_game = __view_info = __is_view_map = false;
	//_mission_number = 1;
	_game_difficulty = 0;
	__current_frame = 0;
	__game_speed = 0;

	_isMessageBox = false;
	_load = false;
	_restart = false;
	_go_to_menu = false;
	_go_to_mini_menu = false;
	_go_to_settings = false;
	_next_mission = false;
	_exit_program = false;
	_is_init_settings = false;
}

Facade::~Facade()
{
	delete _p;
//	delete _enemy;
//	delete _enemy_2;
	_entity_list.clear();

	delete _CONFIG;
	delete _window;
}

bool Facade::event_handler()//(sf::Event &event_)
{
	while (_window->pollEvent(event_))
	{
		if (event_.type == sf::Event::Closed)
			_window->close();
		
		if (event_.type == sf::Event::Resized)
		{
			printf("resized\n");
			view.setSize(event_.size.width, event_.size.height);
			view.setCenter(event_.size.width / 2, event_.size.height / 2);
			view2.setSize(event_.size.width, event_.size.height);
			view2.setCenter(event_.size.width / 2, event_.size.height / 2);
			_window->setView(view);
		}/* */

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
				/*
				if (MessageBox(NULL, "Вы уверенны что хотите выйти?", "Выход из игры", MB_YESNO) == IDYES)
					_window->close();
				_exit_program = true;
				*/
				//mini_menu(*_window);
				_go_to_mini_menu = true;
				break;
			case sf::Keyboard::R:
				if (MessageBox(NULL, "Вы уверенны что перезапустить уровень?", "Перезапуск уровня", MB_YESNO) == IDYES)
					_restart = true;
				break;
			}
			if (_next_mission && event_.key.code == sf::Keyboard::Space)
			{
				_next_mission = false;

				//_mission_number++;
				game_mission.mission_incr();
				if (game_mission.get_mission_number() > MissionCFG::MAX_MISSIONS_COUNT)
					_go_to_menu = true;
				else
				{
					_p->set_x(_CONFIG->get_pos_x());
					_p->set_y(_CONFIG->get_pos_y());
					game_mission.set_new_mission(&game_map);//, _mission_number);
				}
			}
		}
	}
	return true;
}

bool Facade::rotate_screen()
{
	if (_p->is_alive())
	{
		/*
		if (!__is_view_map) 
		{
			//_p->move(__game_speed, __current_frame, (float)0.1);
			//set_camera_view(p.get_x(), p.get_y()); // задаем слежку камеры за игроком
		}
		else
			view_map(__game_speed); // активация просмотра карты
		*/
		score_text.setPosition(view.getCenter().x - view.getSize().x / 2, view.getCenter().y -view.getSize().y / 2);

		score_text.setString(FontsCFG::score_string + _itoa(_p->get_score(), __buffer, 10) 
			+ "\t\t\t" + FontsCFG::left_collect + _itoa(Mission::get_stone_count(), __buffer, 10) +"\n" + 
			FontsCFG::health_string + _itoa(_p->get_health(), __buffer, 10));
	}
	else 
	{
		view.rotate((float)0.01);

		score_text.setPosition((float)(_CONFIG->get_width() / 2.0 - 100), (float)(_CONFIG->get_height() / 2.0 - 40));
		score_text.setString(FontsCFG::score_string + _itoa(_p->get_score(), __buffer, 10) + "\n" + FontsCFG::game_over_string);
	}
	return true;
}

bool Facade::init_enemy()
	{
		if (!_entity_list.empty())
			_entity_list.clear();

		int a = ENEMY_SPAWN::ENEMY_POS_INDEX[_game_difficulty][0]; // _mission_number - 1
		int b = ENEMY_SPAWN::ENEMY_POS_INDEX[_game_difficulty][1];

		for(int i = a; i <= b; i++)
			_entity_list.attach(new Enemy(
				(float)ENEMY_SPAWN::ENEMY_POS[i][0], 
				(float)ENEMY_SPAWN::ENEMY_POS[i][1], 
				"gun", 
				__enemy_image));

		_entity_list.attach(_p);

		return true;
	}

bool Facade::init_settings()
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
			_window = new sf::RenderWindow(
				sf::VideoMode(_CONFIG->get_width(), _CONFIG->get_height()), 
				//sf::VideoMode(800, 600), 
				"Test",
				sf::Style::Default);
				//sf::Style::Close);
				//_CONFIG->is_fullscreen() ? sf::Style::Fullscreen: sf::Style::None);
			// рестартим камеру и задаем стандартный размер
			view.reset(sf::FloatRect(0.0, 0.0, (float)_CONFIG->get_width(), (float)_CONFIG->get_height()));

			/*
			__map_texture.loadFromFile("Sprites/map.png");
			__map_sprites.setTexture(__map_texture); // заливаем текстуру спрайтом
			*/
			

			fonts_settings(); // настраиваем текст
			//sounds_settings(); // настраиваем звук
			//game_mission.set_mission_textbox(); // настраиваем окно задания
			game_mission.set_mission_number(_CONFIG->get_default_mission());//_mission_number = _CONFIG->get_default_mission();
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

bool Facade::init_entities()
{
	if (_is_init_settings)
	{
		/* =================================================================== */
		_p = new Player(_CONFIG->get_pos_x(), _CONFIG->get_pos_y(), __player_image);//, 6, 136, 89, 55, "hero.png");//80.0, 80.0, 96, 96, "hero.png");
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

void Facade::set_game_speed()
{
	/* =================================================================== */
	// Задаем скорость игры
	__game_speed = float(__system_clock.getElapsedTime().asMicroseconds()); // цепляем прошедшее время в милисекундах и делим на коснтанту, в итоге мы получаем скорость игры
	__system_clock.restart(); // рестартим
	__game_speed /= _CONFIG->get_time_div();
	/* =================================================================== */
}

sf::Keyboard::Key Facade::write(sf::String &key, sf::RenderWindow &window)
{
	bool isMenu = true;
	while (isMenu)
	{
		while (window.pollEvent(event_))
		{
			if (event_.type == sf::Event::Closed)
				window.close();

			if (event_.type == sf::Event::KeyPressed)
			{
				if (event_.key.code >= sf::Keyboard::A && event_.key.code <= sf::Keyboard::Z)
				{
					key = (char)(event_.text.unicode + 65);
					return event_.key.code;
				}
				if (event_.key.code == sf::Keyboard::Up)
				{
					key = "Up";
					return event_.key.code;
				}
				if (event_.key.code == sf::Keyboard::Down)
				{
					key = "Down";
					return event_.key.code;
				}
				if (event_.key.code == sf::Keyboard::Left)
				{
					key = "Left";
					return event_.key.code;
				}
				if (event_.key.code == sf::Keyboard::Right)
				{
					key = "Right";
					return event_.key.code;
				}
			}
		}
	}
	return sf::Keyboard::F1;
}

bool Facade::write_line(sf::Text &text, sf::Event event_)
{
	if (event_.type == sf::Event::KeyPressed)
	{
		/* ============================================================================================== */
		if (event_.key.code >= sf::Keyboard::A && event_.key.code <= sf::Keyboard::Z)
		{
			if (event_.key.shift)
				text.setString(text.getString() + (char)(event_.text.unicode + 65));
			else
				text.setString(text.getString() + (char)(event_.text.unicode + 97));
		}
		if (event_.key.code >= sf::Keyboard::Num0 && event_.key.code <= sf::Keyboard::Num9)
		{
			text.setString(text.getString() + (char)(event_.text.unicode + 22));
		}
		if (event_.key.code >= sf::Keyboard::Numpad0 && event_.key.code <= sf::Keyboard::Numpad9)
		{
			text.setString(text.getString() + (char)(event_.text.unicode - 27));
		}
		/* ============================================================================================== */
		if (event_.key.code == sf::Keyboard::Return)
		{
			if (text.getString().getSize() > 0)
				return true;
		}
		if (event_.key.code == sf::Keyboard::BackSpace)
		{
			if (text.getString().getSize() > 0)
				text.setString(text.getString().substring(0, text.getString().getSize() - 1));
		}
		/* ============================================================================================== */
	}
	return false;
}

bool Facade::save_game()
{
	std::ofstream fout(SAVE_FILE, std::ios_base::out);

	if (fout)
	{
		fout 
			<< _p->get_x()							<< " "
			<< _p->get_y()							<< "\n"
			<< _p->get_score()						<< "\n"
			<< _p->get_health()						<< "\n"
			<< game_mission.get_mission_number()	<< "\n"
			<< _game_difficulty						<< "\n"
			<< game_mission.get_stone_count()		<< "\n"
			<< rr_params.health						<< "\n"
			<< rr_params.score						<< "\n"
			;
		for(int i = 0; i < MapCFG::MAP_HEIGHT; i++)
			fout << game_map.get_map()[i].toAnsiString() << "\n";

		for(int i = 0; i < MapCFG::MAP_HEIGHT; i++)
			fout << rr_params.copy_map[i].toAnsiString() << "\n";

		MessageBox(
			NULL, 
			"Игра успешно сохранена!", 
			"Сохранение игры", 
			MB_OK);
		return true;
	}
	else
	{
		MessageBox(
			NULL, 
			"Проблемы с сохранением. Игра не сохранена!", 
			"Ошибка сохранения игры", 
			MB_OK);
		return false;
	}
	fout.close();
}

bool Facade::load_game()
{
	std::ifstream fin(SAVE_FILE, std::ios_base::in);
	if (fin)
	{
		try
		{
			fin
				>> save_params.x
				>> save_params.y
				>> save_params.score
				>> save_params.health
				>> save_params.mission_number
				>> save_params.difficult
				>> save_params.stone_count
				>> save_params.rr_health
				>> save_params.rr_score
				;
			fin.getline(save_params.map[0], MapCFG::MAP_WIDTH + 2);
			for(int i = 0; i < MapCFG::MAP_HEIGHT; i++)
				fin.getline(save_params.map[i], MapCFG::MAP_WIDTH + 2);
			for(int i = 0; i < MapCFG::MAP_HEIGHT; i++)
				fin.getline(save_params.copy_map[i], MapCFG::MAP_WIDTH + 2);

			MessageBox(
				NULL, 
				"Игра успешно загружена!", 
				"Загрузка игры", 
				MB_OK);
			return true;
		}
		catch(...)
		{
			MessageBox(
				NULL, 
				"При загрузке произошли ошибки. Игра не загружена!", 
				"Ошибка загрузки игры", 
				MB_OK);
			return false;
		}
	}
	else
	{
		MessageBox(
			NULL, 
			"Файл сохранения не найден. Игра не загружена!", 
			"Ошибка загрузки игры", 
			MB_OK);
		return false;
	}
	fin.close();
}

bool Facade::mini_menu(sf::RenderWindow &window) 
{
	/*
	sf::Texture menuText;

	sf::Image menuTxt_img;
	menuTxt_img.loadFromFile("Sprites/mission_background.png");
	menuTxt_img.createMaskFromColor(sf::Color(255, 255, 255));
	menuText.loadFromImage(menuTxt_img);

	sf::Sprite menuTxt(menuText);
	*/
	//bool isMenu = true;
	//bool isMessageBox = false;
	
	int menuNum = 0;

	int x = view.getCenter().x - view.getSize().x / 2;
	int y = view.getCenter().y -view.getSize().y / 2;

	mn_continue.setPosition(x + 100, y + 30);
	mn_load.setPosition(x + 100, y + 90);
	mn_save.setPosition(x + 100, y + 150);
	mn_settings.setPosition(x + 100, y + 210);
	mn_back_to_menu.setPosition(x + 100, y + 270);

	menuTxt.setPosition(x + 80, y + 20);
	//menuTxt.scale((float)0.5, (float)0.9);
	menuTxt.setScale((float)0.84, (float)0.9);

	//while (isMenu)
	{
		
		window.draw(menuTxt);

		if (_go_to_settings)
		{
			_go_to_settings = settings_menu(window);
		}
		else
		{
			if (!_isMessageBox)
			{
				mn_continue.setColor(sf::Color::Black);
				mn_load.setColor(sf::Color::Black);
				mn_save.setColor(sf::Color::Black);
				mn_settings.setColor(sf::Color::Black);
				mn_back_to_menu.setColor(sf::Color::Black);
				menuNum = 0;
				if (!_isMessageBox)
				{
					if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_continue.setColor(sf::Color::Blue); menuNum = 1; }
					if (sf::IntRect(100, 90, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_load.setColor(sf::Color::Blue); menuNum = 2; }
					if (sf::IntRect(100, 150, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_save.setColor(sf::Color::Blue); menuNum = 3; }
					if (sf::IntRect(100, 210, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_settings.setColor(sf::Color::Blue); menuNum = 4; }
					if (sf::IntRect(100, 270, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_back_to_menu.setColor(sf::Color::Blue); menuNum = 5; }
				}
				while (window.pollEvent(event_))
				{
					if (event_.type == sf::Event::Closed)
						window.close();

					if (event_.type == sf::Event::KeyPressed)
						if (event_.key.code == sf::Keyboard::Escape)
							return false;

					if (event_.type == sf::Event::Resized)
					{
						printf("resized\n");
						view.setSize(event_.size.width, event_.size.height);
						view.setCenter(event_.size.width / 2, event_.size.height / 2);
						view2.setSize(event_.size.width, event_.size.height);
						view2.setCenter(event_.size.width / 2, event_.size.height / 2);
						window.setView(view);
						return false;
					}

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						switch(menuNum)
						{
						case 1:
							return false;
							break;
						case 2:
							//isMenu = false;
							_load = true;
							return false;
							break;
						case 3:
							save_game();
							break;
						case 4:
							_go_to_settings = true;
							break;
						case 5:
							/*if (MessageBox(NULL, "Вы уверенны что хотите вернуться в меню?", "Выход из игры", MB_YESNO) == IDYES)
							{
								_go_to_menu = true;
								//isMenu = false; 
								return false;
							}*/
							_isMessageBox = true;
							break;
						}
					}
				}
			}
			//window.draw(menuTxt);
			window.draw(mn_continue);
			window.draw(mn_load);
			window.draw(mn_save);
			window.draw(mn_settings);
			window.draw(mn_back_to_menu);
		}
		if (_isMessageBox)
		{
			bool exit(false);
			_isMessageBox = !draw_message(window, L"Выход из игры", L"Вы хотите покинуть игру?", exit);
			if (exit)
			{
				_go_to_menu = true;
				//isMenu = false; 
				return false; 
			}
		}
		//window.display();
	}
	return true;
	////////////////////////////////////////////////////
}

bool Facade::settings_menu(sf::RenderWindow &window) 
{
	int menuNum = 0;
	bool changed(false);

	(*_p).get_move_control(left, right, up, down);
	(*_p).get_move_control(temp[0], temp[1], temp[2], temp[3]);

	window.setView(view);

	int x = view.getCenter().x - view.getSize().x / 2;
	int y = view.getCenter().y - view.getSize().y / 2;

	mn_setting_caption.setPosition(x + 200, y + 30);
	mn_key_up.setPosition(x + 100, y + 90);
	mn_key_down.setPosition(x + 100, y + 130);
	mn_key_left.setPosition(x + 100, y + 170);
	mn_key_right.setPosition(x + 100, y + 210);

	mn_key_up_value.setPosition(x + 340, y + 90);
	mn_key_down_value.setPosition(x + 340, y + 130);
	mn_key_left_value.setPosition(x + 340, y + 170);
	mn_key_right_value.setPosition(x + 340, y + 210);

	mn_accept.setPosition(x + 100, y + 270);
	mn_cancel.setPosition(x + 325, y + 270);

	mn_setting_caption.setColor(sf::Color::Black);
	mn_key_up.setColor(sf::Color::Black);
	mn_key_down.setColor(sf::Color::Black);
	mn_key_left.setColor(sf::Color::Black);
	mn_key_right.setColor(sf::Color::Black);
 
	mn_key_up_value.setColor(sf::Color::Black);
	mn_key_down_value.setColor(sf::Color::Black);
	mn_key_left_value.setColor(sf::Color::Black);
	mn_key_right_value.setColor(sf::Color::Black);

	mn_accept.setColor(sf::Color::Black);
	mn_cancel.setColor(sf::Color::Black);

	mn_key_up_value.setString(temp[2]);
	mn_key_down_value.setString(temp[3]);
	mn_key_left_value.setString(temp[0]);
	mn_key_right_value.setString(temp[1]);
	
	mn_accept.setString(FontsCFG::game_menu[13]);
	mn_cancel.setString(FontsCFG::game_menu[14]);

	//if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_continue.setColor(sf::Color::Blue); menuNum = 1; }
	if (sf::IntRect(100, 90, 300, 30).contains(sf::Mouse::getPosition(window))) { mn_key_up.setColor(sf::Color::Blue); menuNum = 1; }
	if (sf::IntRect(100, 130, 300, 30).contains(sf::Mouse::getPosition(window))) { mn_key_down.setColor(sf::Color::Blue); menuNum = 2; }
	if (sf::IntRect(100, 170, 300, 30).contains(sf::Mouse::getPosition(window))) { mn_key_left.setColor(sf::Color::Blue); menuNum = 3; }
	if (sf::IntRect(100, 210, 300, 30).contains(sf::Mouse::getPosition(window))) { mn_key_right.setColor(sf::Color::Blue); menuNum = 4; }
	if (sf::IntRect(100, 270, 200, 30).contains(sf::Mouse::getPosition(window))) { mn_accept.setColor(sf::Color::Blue); menuNum = 5; }
	if (sf::IntRect(325, 270, 200, 30).contains(sf::Mouse::getPosition(window))) { mn_cancel.setColor(sf::Color::Blue); menuNum = 6; }

	while (window.pollEvent(event_))
	{
		if (event_.type == sf::Event::Closed)
			window.close();

		if (event_.type == sf::Event::KeyPressed)
			if (event_.key.code == sf::Keyboard::Escape)
				return false;
		if (event_.type == sf::Event::Resized)
		{
			printf("resized\n");
			view.setSize(event_.size.width, event_.size.height);
			view.setCenter(event_.size.width / 2, event_.size.height / 2);
			view2.setSize(event_.size.width, event_.size.height);
			view2.setCenter(event_.size.width / 2, event_.size.height / 2);
			window.setView(view);
		}/* */

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			switch(menuNum)
			{
			case 1:
				up = write(temp[2], window);
				changed = true;
				break;
			case 2:
				down = write(temp[3], window);
				changed = true;
				break;
			case 3:
				left = write(temp[0], window);
				changed = true;
				break;
			case 4:
				right = write(temp[1], window);
				changed = true;
				break;
			case 5:
				return false;
			case 6:
				(*_p).set_move_control(_CONFIG->get_key_left(), _CONFIG->get_key_right(), _CONFIG->get_key_up(), _CONFIG->get_key_down());
				return true;
			}
			if (changed)
			{
				(*_p).set_move_control(left, right, up, down);
				changed = false;
			}
		}
	}

	window.draw(mn_setting_caption);
	window.draw(mn_key_up);
	window.draw(mn_key_down);
	window.draw(mn_key_left);
	window.draw(mn_key_right);

	window.draw(mn_key_up_value);
	window.draw(mn_key_down_value);
	window.draw(mn_key_left_value);
	window.draw(mn_key_right_value);

	window.draw(mn_accept);
	window.draw(mn_cancel);

	return true;
	////////////////////////////////////////////////////
}

bool Facade::menu(sf::RenderWindow &window) 
{
	/*
	sf::Texture menuBackground, menuText;
	menuBackground.loadFromFile("Sprites/test.png");

	sf::Image menuTxt_img;
	menuTxt_img.loadFromFile("Sprites/mission_background.png");
	menuTxt_img.createMaskFromColor(sf::Color(255, 255, 255));
	menuText.loadFromImage(menuTxt_img);

	sf::Sprite menuBg(menuBackground), menuTxt(menuText);
	*/
	bool isMenu = true;
	//bool isSetting = false;
	//bool isMessageBox = false;
	_isMessageBox = false;
	//////////////////////////
	bool isInput = false;
	//////////////////////////
	int menuNum = 0;

	//view.setCenter(_CONFIG->get_width() / 2, _CONFIG->get_height() / 2);
	window.setView(view);

	int x = view.getCenter().x - view.getSize().x / 2;
	int y = view.getCenter().y - view.getSize().y / 2;

	mn_new_game.setPosition(x + 100, y + 30);
	mn_load.setPosition(x + 100, y + 90);
	mn_change_diff.setPosition(x + 100, y + 150);
	mn_settings.setPosition(x + 100, y + 210);
	mn_exit.setPosition(x + 100, y + 330);
	mn_record_table.setPosition(x + 100, y + 270);

	menuBg.setPosition(x + 80, y + 140);
	menuTxt.setPosition(x + 80, y + 20);//80, 20);
	menuTxt.scale((float)0.84, (float)1.1);

	while (isMenu)
	{
		//menuTxt.scale((float)0.84, (float)1.1);
		window.clear(sf::Color(129, 181, 221));
		menuBg.rotate((float)0.01);

		window.draw(menuBg);
		window.draw(menuTxt);
		//printf("%i \t%i\n", sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);

		if (_go_to_settings)
		{
			menuTxt.setScale((float)0.84, (float)0.9);
			_go_to_settings = settings_menu(window);
		}
		else
		{
			if (!_isMessageBox)
			{
				menuTxt.setScale((float)0.84, (float)1.1);
				mn_change_diff.setString(L"Уровень сложности: " + FontsCFG::game_difficult[_game_difficulty]);

				mn_new_game.setColor(sf::Color::White);
				mn_load.setColor(sf::Color::Yellow);
				mn_change_diff.setColor(sf::Color::Red);
				mn_settings.setColor(sf::Color::Green);
				mn_exit.setColor(sf::Color::Black);
			
				//if (!isInput) mn_record_table.setColor(sf::Color::Black);
			
				menuNum = 0;
				if (!isInput && !_isMessageBox)
				{
					mn_record_table.setColor(sf::Color::Black);
					//if (sf::IntRect(mn_new_game.getPosition().x, mn_new_game.getPosition().y, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_new_game.setColor(sf::Color::Blue); menuNum = 1; }
					if (sf::IntRect(100, 30, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_new_game.setColor(sf::Color::Blue); menuNum = 1; }
				
					//if (sf::IntRect(mn_load.getPosition().x, mn_load.getPosition().y, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_load.setColor(sf::Color::Blue); menuNum = 2; }
					if (sf::IntRect(100, 90, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_load.setColor(sf::Color::Blue); menuNum = 2; }
				
					//if (sf::IntRect(mn_change_diff.getPosition().x, mn_change_diff.getPosition().y, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_change_diff.setColor(sf::Color::Blue); menuNum = 3; }
					//if (sf::IntRect(mn_settings.getPosition().x, mn_settings.getPosition().y, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_settings.setColor(sf::Color::Blue); menuNum = 4; }
					//if (sf::IntRect(mn_exit.getPosition().x, mn_exit.getPosition().y, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_exit.setColor(sf::Color::Blue); menuNum = 5; }
					//if (sf::IntRect(mn_record_table.getPosition().x, mn_record_table.getPosition().y, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_record_table.setColor(sf::Color::Blue); menuNum = 6; }
					if (sf::IntRect(100, 150, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_change_diff.setColor(sf::Color::Blue); menuNum = 3; }
					if (sf::IntRect(100, 210, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_settings.setColor(sf::Color::Blue); menuNum = 4; }
					if (sf::IntRect(100, 330, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_exit.setColor(sf::Color::Blue); menuNum = 5; }
					if (sf::IntRect(100, 270, 300, 50).contains(sf::Mouse::getPosition(window))) { mn_record_table.setColor(sf::Color::Blue); menuNum = 6; }
			
				}

				while (window.pollEvent(event_))
				{
					if (event_.type == sf::Event::Closed)
						window.close();

					if (event_.type == sf::Event::Resized)
					{
						printf("resized\n");
						view.setSize(event_.size.width, event_.size.height);
						view.setCenter(event_.size.width / 2, event_.size.height / 2);
						view2.setSize(event_.size.width, event_.size.height);
						view2.setCenter(event_.size.width / 2, event_.size.height / 2);
						window.setView(view);
					}/* */

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						switch(menuNum)
						{
						case 1:
							_p->set_health(FULL_HEALTH);
							isMenu = false;
							break;
						case 2:
							_load = true;
							isMenu = false;
							break;
						case 3:
							if (_game_difficulty == MissionCFG::MAX_GAME_DIFFICULT - 1) _game_difficulty = 0;
							else _game_difficulty++;
							break;
						case 4:
							//settings_menu(window);
							_go_to_settings = true;
							break;
						case 5:
							/*
							if (MessageBox(NULL, "Вы уверенны что хотите выйти?", "Выход из игры", MB_YESNO) == IDYES)
							{
								window.close();
								_exit_program = true;
								isMenu = false; 
							}/**/
							_isMessageBox = true;
							break;
						case 6:
							isInput = true;
							break;
						}
					}
			
					if (isInput)
						isInput = !write_line(mn_record_table, event_);
				}
			}
			window.draw(mn_new_game);
			window.draw(mn_load);
			window.draw(mn_change_diff);
			window.draw(mn_settings);
			window.draw(mn_exit);
			//////////////////////////////////
			window.draw(mn_record_table);
		}
		if (_isMessageBox)
		{
			bool exit(false);
			_isMessageBox = !draw_message(window, L"Выход из игры", L"Вы хотите покинуть игру?", exit);
			if (exit)
			{
				window.close();
				_exit_program = true;
				isMenu = false; 
			}
		}

		window.display();
	}
	return true;
	////////////////////////////////////////////////////
}

bool Facade::draw_message(sf::RenderWindow &window, sf::String caption, sf::String main_text, bool &result)
{
	int x = view.getCenter().x - view.getSize().x / 2;
	int y = view.getCenter().y - view.getSize().y / 2;

	int menuNum = 0;
	MSBX_sprite.setScale((float)1.0, (float)0.5);
	MSBX_sprite.setPosition(
		//x + view.getSize().x / 2,
		//y + view.getSize().y / 2
		//385, 230
		view.getCenter().x - MSBX_sprite.getTextureRect().width / 2.0f, 
		view.getCenter().y - MSBX_sprite.getTextureRect().height / 2.0f
		);

	window.draw(MSBX_sprite);


	mn_accept.setColor(sf::Color::Black);
	mn_cancel.setColor(sf::Color::Black);
	mn_caption.setColor(sf::Color::Black);
	mn_text.setColor(sf::Color::Black);

	mn_accept.setString(L"Да");
	mn_cancel.setString(L"Нет");

	int dX = view2.getCenter().x - view.getCenter().x;
	int dY = view2.getCenter().y - view.getCenter().y;
	
	mn_accept.setPosition(
		MSBX_sprite.getPosition().x + 90,
		MSBX_sprite.getPosition().y + 120
		);

	mn_cancel.setPosition(
		MSBX_sprite.getPosition().x + MSBX_sprite.getTextureRect().width - 140,
		MSBX_sprite.getPosition().y + 120
		);

	mn_caption.setPosition(
		(MSBX_sprite.getPosition().x + MSBX_sprite.getTextureRect().width / 2.0f) - caption.getSize() * 8,
		MSBX_sprite.getPosition().y + 10
		);

	mn_text.setPosition(
		(MSBX_sprite.getPosition().x + MSBX_sprite.getTextureRect().width / 2.0f) - main_text.getSize() * 8,
		MSBX_sprite.getPosition().y + 60
		);

	mn_caption.setString(caption);
	mn_text.setString(main_text);

	//if (sf::IntRect(475, 350, 150, 50).contains(sf::Mouse::getPosition(window))) { mn_accept.setColor(sf::Color::Blue); menuNum = 1; }
	//if (sf::IntRect(755, 350, 150, 50).contains(sf::Mouse::getPosition(window))) { mn_cancel.setColor(sf::Color::Blue); menuNum = 2; }		
	if (sf::IntRect(mn_accept.getPosition().x + dX, mn_accept.getPosition().y + dY, 150, 50).contains(sf::Mouse::getPosition(window))) { mn_accept.setColor(sf::Color::Blue); menuNum = 1; printf("%i\t%i\n", sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y); }
	if (sf::IntRect(mn_cancel.getPosition().x + dX, mn_cancel.getPosition().y + dY, 150, 50).contains(sf::Mouse::getPosition(window))) { mn_cancel.setColor(sf::Color::Blue); menuNum = 2; }

	while (window.pollEvent(event_))
	{
		if (event_.type == sf::Event::Closed)
			window.close();

		if (event_.type == sf::Event::Resized)
		{
			printf("resized\n");
			view.setSize(event_.size.width, event_.size.height);
			view.setCenter(event_.size.width / 2, event_.size.height / 2);
			view2.setSize(event_.size.width, event_.size.height);
			view2.setCenter(event_.size.width / 2, event_.size.height / 2);
			window.setView(view);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			switch(menuNum)
			{
			case 1:
				result = true;
				return true;
			case 2:
				result = false;
				return true;
			}
		}
	}
	window.draw(mn_accept);
	window.draw(mn_cancel);
	window.draw(mn_caption);
	window.draw(mn_text);
	return false;
}

bool Facade::load_procedure()
{
	_load = false;
	if (load_game())
	{
		_p->set_x(save_params.x);
		_p->set_y(save_params.y);

		_p->set_score(save_params.score);
		_p->set_health(save_params.health);

		_game_difficulty = save_params.difficult;
		game_mission.set_mission_number(save_params.mission_number);
		game_mission.set_stone_count(save_params.stone_count);

		for(int i = 0; i < MapCFG::MAP_HEIGHT; i++)
			game_map.get_map()[i] = save_params.map[i];

		init_enemy();
					
		_next_mission = false;
		__view_info = true;

		for(int i = 0; i < MapCFG::MAP_HEIGHT; i++)
			rr_params.copy_map[i] = save_params.copy_map[i];

		rr_params.score = save_params.rr_score;
		rr_params.health = save_params.rr_health;
		return true;
	}
	else
	{
		_restart = false;
		return false;
	}
}

bool Facade::restart_procedure()
{
	_p->set_score(rr_params.score);
	_p->set_health(rr_params.health);

	score_text.setPosition(FontsCFG::SCORE_POS[0], FontsCFG::SCORE_POS[1]);
	view.rotate(-view.getRotation());


	_next_mission = false;
	game_map.load_map(rr_params.copy_map);
	_restart = false;

	return true;
}

bool Facade::begining_procedure()
{
	menu(*_window);
	_next_mission = false;
	game_mission.set_mission_number(_CONFIG->get_default_mission());
	init_enemy();
	game_map.load_map(rr_params.default_map);
	__view_info = true;

	game_map.map_reserve(rr_params.copy_map);

	return true;
}

bool Facade::main_procedure()
{
	
	/* =================================================================== */
	this->set_game_speed();
	this->event_handler();
	//view_control(game_speed); // демонстрация возможностей камеры
	_window->setView(view); // задаем параметры камеры ДО очистки экрана
	_window->clear(sf::Color(129, 181, 221));
	/* =================================================================== */
	game_map.draw_map((*_window));//, __map_sprites); // Отрисовка карты
	/* =================================================================== */
	if (_go_to_mini_menu || __view_info)
		_entity_list.notify((*_window), __game_speed, true, __current_frame, (*_p));
	else
		_entity_list.notify((*_window), __game_speed, false, __current_frame, (*_p));
	/* =================================================================== */
	set_camera_view(_p->get_x(), _p->get_y()); // задаем слежку камеры за объектом находящемся по координатам Х и У

	rotate_screen();
	/* =================================================================== */

	if (__view_info)
	{
		if (_next_mission)
			game_mission.get_mission_complete_text(
				(*_window), 
				mission_text, 
				view.getCenter().x,
				view.getCenter().y
				//_CONFIG->get_width() / 2, 
				//_CONFIG->get_height() / 2//, 
				);//this->_mission_number);
		else
			game_mission.get_mission_text(
				(*_window), 
				mission_text, 
				view.getCenter().x,
				view.getCenter().y
				//_CONFIG->get_width() / 2, 
				//_CONFIG->get_height() / 2//, 
				);//this->_mission_number);
	}

	if (game_mission.get_stone_count() == 0 && !_next_mission)
	{
		_next_mission = true;
		__view_info = true;
		game_map.map_reserve(rr_params.copy_map);
		rr_params.score = _p->get_score();
		rr_params.health = _p->get_health();
	}

	if (_go_to_menu)
	{
		_go_to_menu = false;
		return true;
	}

	if (_restart)
		return true;

	if (_load)
		return true;

	_window->draw(score_text);

	if (_go_to_mini_menu)
		_go_to_mini_menu = mini_menu(*_window);

	_window->display();
	return false;
}

bool Facade::start_engine()
{
	try
	{
		if (_is_init_settings)
		{

			///////////
			view2 = view;
			///////////
			rr_params.health = FULL_HEALTH;
			rr_params.score = 0;
			game_map.map_reserve(rr_params.default_map);

			while (!_exit_program)
			{
				if (_load)
				{
					if (!load_procedure())
						continue;
				}
				else
				{
					if (!_restart)
						begining_procedure();
					else
						restart_procedure();
				
					game_mission.set_new_mission(&game_map);//, _mission_number);
					_p->set_x(_CONFIG->get_pos_x());
					_p->set_y(_CONFIG->get_pos_y());
				}
				while (_window->isOpen())
				{
					if (main_procedure())
						break;
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