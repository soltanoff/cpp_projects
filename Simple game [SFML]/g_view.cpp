#include "g_view.h"



sf::View view; // определяем объект вида, при помощи которого организуем слежку камеры за игроком



void set_camera_view(float x, float y) // задаем слежку камеры за объектом находящемся по координатам Х и У
{
	//if (!(y < 1280 || x < 680))
		view.setCenter(x, y);
}

void view_map(float game_speed)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		view.setCenter(view.getCenter().x - game_speed, view.getCenter().y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		view.setCenter(view.getCenter().x + game_speed, view.getCenter().y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		view.setCenter(view.getCenter().x, view.getCenter().y - game_speed);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		view.setCenter(view.getCenter().x, view.getCenter().y + game_speed);
	}
}

void view_control(float game_speed)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
	{
		view.zoom(1.0100f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
	{
		view.zoom(0.999f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
	{
		view.rotate(0.1f); // поворот на 1 градус
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		view.rotate(-0.1f); // поворот на -1 градус
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
	{
		view.setSize(1280, 800);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
	{
		view.setSize(680, 480);
	}
}