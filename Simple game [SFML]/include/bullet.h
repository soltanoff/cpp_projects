#ifndef G_BULLET
#define G_BULLET
#include "g_character.h"
#include "player.h"


sf::Image bullet_image;

const int BULLET_SPEED = 120;
const int BULLET_DAMAGE = 1;


class Bullet : public G_Character
{
private:
	/* ============================================================================================================================= */
	inline void map_iteraction();
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
	for (int i = (int)(this->y / MAP_TILE_SIZE); i < (this->y + entity_sprite.getTextureRect().height) / MAP_TILE_SIZE; i++)
	{
		for (int j = (int)(this->x / MAP_TILE_SIZE); j < (this->x + entity_sprite.getTextureRect().width) / MAP_TILE_SIZE; j++)
		{
			if (simple_map_structure[i][j] == MAP_CURB)
			{
				this->set_health(0);
			}
		}
	}
}

void Bullet::set_coord(float X, float Y, float rotation)
{
	this->x = X;
	this->y = Y;

	this->entity_sprite.setRotation(rotation);
	this->entity_sprite.setPosition(this->x, this->y);
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
	if (this->x <=  p.get_x() + p.get_width() && this->x >= p.get_x() &&
		this->y <=  p.get_y() + p.get_height() && this->y >= p.get_y())
	{
		this->set_health(0);
		p.health_decr(BULLET_DAMAGE);
	}
}

void Bullet::update(float game_speed)
{
	if (game_speed >= 1)
	{
		this->x += dx / game_speed / BULLET_SPEED;
		this->y += dy / game_speed / BULLET_SPEED;
			
		this->entity_sprite.setPosition(this->x, this->y);
		this->map_iteraction();
	}
}

void Bullet::action_time(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p)
{
		
}
#endif /* G_BULLET */