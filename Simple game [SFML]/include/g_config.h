#ifndef G_CONFIG
#define G_CONFIG
#include <boost\property_tree\ini_parser.hpp>
const char SETTINGS_FILE[] = "settings.ini";



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
	inline int get_width() const { return _width; }
	inline int get_height() const { return _height; }
	inline int get_time_div() const { return _time_div; }
	/* ============================================================================================================================= */
	void set_pos(float x, float y) { _pos_x = x; _pos_y = y; }
	void set_window_size(int w, int h) { _height = h; _width = w; }
	void take_config()
	{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini(SETTINGS_FILE, pt);

		_width = pt.get<int>("config.width", 600);
		_height = pt.get<int>("config.height", 800);
		

		_fullscreen = pt.get<bool>("config.fullscreen", true);

		_pos_x = pt.get<float>("config.pos_x", 80);
		_pos_y = pt.get<float>("config.pos_y", 80);

		_default_mission = pt.get<short>("config.default_mission", 0);	 
		
		_time_div = pt.get<int>("config.time_div", 800);
	}
	/* ============================================================================================================================= */
};

G_Config* G_Config::_self=NULL;
#endif /* G_CONFIG */