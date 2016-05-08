#ifndef G_CONFIG
#define G_CONFIG
#include <boost\property_tree\ini_parser.hpp>



class G_Config
{
	/* ============================================================================================================================= */
	static G_Config* _self;
	/* ============================================================================================================================= */
private:
	/* ============================================================================================================================= */
	short _default_mission;
	bool _fullscreen;
	float _pos_x;
	float _pos_y;
	int _height;
	int _width;
	int _time_div;
	int _bullet_speed;
	sf::Keyboard::Key _up, _down, _left, _right;
	/* ============================================================================================================================= */
	G_Config() {}
	/* ============================================================================================================================= */
public:
	/* ============================================================================================================================= */
	static G_Config* instance()
	{
		if(!_self) _self = new G_Config();
		return _self;
	}
	/* ============================================================================================================================= */
	inline short get_default_mission() const { return _default_mission; }
	inline bool is_fullscreen() const { return _fullscreen; }
	inline float get_pos_x() const { return _pos_x; }
	inline float get_pos_y() const { return _pos_y; }
	inline int get_bullet_speed() const { return _bullet_speed; }
	inline int get_width() const { return _width; }
	inline int get_height() const { return _height; }
	inline int get_time_div() const { return _time_div; }
	inline sf::Keyboard::Key get_key_up() { return _up; }
	inline sf::Keyboard::Key get_key_down() { return _down; }
	inline sf::Keyboard::Key get_key_left() { return _left; }
	inline sf::Keyboard::Key get_key_right() { return _right; }
	/* ============================================================================================================================= */
	void set_pos(float x, float y) { _pos_x = x; _pos_y = y; }
	void set_window_size(int w, int h) { _height = h; _width = w; }
	void take_config()
	{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini(SETTINGS_FILE, pt);

		_width = pt.get<int>("config.width", 600);
		_height = pt.get<int>("config.height", 800);
		

		_fullscreen = pt.get<bool>("config.fullscreen", false);

		_pos_x = pt.get<float>("config.pos_x", 80);
		_pos_y = pt.get<float>("config.pos_y", 80);

		_default_mission = pt.get<short>("config.default_mission", 0);	 
		
		_time_div = pt.get<int>("config.time_div", 800);
		
		_bullet_speed = pt.get<int>("config.bullet_speed", 400);//EnemyCFG::BULLET_SPEED);

		_up = (sf::Keyboard::Key)pt.get<int>("config.up", 22);
		_down = (sf::Keyboard::Key)pt.get<int>("config.down", 18);
		_left = (sf::Keyboard::Key)pt.get<int>("config.left", 0);
		_right = (sf::Keyboard::Key)pt.get<int>("config.right", 3);
	}
	/* ============================================================================================================================= */
};

G_Config* G_Config::_self=NULL;
#endif /* G_CONFIG */