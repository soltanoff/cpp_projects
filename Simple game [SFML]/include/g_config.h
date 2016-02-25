#ifndef G_CONFIG
#define G_CONFIG
#include <boost\property_tree\ini_parser.hpp>
const char SETTINGS_FILE[] = "settings.ini";



class G_Config
{
	static G_Config* _self;
private:
	short _default_mission;
	bool _fullscreen;
	double _pos_x;
	double _pos_y;
	int _height;
	int _width;
	int _time_div;
	G_Config() {}
public:
	static G_Config* instance()
	{
		if(!_self) _self = new G_Config();
		return _self;
	}
	
	inline short get_default_mission() { return _default_mission; }
	inline bool is_fullscreen() { return _fullscreen; }
	inline double get_pos_x() { return _pos_x; }
	inline double get_pos_y() { return _pos_y; }
	inline int get_width() { return _width; }
	inline int get_height() { return _height; }
	inline int get_time_div() { return _time_div; }
	
	void set_pos(double x, double y) { _pos_x = x; _pos_y = y; }
	void set_window_size(int w, int h) { _height = h; _width = w; }
	void take_config()
	{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini(SETTINGS_FILE, pt);

		_width = pt.get<int>("config.width", 600);
		_height = pt.get<int>("config.height", 800);
		

		_fullscreen = pt.get<bool>("config.fullscreen", true);

		_pos_x = pt.get<double>("config.pos_x", 80);
		_pos_y = pt.get<double>("config.pos_y", 80);

		_default_mission = pt.get<short>("config.default_mission", 0);	 
		
		_time_div = pt.get<int>("config.time_div", 800);
	}
};

G_Config* G_Config::_self=NULL;
#endif /* G_CONFIG */