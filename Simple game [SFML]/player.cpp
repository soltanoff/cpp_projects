#include "player.h"



void Player::move(float game_speed, float &current_frame, float obj_speed)
{
	if (sf::Keyboard::isKeyPressed(this->_move_left))// && this->.getPosition().x >= -1.5) 
	{
		this->sprite_h = 58;
		this->dir = 1;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * this->sprite_w, 136, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
	}
	if (sf::Keyboard::isKeyPressed(this->_move_right))// && hero_sprite.getPosition().x <= WINDOW_WIDTH - LION_POS) 
	{
		this->sprite_h = 58;
		this->dir = 0;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * this->sprite_w, 232, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
	}
	if (sf::Keyboard::isKeyPressed(this->_move_up))// && hero_sprite.getPosition().y >= -37.0) 
	{
		this->sprite_h = 96;
		this->dir = 3;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * this->sprite_w, 307, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
		/*this->sprite_w = 45; this->sprite_h = 90;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * 50 + this->sprite_w, 0, this->sprite_w, this->sprite_h));// задаем тайлсет	/**/
	}
	if (sf::Keyboard::isKeyPressed(this->_move_down))// && hero_sprite.getPosition().y <= WINDOW_HEIGHT- LION_POS) 
	{
		this->sprite_h = 96;
		this->dir = 2;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * this->sprite_w, 0, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
	}
	this->map_iteraction();
	this->update(game_speed);
}

void Player::map_iteraction()
{
	for (short i = this->y / MAP_TILE_SIZE; i < (this->y + this->sprite_h) / MAP_TILE_SIZE; i++)
	{
		for (short j = this->x / MAP_TILE_SIZE; j < (this->x + this->sprite_w) / MAP_TILE_SIZE; j++)
		{
			if (simple_map_structure[i][j] == '0')
			{
				if (this->dy < 0)
				{
					this->y = i * MAP_TILE_SIZE + MAP_TILE_SIZE;
				}
				if (this->dy > 0)
				{
					this->y = i * MAP_TILE_SIZE - this->sprite_h;
				}
				if (this->dx < 0)
				{
					this->x = j * MAP_TILE_SIZE + MAP_TILE_SIZE;
				}
				if (this->dx > 0)
				{
					this->x = j * MAP_TILE_SIZE - this->sprite_w;
				}
			}
			if (simple_map_structure[i][j] == 's')
			{
				beep_sound.play();
				simple_map_structure[i][j] = ' ';
				this->game_score += 10;
			}
		}
	}
}