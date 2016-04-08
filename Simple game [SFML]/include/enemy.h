#ifndef G_ENEMY
#define G_ENEMY
#include "g_character.h"
#include "player.h"
#include "textures_settings.h"
#include <math.h>
#include <list>

sf::Image bullet_image;

const int BULLET_SPEED = 50;

template<class T>
class Bullet : public G_Character<T>
{
private:
	void map_iteraction();
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

	void set_health(int h) { this->health = h; }

	void set_coord(T X, T Y, float rotation)
	{
		this->x = X;
		this->y = Y;

		this->entity_sprite.setRotation(rotation);
		this->entity_sprite.setPosition(this->x, this->y);
	}

	void set_direction(T dX, T dY)
	{
		this->dx = dX;
		this->dy = dY;
	}

	void update(float game_speed)
	{
		if (game_speed >= 1)
		{
			this->x += dx / game_speed / BULLET_SPEED;
			this->y += dy / game_speed / BULLET_SPEED;
			
			this->entity_sprite.setPosition(this->x, this->y);
			this->map_iteraction();
		}
	}
};

template<class T>
void Bullet<T>::map_iteraction()
{
	for (int i = this->y / MAP_TILE_SIZE; i < (this->y + entity_sprite.getTextureRect().height) / MAP_TILE_SIZE; i++)
	{
		for (int j = this->x / MAP_TILE_SIZE; j < (this->x + entity_sprite.getTextureRect().width) / MAP_TILE_SIZE; j++)
		{
			if (simple_map_structure[i][j] == MAP_CURB)
			{
				this->set_health(0);
			}
		}
	}
}

const int CRITICAL_DISTANCE = 300;

template<class T>
class Enemy : public G_Character<T>
{
private:
	bool enemy_spoted;
	sf::String name;
	void map_iteraction() {}
public:
	//std::list<Bullet<float>*> enemy_bullets; // создаю список пуль
	//std::list<Bullet<float>*>::iterator it; // итератор чтобы проходить по эл-там списка
	Bullet<T>* enemy_bullets;

	bool shooted;

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
		//bullet_image.loadFromFile("Sprites/bullet.png");
		//bullet_image.createMaskFromColor(sf::Color(255, 255, 255));
		enemy_bullets = new Bullet<T>(this->x, this->y, 0, 0, bullet_image);

		shooted = false;

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
		//printf("distance %i ", distance);
		if (distance <= CRITICAL_DISTANCE)
		{
			if (!shooted)
			{
				enemy_bullets->set_coord(this->x, this->y, rotation);
				enemy_bullets->set_direction(dX, dY);
				shooted = true;
			}

			//if (enemy_bullets == NULL)
				//enemy_bullets = new Bullet<T>(this->x, this->y, dX, dY, bullet_image);
			//if (this->enemy_bullets.empty())
				//this->enemy_bullets.push_back(new Bullet<float>(this->x, this->y, dX, dY, bullet_image));
			//printf("TARGET!");
		}
		//printf("\n");
	}


	//***************************************************
	void update(float game_speed)
	{
		if (shooted && enemy_bullets->is_alive())
			enemy_bullets->update(game_speed);
		else
		{
			shooted = false;
			enemy_bullets->set_health(FULL_HEALTH);
		}
		//if (enemy_bullets != NULL)
			//enemy_bullets->update(game_speed);
		/*for (it = enemy_bullets; it != enemy_bullets.end(); it++)
		{
			(*it)->update(game_speed); //рисуем объекты (сейчас это только враги)
		}*/

		//(*enemy_bullets.begin())->update(game_speed);
	}

	void shot(sf::RenderWindow &window)
	{
		if (shooted && enemy_bullets->is_alive())
			window.draw(enemy_bullets->get_sprite());
	}
};
#endif /* G_ENEMY */