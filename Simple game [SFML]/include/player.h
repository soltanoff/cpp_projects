#ifndef _PLAYER_
#define _PLAYER_
#include "main_settings.h"
#include "g_character.h"
#include "simple_map.h"
#include "sounds.h"
#include "missions.h"



class Player : public G_Character
{
private:
	/* ============================================================================================================================= */
	sf::Keyboard::Key _move_left;
	sf::Keyboard::Key _move_right;
	sf::Keyboard::Key _move_up;
	sf::Keyboard::Key _move_down;
	/* ============================================================================================================================= */
	int game_score; 
	void map_iteraction();
	Sounds game_sound;
	/* ============================================================================================================================= */
	inline void update(float game_speed);
	bool move(float game_speed, float &current_frame, float obj_speed = 0.1);
	/* ============================================================================================================================= */
public:
	/* ============================================================================================================================= */
	Player(float X, float Y, sf::Image &image):
		G_Character(X, Y, 
			Player_Texture::MVL_LEFT_STEP, 
			Player_Texture::MVL_TOP, 
			Player_Texture::MVL_WIDTH, 
			Player_Texture::MVL_HEIGHT, image
		), game_score(0),
		_move_left(sf::Keyboard::A), _move_right(sf::Keyboard::D), 
		_move_up(sf::Keyboard::W), _move_down(sf::Keyboard::S)
	{
		//game_sound.sounds_settings(); // настраиваем звук
	}
	/* ============================================================================================================================= */
	inline void health_decr(int value);
	inline bool is_alive();
	inline int get_score() const { return this->game_score; }
	/* ============================================================================================================================= */
	inline void get_move_control(sf::String &Left, sf::String &Right, sf::String &Up, sf::String &Down);
	inline void get_move_control(sf::Keyboard::Key &Left, sf::Keyboard::Key &Right, sf::Keyboard::Key &Up, sf::Keyboard::Key &Down);
	inline void set_move_control(sf::Keyboard::Key Left, sf::Keyboard::Key Right, sf::Keyboard::Key Up, sf::Keyboard::Key Down);
	inline void set_score(int score) { this->game_score = score; }
	/* ============================================================================================================================= */
	void action_time(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p);
	/* ============================================================================================================================= */
};

sf::String get_code(sf::Keyboard::Key key)
{
	if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
		return (char)(key + 65);
	if (key == sf::Keyboard::Up)
		return "Up";
	if (key == sf::Keyboard::Down)
		return "Down";
	if (key == sf::Keyboard::Left)
		return "Left";
	if (key == sf::Keyboard::Right)
		return "Right";
	return "None";
}

void Player::get_move_control(sf::String &Left, sf::String &Right, sf::String &Up, sf::String &Down)
{
	Left = get_code(_move_left);
	Right = get_code(_move_right); 
	Up = get_code(_move_up);
	Down = get_code(_move_down);
}

void Player::get_move_control(sf::Keyboard::Key &Left, sf::Keyboard::Key &Right, sf::Keyboard::Key &Up, sf::Keyboard::Key &Down)
{
	Left = _move_left;
	Right = _move_right; 
	Up = _move_up;
	Down = _move_down;
}

void Player::set_move_control(sf::Keyboard::Key Left, sf::Keyboard::Key Right, sf::Keyboard::Key Up, sf::Keyboard::Key Down)
{
	_move_left = Left;
	_move_right = Right; 
	_move_up = Up;
	_move_down = Down;
}

bool Player::is_alive() 
{ 
	if (this->health > 0) 
	{
		this->entity_sprite.setColor(sf::Color(255, 255, 255));
		return true; 
	}
	else 
	{
		if (this->health < 0) this->health = 0;
		this->entity_sprite.setColor(sf::Color(0, 0, 0));
		return false;
	}
}

inline void Player::health_decr(int value) 
{
	if (this->is_alive())
	{
		this->game_sound.play_fail(); 
		this->health -= value; 
	}
}

void Player::update(float game_speed)
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

	//this->x += dx * game_speed;
	//this->y += dy * game_speed;
	if (this->game_score < 0)
		this->game_score = 0;

	this->speed = 0;
	this->entity_sprite.setPosition(
		this->get_x() + dx * game_speed, 
		this->get_y() + dy * game_speed
		);
}

void Player::action_time(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p)
{
	if (is_alive() && !no_iteraction)
	{
		//if (!__is_view_map) 
		//{
			move(game_speed, current_frame, (float)0.1);
			//set_camera_view(p.get_x(), p.get_y()); // задаем слежку камеры за игроком
		//}
		//else
			//view_map(__game_speed); // активация просмотра карты
	}
	window.draw(this->get_sprite());
}

bool Player::move(float game_speed, float &current_frame, float obj_speed)
{
	bool moved = false;
	if (sf::Keyboard::isKeyPressed(this->_move_left)) 
	{
		this->dir = 1;
		this->speed = obj_speed;
		current_frame += (float)(0.005 * game_speed);
		if (current_frame > 3) current_frame -= 3;
		this->entity_sprite.setTextureRect(
			sf::IntRect(
			Player_Texture::BASE_RECTLEFT_POS * int(current_frame) + Player_Texture::MVL_LEFT_STEP, 
			Player_Texture::MVL_TOP, 
			Player_Texture::MVL_WIDTH, 
			Player_Texture::MVL_HEIGHT
			));//(sf::IntRect(int(current_frame) * this->sprite_w, 136, 1 * this->sprite_w, 1 * this->sprite_h));// задаем тайлсет
		moved = true;
	}
	else if (sf::Keyboard::isKeyPressed(this->_move_right))
	{
		this->dir = 0;
		this->speed = obj_speed;
		current_frame += (float)(0.005 * game_speed);
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
		current_frame += (float)(0.005 * game_speed);
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
		current_frame += (float)(0.005 * game_speed);
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
	/*
	if (moved)
	{
		printf("[Player] (%f, \t%f)\n", this->get_x(), this->get_y());
	}
	*/
	this->update(game_speed);
	this->map_iteraction();

	//printf("%f\n", game_speed);

	return moved;
}

void Player::map_iteraction()
{
	for (int i = (int)(this->get_y() / MapCFG::MAP_TILE_SIZE); i < (this->get_y() + entity_sprite.getTextureRect().height) / MapCFG::MAP_TILE_SIZE; i++)
	{
		for (int j = (int)(this->get_x() / MapCFG::MAP_TILE_SIZE); j < (this->get_x() + entity_sprite.getTextureRect().width) / MapCFG::MAP_TILE_SIZE; j++)
		{
			if (Map::get_map()[i][j] == MapCFG::MAP_CURB)
			{
				if (this->dy < 0)
				{
					this->set_y((float)(i * MapCFG::MAP_TILE_SIZE + MapCFG::MAP_TILE_SIZE));
				}
				if (this->dy > 0)
				{
					this->set_y((float)(i * MapCFG::MAP_TILE_SIZE - entity_sprite.getTextureRect().height));
				}
				if (this->dx < 0)
				{
					this->set_x((float)(j * MapCFG::MAP_TILE_SIZE + MapCFG::MAP_TILE_SIZE));
				}
				if (this->dx > 0)
				{
					this->set_x((float)(j * MapCFG::MAP_TILE_SIZE - entity_sprite.getTextureRect().width));
				}
			}
			if (/*simple_map_structure[i][j] */ Map::get_map()[i][j] == MapCFG::MAP_STONE)
			{
				//beep_sound.play();
				game_sound.play_beep();
				this->game_score += PlayerCFG::STONE_SCOREINCR;
				if (Mission::get_stone_count()) Mission::stone_count_decr();
				Map::get_map()[i][j] = MapCFG::MAP_NOTHING;
			}
			if (Map::get_map()[i][j] == MapCFG::MAP_WILDFLOWER)
			{
				//fail_sound.play();
				//game_sound.play_fail();
				this->health_decr(PlayerCFG::WILDFLOWER_DAMAGE);
				if (this->game_score) this->game_score -= PlayerCFG::WILDFLOWER_SCOREDECR;
				Map::get_map()[i][j] = MapCFG::MAP_NOTHING;
			}
			if (Map::get_map()[i][j] == MapCFG::MAP_HEATLHFLOWER)
			{
				if (this->health != FULL_HEALTH)
				{
					//heal_sound.play();
					game_sound.play_heal();
					this->health += PlayerCFG::HEATLHFLOWER_HEAL;
					if (this->health > FULL_HEALTH) this->health = FULL_HEALTH;
					Map::get_map()[i][j] = MapCFG::MAP_NOTHING;
				}
			}
		}
	}
}
#endif /* _PLAYER_ */