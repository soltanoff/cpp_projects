#ifndef G_VIEW
#define G_VIEW
#include <SFML\Graphics.hpp>



extern sf::View view; // определяем объект вида, при помощи которого организуем слежку камеры за игроком

const short CAMERA_VIEW_X_OFFSET = 70;

void set_camera_view(float x, float y); // задаем слежку камеры за объектом находящемся по координатам Х и У

void view_map(float game_speed);

void view_control(float game_speed);
#endif /* G_VIEW */