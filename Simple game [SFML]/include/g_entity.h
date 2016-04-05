#ifndef G_ENTITY
#define G_ENTITY
#include <SFML/Graphics.hpp>



template<class T>
class G_Entity
{
protected:
	T x; // ������� �� ��
	T y; // ������� �� ��

	sf::String texture_file; // ���� � �������� ��������

	sf::Texture entity_texture; // �������� ��������
	sf::Sprite entity_sprite; // ������ ��������
public:
	G_Entity(T X, T Y, T textLeft, T textTop, T W, T H, sf::Image &image):
		x(X), y(Y)
	{
		this->entity_texture.loadFromImage(image);
		
		this->entity_sprite.setTexture(this->entity_texture);
		this->entity_sprite.setPosition(x, y);
		this->entity_sprite.setTextureRect(sf::IntRect(int(textLeft), int(textTop), int(W), int(H)));
	}

	float get_x() { return this->x; }
	float get_y() { return this->y; }
	sf::Sprite get_sprite() { return this->entity_sprite; }

	void set_x(float X) { this->x = X; }
	void set_y(float Y) { this->y = Y; }

	virtual void update(float game_speed) = 0;
};
#endif /* G_ENTITY */