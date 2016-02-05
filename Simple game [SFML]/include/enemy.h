#ifndef G_ENEMY
#define G_ENEMY
#include "g_character.h"
#include "player.h"
#include "textures_settings.h"
#include <math.h>



template<class T>
class Enemy : public G_Character<T>
{
private:
	bool enemy_spoted;
	sf::String name;
	void map_iteraction() {}
public:
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
		this->entity_sprite.setOrigin(this->entity_sprite.getTextureRect().width / 2.0, this->entity_sprite.getTextureRect().height / 2.0);
	}

	void search_enemy(Player<T> &enemy)
	{	
		T dX = - this->x + enemy.get_x(); // вектор , колинеарный прямой, которая пересекает спрайт и курсор
		T dY = - this->y + enemy.get_y(); // он же, координата y

		float rotation = (atan2(dY, dX)) * 180 / 3.14159265; // получаем угол в радианах и переводим его в градусы
		printf("roptation: %f \n", rotation); // смотрим на градусы в консольке
		this->entity_sprite.setRotation(rotation); // поворачиваем спрайт на эти градусы		
	}
	void update(float game_speed)
	{

	}
};
#endif /* G_ENEMY */