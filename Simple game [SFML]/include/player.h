#ifndef _PLAYER_
#define _PLAYER_
#include "textures_settings.h"
#include "g_character.h"
#include "simple_map.h"
#include "sounds.h"
#include "missions.h"


template<class T>
class Player : public G_Character<T>
{
private:
	sf::Keyboard::Key _move_left;
	sf::Keyboard::Key _move_right;
	sf::Keyboard::Key _move_up;
	sf::Keyboard::Key _move_down;

	int game_score; 
	void map_iteraction();
public:
	Player(T X, T Y, sf::Image &image):
		G_Character(X, Y, 
			Player_Texture::MVL_LEFT_STEP, 
			Player_Texture::MVL_TOP, 
			Player_Texture::MVL_WIDTH, 
			Player_Texture::MVL_HEIGHT, image
		), game_score(0),
		_move_left(sf::Keyboard::A), _move_right(sf::Keyboard::D), 
		_move_up(sf::Keyboard::W), _move_down(sf::Keyboard::S)
	{
	}

	bool move(float game_speed, float &current_frame, float obj_speed = 0.1);
	void set_move_control(sf::Keyboard::Key Left, sf::Keyboard::Key Right, sf::Keyboard::Key Up, sf::Keyboard::Key Down)
	{
		_move_left = Left;
		_move_right = Right; 
		_move_up = Up;
		_move_down = Down;
	}

	int get_score() { return this->game_score; }
	
	void set_score(int score) { this->game_score = score; }
	
	void health_decr(int value)
	{
		this->health -= value;
	}

	bool is_alive() 
	{ 
		if (this->health > 0) return true; 
		else 
		{
			if (this->health < 0) this->health = 0;
			this->entity_sprite.setColor(sf::Color(0, 0, 0));
			return false;
		}
	}

	void update(float game_speed)
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

template<class T>
bool Player<T>::move(float game_speed, float &current_frame, float obj_speed)
{
	bool moved = false;
	if (sf::Keyboard::isKeyPressed(this->_move_left)) 
	{
		this->dir = 1;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(
			sf::IntRect(
			Player_Texture::BASE_RECTLEFT_POS * int(current_frame) + Player_Texture::MVL_LEFT_STEP, 
			Player_Texture::MVL_TOP, 
			Player_Texture::MVL_WIDTH, 
			Player_Texture::MVL_HEIGHT
			));//(sf::IntRect(int(current_frame) * this->sprite_w, 136, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
	}
	else if (sf::Keyboard::isKeyPressed(this->_move_right))
	{
		this->dir = 0;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		//this->entity_sprite.setTextureRect(sf::IntRect(96 * int(current_frame) + 6, 231, 89, 55));//sf::IntRect(int(current_frame) * this->sprite_w, 232, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
		this->entity_sprite.setTextureRect(
			sf::IntRect(
			Player_Texture::BASE_RECTLEFT_POS * int(current_frame) + Player_Texture::MVR_LEFT_STEP, 
			Player_Texture::MVR_TOP, 
			Player_Texture::MVR_WIDTH, 
			Player_Texture::MVR_HEIGHT
			));
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(this->_move_up))
	{
		this->dir = 3;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		//this->entity_sprite.setTextureRect(sf::IntRect(96 * int(current_frame) + 27, 305, 39, 89));//sf::IntRect(int(current_frame) * this->sprite_w, 307, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
		/*this->sprite_w = 45; this->sprite_h = 90;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * 50 + this->sprite_w, 0, this->sprite_w, this->sprite_h));// задаем тайлсет	/**/
		this->entity_sprite.setTextureRect(
			sf::IntRect(
			Player_Texture::BASE_RECTLEFT_POS * int(current_frame) + Player_Texture::MVU_LEFT_STEP, 
			Player_Texture::MVU_TOP, 
			Player_Texture::MVU_WIDTH, 
			Player_Texture::MVU_HEIGHT
			));
		moved = true;
	}
	else if (sf::Keyboard::isKeyPressed(this->_move_down))
	{
		this->dir = 2;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		//this->entity_sprite.setTextureRect(sf::IntRect(96 * int(current_frame) + 27, 6, 39, 89));//sf::IntRect(int(current_frame) * this->sprite_w, 0, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
		this->entity_sprite.setTextureRect(
			sf::IntRect(
			Player_Texture::BASE_RECTLEFT_POS * int(current_frame) + Player_Texture::MVD_LEFT_STEP, 
			Player_Texture::MVD_TOP, 
			Player_Texture::MVD_WIDTH, 
			Player_Texture::MVD_HEIGHT
			));
		moved = true;
	}

	this->update(game_speed);
	this->map_iteraction();

	//printf("%f\n", game_speed);

	return moved;
}

template<class T>
void Player<T>::map_iteraction()
{
	for (int i = this->y / MAP_TILE_SIZE; i < (this->y + entity_sprite.getTextureRect().height) / MAP_TILE_SIZE; i++)
	{
		for (int j = this->x / MAP_TILE_SIZE; j < (this->x + entity_sprite.getTextureRect().width) / MAP_TILE_SIZE; j++)
		{
			if (simple_map_structure[i][j] == MAP_CURB)
			{
				if (this->dy < 0)
				{
					this->y = i * MAP_TILE_SIZE + MAP_TILE_SIZE;
				}
				if (this->dy > 0)
				{
					this->y = i * MAP_TILE_SIZE - entity_sprite.getTextureRect().height;
				}
				if (this->dx < 0)
				{
					this->x = j * MAP_TILE_SIZE + MAP_TILE_SIZE;
				}
				if (this->dx > 0)
				{
					this->x = j * MAP_TILE_SIZE - entity_sprite.getTextureRect().width;
				}
			}
			if (simple_map_structure[i][j] == MAP_STONE)
			{
				beep_sound.play();
				this->game_score += 10;
				if (stone_count) stone_count--;
				simple_map_structure[i][j] = MAP_NOTHING;
			}
			if (simple_map_structure[i][j] == MAP_WILDFLOWER)
			{
				fail_sound.play();
				this->health -= 40;
				if (this->game_score) this->game_score -= 10;
				simple_map_structure[i][j] = MAP_NOTHING;
			}
			if (simple_map_structure[i][j] == MAP_HEATLHFLOWER)
			{
				if (this->health != FULL_HEALTH)
				{
					heal_sound.play();
					this->health += 20;
					if (this->health > FULL_HEALTH) this->health = FULL_HEALTH;
					simple_map_structure[i][j] = MAP_NOTHING;
				}
			}
		}
	}
}
#endif /* _PLAYER_ */