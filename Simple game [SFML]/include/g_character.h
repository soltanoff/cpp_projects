#ifndef G_CHARACTER
#define G_CHARACTER
#include "g_entity.h"
#define FULL_HEALTH 100


class G_Character : public G_Entity
{
protected:
	float health; // здоровье персонажа
	float dx; // ускорение по Ox
	float dy; // ускорение по Oy 
	float speed; // скорость
	short dir; // направление движения
public:
	G_Character(float X, float Y, float H, float W, sf::String Txtr_File):
		G_Entity(X, Y, H, W, Txtr_File), health(FULL_HEALTH), dx(0), dy(0), speed(0)
	{
	}
	
	float get_health() { return this->health; }
	short get_dir() { return this->dir; }
	float get_dx() { return this->dx; }
	float get_dy() { return this->dy; }
	float get_speed() { return this->speed; }

	void set_health(float Health) { this->health = Health; }
	void set_dir(short Dirrection) { this->dir = Dirrection; }
	void set_dx(float Acceleration) { this->dx = Acceleration; }
	void set_dy(float Acceleration) { this->dy = Acceleration; }
	void set_speed(float Speed) { this->speed = Speed; }

	virtual void update(float game_speed)
	{
		switch(this->dir)
		{
		case 0:
			this->dx = speed;
			this->dy = 0;
			break;
		case 1:
			this->dx = -speed;
			this->dy = 0;
			break;
		case 2:
			this->dx = 0;
			this->dy = speed;
			break;
		case 3:
			this->dx = 0;
			this->dy = -speed;
			break;
		}

		this->x += dx * game_speed;
		this->y += dy * game_speed;

		this->speed = 0;
		this->entity_sprite.setPosition(this->x, this->y);
	}
};

#endif /* G_CHARACTER */