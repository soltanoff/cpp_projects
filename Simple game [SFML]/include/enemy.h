#ifndef G_ENEMY
#define G_ENEMY
#include "g_character.h"
#include "player.h"
#include "textures_settings.h"
#include <math.h>
#include <list>

sf::Image bullet_image;

template<class T>
class Bullet : public G_Character<T>
{
private:
	void map_iteraction() {}
public:
	int k;

	Bullet(T X, T Y, T dX, T dY, sf::Image &image):
		G_Character(X, Y,
			Bullet_Texture::RECT_LEFT, 
			Bullet_Texture::RECT_TOP, 
			Bullet_Texture::WIDTH, 
			Bullet_Texture::HEIGHT
			, image
		)
	{
		this->entity_sprite.setOrigin(this->entity_sprite.getTextureRect().width / 2.0, this->entity_sprite.getTextureRect().height / 2.0);
		this->dx = dX;
		this->dy = dY;
	}

	void search_enemy(Player<T> &enemy)
	{	
		T dX = - this->x + enemy.get_x(); // вектор , колинеарный прямой, которая пересекает спрайт и курсор
		T dY = - this->y + enemy.get_y(); // он же, координата y

		float rotation = (atan2(dY, dX)) * 180 / 3.14159265; // получаем угол в радианах и переводим его в градусы
		//printf("rotation: %f \n", rotation); // смотрим на градусы в консольке
		this->entity_sprite.setRotation(rotation); // поворачиваем спрайт на эти градусы	


	}



	void update(float game_speed)
	{
		this->x += dx * game_speed;
		this->y += dy * game_speed;
	}
};

const int CRITICAL_DISTANCE = 250;

template<class T>
class Enemy : public G_Character<T>
{
private:
	bool enemy_spoted;
	sf::String name;
	void map_iteraction() {}
public:
	std::list<Bullet<float>*> enemy_bullets; // создаю список пуль
	//std::list<Bullet<float>*>::iterator it; // итератор чтобы проходить по эл-там списка

	Enemy(T X, T Y, sf::String enemy_name, sf::Image &image):
		enemy_spoted(false), name(enemy_name),
		G_Character(X, Y,
			Enemy_Texture::RECT_LEFT, 
			Enemy_Texture::RECT_TOP, 
			Enemy_Texture::WIDTH, 
			Enemy_Texture::HEIGHT
			, image
		)
	{
		bullet_image.loadFromFile("Sprites/bullet.png");
		bullet_image.createMaskFromColor(sf::Color(255, 255, 255));
		this->entity_sprite.setOrigin(this->entity_sprite.getTextureRect().width / 2.0, this->entity_sprite.getTextureRect().height / 2.0);
	}

	void search_enemy(Player<T> &enemy)
	{	
		T dX = - this->x + enemy.get_x(); // вектор , колинеарный прямой, которая пересекает спрайт и курсор
		T dY = - this->y + enemy.get_y(); // он же, координата y

		float rotation = (atan2(dY, dX)) * 180 / 3.14159265; // получаем угол в радианах и переводим его в градусы
		//printf("rotation: %f \n", rotation); // смотрим на градусы в консольке
		this->entity_sprite.setRotation(rotation); // поворачиваем спрайт на эти градусы	

		int distance = sqrt( pow(dX, 2) + pow(dY, 2) );
		printf("distance %i ", distance);
		if (distance <= CRITICAL_DISTANCE)
		{
			if (this->enemy_bullets.empty())
				this->enemy_bullets.push_back(new Bullet<float>(this->x, this->y, dX, dY, bullet_image));
			printf("TARGET!");
		}
		printf("\n");
	}


	//***************************************************
	void update(float game_speed)
	{
		/*for (it = enemy_bullets; it != enemy_bullets.end(); it++)
		{
			(*it)->update(game_speed); //рисуем объекты (сейчас это только враги)
		}*/

		//(*enemy_bullets.begin())->update(game_speed);
	}
};
#endif /* G_ENEMY */