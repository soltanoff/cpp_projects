#include "player.h"



void Player::move(float game_speed, float &current_frame, float obj_speed)
{
	if (sf::Keyboard::isKeyPressed(this->_move_left))// && this->.getPosition().x >= -1.5) 
	{
		this->dir = 1;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * this->sprite_w, 1 * this->sprite_h, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
	}
	if (sf::Keyboard::isKeyPressed(this->_move_right))// && hero_sprite.getPosition().x <= WINDOW_WIDTH - LION_POS) 
	{
		this->dir = 0;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * this->sprite_w, 2 * this->sprite_h, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
	}
	if (sf::Keyboard::isKeyPressed(this->_move_up))// && hero_sprite.getPosition().y >= -37.0) 
	{
		this->dir = 3;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * this->sprite_w, 3 * this->sprite_h, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
			
	}
	if (sf::Keyboard::isKeyPressed(this->_move_down))// && hero_sprite.getPosition().y <= WINDOW_HEIGHT- LION_POS) 
	{
		this->dir = 2;
		this->speed = obj_speed;
		current_frame += 0.005 * game_speed;
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(sf::IntRect(int(current_frame) * this->sprite_w, 0 * this->sprite_h, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
	}

	this->update(game_speed);
}