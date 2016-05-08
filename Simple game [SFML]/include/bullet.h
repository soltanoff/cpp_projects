#ifndef G_BULLET
#define G_BULLET
#include "g_config.h"
#include "g_character.h"
#include "player.h"
#include "main_settings.h"

sf::Image bullet_image;


class Bullet : public G_Character
{
private:
	/* ============================================================================================================================= */
	inline void map_iteraction();
	int bullet_speed;
	/* ============================================================================================================================= */
public:
	/* ============================================================================================================================= */
	Bullet(float X, float Y, float dX, float dY, sf::Image &image):
		G_Character(X, Y,
			(float)Bullet_Texture::RECT_LEFT, 
			(float)Bullet_Texture::RECT_TOP, 
			(float)Bullet_Texture::WIDTH, 
			(float)Bullet_Texture::HEIGHT
			, image
		)
	{
		this->entity_sprite.setOrigin((float)(this->entity_sprite.getTextureRect().width / 2.0), (float)(this->entity_sprite.getTextureRect().height / 2.0));
		this->dx = dX;
		this->dy = dY;
		G_Config *cfg = G_Config::instance();
		this->bullet_speed = cfg->get_bullet_speed();
	}
	/* ============================================================================================================================= */
	inline void player_iteraction(G_Character &p);
	/* ============================================================================================================================= */
	inline void set_coord(float X, float Y, float rotation);
	inline void set_direction(float dX, float dY);
	/* ============================================================================================================================= */
	inline void update(float game_speed);
	void action_time(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p);
	/* ============================================================================================================================= */
};


void Bullet::map_iteraction()
{
	for (int i = (int)(this->get_y() / MapCFG::MAP_TILE_SIZE); i < (this->get_y() + entity_sprite.getTextureRect().height) / MapCFG::MAP_TILE_SIZE; i++)
	{
		for (int j = (int)(this->get_x() / MapCFG::MAP_TILE_SIZE); j < (this->get_x() + entity_sprite.getTextureRect().width) / MapCFG::MAP_TILE_SIZE; j++)
		{
			if (Map::get_map()[i][j] == MapCFG::MAP_CURB)
			{
				this->set_health(0);
			}
		}
	}
}

void Bullet::set_coord(float X, float Y, float rotation)
{
	this->entity_sprite.setRotation(rotation);
	this->entity_sprite.setPosition(X, Y);
}

void Bullet::set_direction(float dX, float dY)
{
	this->dx = dX;
	this->dy = dY;
}

void Bullet::player_iteraction(G_Character &p)
{
	//if (abs(this->x + entity_sprite.getTextureRect().width - p.get_x()) < 1 && 
		//abs(this->y + entity_sprite.getTextureRect().height - p.get_y()) < 1)
		//printf("TARGET\n %f %f", p.get_x(), p.get_y());
	//if (this->x == p.get_x() && this->y == p.get_y())
	if (this->is_alive())
	if (this->get_x() <=  p.get_x() + p.get_width() && this->get_x() >= p.get_x() &&
		this->get_y() <=  p.get_y() + p.get_height() && this->get_y() >= p.get_y())
	{
		if (this->is_alive())
			p.health_decr(EnemyCFG::BULLET_DAMAGE);
		this->entity_sprite.setPosition(0.0, 0.0);
		this->set_health(0);
	}
}

extern int BULLET_SPEED;

void Bullet::update(float game_speed)
{
	if (game_speed >= 1)
	{
		//this->x += dx / game_speed / EnemyCFG::BULLET_SPEED;
		//this->y += dy / game_speed / EnemyCFG::BULLET_SPEED;
			
		this->entity_sprite.setPosition(
			this->get_x() + dx / game_speed / bullet_speed,//EnemyCFG::BULLET_SPEED, 
			this->get_y() + dy / game_speed / bullet_speed//EnemyCFG::BULLET_SPEED
			);
		this->map_iteraction();
	}
}

void Bullet::action_time(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p)
{
		
}
#endif /* G_BULLET */