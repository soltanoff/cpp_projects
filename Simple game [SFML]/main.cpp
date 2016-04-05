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

	//std::list<G_Character<float>*>  obj; // ������ ������, ���� ���� ������ �������.�������� ������.
	//std::list<G_Character<float>*>::iterator it; // �������� ����� ��������� �� ��-��� ������
	/* =================================================================== */
	G_Config* CONFIG = G_Config::instance();
	CONFIG->take_config();

	// ������ ������� ����
	sf::RenderWindow window(sf::VideoMode(CONFIG->get_width(), CONFIG->get_height()), "Test",
		CONFIG->is_fullscreen() ? sf::Style::Fullscreen: sf::Style::Default);
	view.reset(sf::FloatRect(0.0, 0.0, CONFIG->get_width(), CONFIG->get_height())); // ��������� ������ � ������ ����������� ������
	
	fonts_settings(); // ����������� �����
	sounds_settings(); // ����������� ����
	set_mission_textbox(); // ����������� ���� �������
	set_new_mission(CONFIG->get_default_mission());
	bool view_info(false);
	
	/* =================================================================== */
	Player<float> p(CONFIG->get_pos_x(), CONFIG->get_pos_y(), player_image);//, 6, 136, 89, 55, "hero.png");//80.0, 80.0, 96, 96, "hero.png");
	Enemy<float> enemy(CONFIG->get_pos_x()+300, CONFIG->get_pos_y(), "gun", enemy_image);
	//Bullet<float> bull(CONFIG->get_pos_x()+100, CONFIG->get_pos_y(),  bullet_image);
	/* =================================================================== */
	
	// ������ �����
	sf::Texture map_texture; // �������� �����
	map_texture.loadFromFile("Sprites/map.png");

	sf::Sprite map_sprites; // ������ ������ ��� �����
	map_sprites.setTexture(map_texture); // �������� �������� ��������
	/* =================================================================== */
	
	sf::Clock system_clock; // ������� ������, ������� ������ ����� (����� ������� ��� �������� ������� � "�����" ��������� ��������"
	float current_frame(0); // ������ ������� ����
	bool is_view_map(false); // �������� � ���, ��� ������ ������������� ����� 
	while (window.isOpen())
	{
		/* =================================================================== */
		// ������ �������� ����
		float game_speed = float(system_clock.getElapsedTime().asMicroseconds()); // ������� ��������� ����� � ������������ � ����� �� ���������, � ����� �� �������� �������� ����
		system_clock.restart(); // ���������
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
					//if (MessageBox(NULL,"�� �������� ��� ������ �����?", "����� �� ����", MB_YESNO) == IDYES)
						window.close();
					break;
				}
			}
		}
		/* =================================================================== */
		// ������������ ������� ������
		if (p.is_alive())
		{
			if (!is_view_map) 
			{
				p.move(game_speed, current_frame, 0.1);
				//set_camera_view(p.get_x(), p.get_y()); // ������ ������ ������ �� �������
			}
			else
				view_map(game_speed); // ��������� ��������� �����

			score_text.setString(score_string + itoa(p.get_score(), buffer, 10) + "\n" + health_string + itoa(p.get_health(), buffer, 10));
		}
		else 
		{
			view.rotate(0.01);

			score_text.setPosition(CONFIG->get_width() / 2.0 - 100, CONFIG->get_height() / 2.0 - 40);
			score_text.setString(score_string + itoa(p.get_score(), buffer, 10) + "\n" + game_over_string);
		}
		enemy.search_enemy(p);
		//view_control(game_speed); // ������������ ������������ ������
		window.setView(view); // ������ ��������� ������ �� ������� ������
		window.clear();

		draw_map(window, map_sprites); // ��������� �����
		if (view_info) get_mission_text(window, mission_text, CONFIG->get_width() / 2.0, CONFIG->get_height() / 2.0, CONFIG->get_default_mission());
		window.draw(p.get_sprite());
		//window.draw(bull.get_sprite());
		window.draw(enemy.get_sprite());
		enemy.update(game_speed);
		//window.draw((*(enemy.enemy_bullets.begin()))->get_sprite());
		/*
		for (it = enemy.enemy_bullets; it != enemy.enemy_bullets.end(); it++)
		{
			window.draw((*it)->get_sprite()); //������ ������� (������ ��� ������ �����)
		}/**/
		/*for (it = obj.begin(); it != obj.end(); it++)
		{
			window.draw((*it)->get_sprite()); //������ ������� (������ ��� ������ �����)
		}*/

		window.draw(score_text);
		window.display();
	}
 
	return 0;
}

/*sf::Texture hero_texture; // ����������� ���������
	hero_texture.loadFromFile("Sprites/hero.png"); 
	// �������� ����� ���������, �.�. ����� ������� 
	sf::Sprite hero_sprite; // ������� ������ (���� ���)
	hero_sprite.setTexture(hero_texture);    //� ����   ����� ����
	hero_sprite.setTextureRect(sf::IntRect(0*LION_POS, 1*LION_POS, 1*LION_POS, 1*LION_POS));// ������ �������
	hero_sprite.setPosition(50.0,50.0); // ������ ��������� ������� �������
	*/

/*
	sf::Image hero_image; // ����������� �����
	hero_image.loadFromFile("Sprites/hero.png"); 

	sf::Texture hero_texture; // ����������� ���������
	hero_texture.loadFromImage(hero_image);
*/