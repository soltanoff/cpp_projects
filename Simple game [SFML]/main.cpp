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
	// ������ ������� ����
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Test");
	/* =================================================================== */

	Player p(50.0, 50.0, LION_POS, LION_POS, "hero.png");
	/* =================================================================== */
	// ������ �����
	sf::Texture map_texture; // �������� �����
	map_texture.loadFromFile("Sprites/map.png");

	sf::Sprite map_sprites; // ������ ������ ��� �����
	map_sprites.setTexture(map_texture); // �������� �������� ��������
	/* =================================================================== */
	sf::Clock system_clock; // ������� ������, ������� ������ ����� (����� ������� ��� �������� ������� � "�����" ��������� ��������"
	float current_frame(0); // ������ ������� ����
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
		p.move(game_speed, current_frame, 0.1);
		/* =================================================================== */
		// ������� ����� ������
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			if (MessageBox(NULL,"�� �������� ��� ������ �����?", "����� �� ����", MB_YESNO) == IDYES)
				window.close();
		}
		/* =================================================================== */

		window.clear();
		draw_map(window, map_sprites); // ��������� �����
		window.draw(p.get_sprite());
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