#ifndef G_VIEW
#define G_VIEW
#include <SFML\Graphics.hpp>



extern sf::View view; // ���������� ������ ����, ��� ������ �������� ���������� ������ ������ �� �������

const short CAMERA_VIEW_X_OFFSET = 100;

void set_camera_view(float x, float y); // ������ ������ ������ �� �������� ����������� �� ����������� � � �

void view_map(float game_speed);

void view_control(float game_speed);
#endif /* G_VIEW */