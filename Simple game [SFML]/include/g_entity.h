#ifndef G_ENTITY
#define G_ENTITY
#include <SFML/Graphics.hpp>



template<class T>
class G_Entity
{
protected:
	T x; // позиция по Ох
	T y; // позиция по ОУ
	//float sprite_h; // высота спрайта
	//float sprite_w; // ширина спрайта
	sf::String texture_file; // путь к текстуре сущности

	sf::Image entity_image;
	sf::Texture entity_texture; // текстура сущности
	sf::Sprite entity_sprite; // спрайт сущности
public:
	G_Entity(T X, T Y, T textLeft, T textTop, T W, T H, sf::String Txtr_File):
		x(X), y(Y), /*sprite_h(H), sprite_w(W),*/ texture_file(Txtr_File)
	{
		this->entity_image.loadFromFile("Sprites/" + texture_file);
		this->entity_image.createMaskFromColor(sf::Color(41, 33, 59));

		this->entity_texture.loadFromImage(this->entity_image);
		
		this->entity_sprite.setTexture(this->entity_texture);
		this->entity_sprite.setPosition(x, y);
		this->entity_sprite.setTextureRect(sf::IntRect(int(textLeft), int(textTop), int(W), int(H)));
	}

	float get_x() { return this->x; }
	float get_y() { return this->y; }
	sf::Sprite get_sprite() { return this->entity_sprite; }

	void set_x(float X) { this->x = X; }
	void set_y(float Y) { this->y = Y; }
};
#endif /* G_ENTITY */