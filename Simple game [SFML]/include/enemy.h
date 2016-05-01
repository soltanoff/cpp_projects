#ifndef G_ENEMY
#define G_ENEMY
#include "textures_settings.h"
#include "g_character.h"
#include "player.h"
#include "bullet.h"
#include <math.h>



class Enemy : public G_Character
{
private:
	/* ============================================================================================================================= */
	sf::String name;
	Bullet *enemy_bullets;
	/* ============================================================================================================================= */
	bool shooted;
	bool enemy_spoted;
	/* ============================================================================================================================= */
	inline void search_enemy(G_Character &enemy);
	inline void shot(sf::RenderWindow &window);
	/* ============================================================================================================================= */
	inline void update(float game_speed);
	void enemy_action(sf::RenderWindow &window, G_Character &p, float game_speed, bool no_iteraction);
	/* ============================================================================================================================= */
public:
	/* ============================================================================================================================= */
	Enemy(float X, float Y, sf::String enemy_name, sf::Image &image):
		enemy_spoted(false), name(enemy_name),
		G_Character(X, Y,
			(float)Enemy_Texture::RECT_LEFT, 
			(float)Enemy_Texture::RECT_TOP, 
			(float)Enemy_Texture::WIDTH, 
			(float)Enemy_Texture::HEIGHT
			, image
		)
	{
		//bullet_image.loadFromFile("Sprites/bullet.png");
		//bullet_image.createMaskFromColor(sf::Color(255, 255, 255));
		enemy_bullets = new Bullet(this->get_x(), this->get_y(), (float)0.0, (float)0.0, bullet_image);

		shooted = false;

		this->entity_sprite.setOrigin((float)(this->entity_sprite.getTextureRect().width / 2.0), (float)(this->entity_sprite.getTextureRect().height / 2.0));
	}
	/* ============================================================================================================================= */
	void action_time(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p);
	/* ============================================================================================================================= */
};


void Enemy::search_enemy(G_Character &enemy)
{	
	//float dX = - this->get_x() + (enemy.get_x());// + enemy.get_width()); // вектор , колинеарный прямой, которая пересекает спрайт и курсор
	//float dY = - this->get_y() + (enemy.get_y());// + enemy.get_height()); // он же, координата y
	/* ======================================== */
	float dX = - this->get_x() + (2 * enemy.get_x() + enemy.get_width()) / 2.0f;
	float dY = - this->get_y() + (2 * enemy.get_y() + enemy.get_height()) / 2.0f;
	/* ======================================== */
	float rotation = (float)((atan2(dY, dX)) * 180 / 3.14159265); // получаем угол в радианах и переводим его в градусы
	//printf("rotation: %f \n", rotation); // смотрим на градусы в консольке
	this->entity_sprite.setRotation(rotation); // поворачиваем спрайт на эти градусы	

	int distance = (int)sqrt( pow(dX, 2) + pow(dY, 2) );
	//printf("distance %i \n", distance);
	if (distance <= EnemyCFG::GUN_CRITICAL_DISTANCE && distance >= EnemyCFG::GUN_MINIMAL_DISTANCE)
	{
		if (!shooted)
		{
			enemy_bullets->set_coord(this->get_x(), this->get_y(), rotation);
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

void Enemy::update(float game_speed)
{
	if (shooted && enemy_bullets->is_alive())
	{
		enemy_bullets->update(game_speed);
	}
	else
	{
		shooted = false;
		//enemy_bullets->set_health(FULL_HEALTH);
	}
	if (!enemy_bullets->is_alive() && !shooted)
		enemy_bullets->set_health(FULL_HEALTH);
	//if (enemy_bullets != NULL)
		//enemy_bullets->update(game_speed);
	/*for (it = enemy_bullets; it != enemy_bullets.end(); it++)
	{
		(*it)->update(game_speed); //рисуем объекты (сейчас это только враги)
	}*/

	//(*enemy_bullets.begin())->update(game_speed);
}

void Enemy::shot(sf::RenderWindow &window)
{
	if (shooted && enemy_bullets->is_alive())
		window.draw(enemy_bullets->get_sprite());
}

void Enemy::enemy_action(sf::RenderWindow &window, G_Character &p, float game_speed, bool no_iteraction)
{
	this->search_enemy(p);
	if (!no_iteraction) 
	{
		this->update(game_speed);
		if (this->enemy_bullets->is_alive()) this->enemy_bullets->player_iteraction(p);
	}
	this->shot(window);
	window.draw(this->get_sprite());
}

void Enemy::action_time(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p)
{
	enemy_action(window, p, game_speed, no_iteraction);
}
#endif /* G_ENEMY */