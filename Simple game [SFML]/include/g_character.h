#ifndef G_CHARACTER
#define G_CHARACTER
//#include "g_entity.h"



class G_Character //: public G_Entity
{
protected:
	/* ============================================================================================================================= */
	int health; // здоровье персонажа
	float dx; // ускорение по Ox
	float dy; // ускорение по Oy 
	float speed; // скорость
	short dir; // направление движения
	/* ============================================================================================================================= */
	//float x; // позиция по Ох
	//float y; // позиция по ОУ
	/* ============================================================================================================================= */
	sf::String texture_file; // путь к текстуре сущности
	/* ============================================================================================================================= */
	sf::Texture entity_texture; // текстура сущности
	sf::Sprite entity_sprite; // спрайт сущности
	/* ============================================================================================================================= */
	virtual inline void update(float game_speed) = 0;
	/* ============================================================================================================================= */
public:
	/* ============================================================================================================================= */
	G_Character(float X, float Y, float textLeft, float textTop, float W, float H, sf::Image &image):
		//G_Entity(X, Y, textLeft, textTop, W, H, image), 
		health(FULL_HEALTH), dx(0), dy(0), speed(0)//, x(X), y(Y)
	{
		this->entity_texture.loadFromImage(image);
		
		this->entity_sprite.setTexture(this->entity_texture);
		this->entity_sprite.setPosition(X, Y);
		this->entity_sprite.setTextureRect(sf::IntRect(int(textLeft), int(textTop), int(W), int(H)));
	}
	/* ============================================================================================================================= */
	inline int get_health() const { return this->health; }
	inline short get_dir() const { return this->dir; }
	inline float get_dx() const { return this->dx; }
	inline float get_dy() const { return this->dy; }
	inline float get_speed() const { return this->speed; }
	inline float get_x() const { return this->entity_sprite.getPosition().x; }
	inline float get_y() const { return this->entity_sprite.getPosition().y; }
	inline int get_width() const { return entity_sprite.getTextureRect().width; }
	inline int get_height() const { return entity_sprite.getTextureRect().height; }
	inline sf::Sprite get_sprite() const { return this->entity_sprite; }
	/* ============================================================================================================================= */
	inline void set_health(int Health) { this->health = Health; }
	inline void set_dir(short Dirrection) { this->dir = Dirrection; }
	inline void set_dx(float Acceleration) { this->dx = Acceleration; }
	inline void set_dy(float Acceleration) { this->dy = Acceleration; }
	inline void set_speed(float Speed) { this->speed = Speed; }
	inline void set_x(float X) { this->entity_sprite.setPosition(X, get_y()); }
	inline void set_y(float Y) { this->entity_sprite.setPosition(get_x(), Y); }
	/* ============================================================================================================================= */
	inline virtual void health_decr(int value) { this->health -= value; }
	inline virtual bool is_alive() 
	{ 
		if (this->health > 0) return true; 
		else 
		{
			if (this->health < 0) this->health = 0;
			return false;
		}
	}
	/* ============================================================================================================================= */
	virtual void action_time(sf::RenderWindow &window, float game_speed, bool no_iteraction, float &current_frame, G_Character &p) = 0; 
};
#endif /* G_CHARACTER */