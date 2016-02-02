#ifndef _PLAYER_
#define _PLAYER_
#include "g_character.h"



class Player : public G_Character
{
private:
	sf::Keyboard::Key _move_left;
	sf::Keyboard::Key _move_right;
	sf::Keyboard::Key _move_up;
	sf::Keyboard::Key _move_down;
public:
	Player(float X, float Y, float H, float W, std::string Txtr_File):
		G_Character(X, Y, H, W, Txtr_File), 
		_move_left(sf::Keyboard::A), _move_right(sf::Keyboard::D), 
		_move_up(sf::Keyboard::W), _move_down(sf::Keyboard::S)
	{
	}

	void move(float game_speed, float &current_frame, float obj_speed = 0.1);
	void set_move_control(sf::Keyboard::Key Left, sf::Keyboard::Key Right, sf::Keyboard::Key Up, sf::Keyboard::Key Down)
	{
		_move_left = Left;
		_move_right = Right; 
		_move_up = Up;
		_move_down = Down;
	}
};

#endif /* _PLAYER_ */