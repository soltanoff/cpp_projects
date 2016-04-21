#ifndef G_ENTITY
#define G_ENTITY
#include <SFML/Graphics.hpp>


class G_Entity
{
protected:
	float x; // позиция по Ох
	float y; // позиция по ОУ

	sf::String texture_file; // путь к текстуре сущности

	sf::Texture entity_texture; // текстура сущности
	sf::Sprite entity_sprite; // спрайт сущности
public:
	G_Entity(float X, float Y, float textLeft, float textTop, float W, float H, sf::Image &image):
		x(X), y(Y)
	{
		this->entity_texture.loadFromImage(image);
		
		this->entity_sprite.setTexture(this->entity_texture);
		this->entity_sprite.setPosition(x, y);
		this->entity_sprite.setTextureRect(sf::IntRect(int(textLeft), int(textTop), int(W), int(H)));
	}

	float get_x() { return this->x; }
	float get_y() { return this->y; }
	int get_width() { return entity_sprite.getTextureRect().width; }
	int get_height() { return entity_sprite.getTextureRect().height; }
	sf::Sprite get_sprite() { return this->entity_sprite; }

	void set_x(float X) { this->x = X; }
	void set_y(float Y) { this->y = Y; }

	virtual void update(float game_speed) = 0;
};
#endif /* G_ENTITY */