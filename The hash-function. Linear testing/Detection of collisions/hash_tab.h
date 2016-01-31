#ifndef HASH_TAB
#define HASH_TAB

#include <string>
#include <math.h>
#include <fstream>
/*
const int SIZE_KEY = 10;
const int TAB_SIZE = 2000;
*/
class hash_tab
{
private:
	struct hash_struct
	{
		unsigned int collisions_count;
		std::string hash_key;
		bool is_empty;
	}*tab;
	
	const int SIZE_KEY;
	const int TAB_SIZE;

	int hash_one(const int key) { return (key % TAB_SIZE); }
	int hash_two(const int key) { return ((key % (TAB_SIZE - 1)) + 1); }

public:
	hash_tab();

	int h(const char *key);

	std::string get_key(const int index) const { return tab[index].hash_key; }
	unsigned int get_collisions_count(const int index) const { return tab[index].collisions_count; } 
	bool get_empty_status(const int index) const { return tab[index].is_empty; }
	int get_max_tab_size() const { return TAB_SIZE; }
	/*
	bool set_key(const int index, const char* key);
	bool set_collisions_count(const int index, const unsigned int count);
	bool set_empty_status(const int index, const bool status);
	/**/
	bool find_htab(const char *key, int &index);
	bool add_to_tab(const char *key);

	void random_keys(const int rand_tab_size);
	void get_collisions_test(const char *TXT_FILENAME);

	bool del_htab(const char *key);
	void del_all_htab();

	void readfromtxt(const char *TAB_FILENAME);
	void write2txt(const char *TAB_FILENAME) const;
	~hash_tab();
};

#endif