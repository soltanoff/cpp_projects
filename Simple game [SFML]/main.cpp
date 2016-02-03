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
	// ������ ������� ����
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Test");
	view.reset(sf::FloatRect(0.0, 0.0, WINDOW_WIDTH, WINDOW_HEIGHT)); // ��������� ������ � ������ ����������� ������

	fonts_settings(); // ����������� �����
	sounds_settings(); // ����������� ����
	/* =================================================================== */

	Player p(80.0, 80.0, 96, 96, "hero.png");
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
		game_speed /= TIME_DIV;
		/* =================================================================== */
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		/* =================================================================== */
		// ������������ ������� ������
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
			is_view_map = !is_view_map;

		if (!is_view_map) 
		{
			p.move(game_speed, current_frame, 0.1);
			set_camera_view(p.get_x(), p.get_y()); // ������ ������ ������ �� �������
		}
		else
			view_map(game_speed); // ��������� ��������� �����
			
		//view_control(game_speed); // ������������ ������������ ������
		/* =================================================================== */
		// ������� ����� ������
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (MessageBox(NULL,"�� �������� ��� ������ �����?", "����� �� ����", MB_YESNO) == IDYES)
				window.close();
		}
		/* =================================================================== */
		score_text.setString(score_string + itoa(p.get_score(), buffer, 10));
		window.setView(view); // ������ ��������� ������ �� ������� ������
		window.clear();

		draw_map(window, map_sprites); // ��������� �����
		window.draw(p.get_sprite());
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