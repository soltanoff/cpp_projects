#include "hash_tab.h"


hash_tab::hash_tab(): SIZE_KEY(10), TAB_SIZE(2000)
{
	tab = new hash_struct[TAB_SIZE];
	for(int i = 0; i < TAB_SIZE; i++)
	{
		tab[i].collisions_count = 0; 
		tab[i].hash_key = "0";
		tab[i].is_empty = true;
	}
}

hash_tab::~hash_tab()
{
	del_all_htab();
	delete [] tab;
}
/*
bool hash_tab::set_key(const int index, const char* key)
{  
	if (index < TAB_SIZE) 
	{
		tab[index].hash_key = key;
		return true;
	}
	else
		return false;
}

bool hash_tab::set_collisions_count(const int index, const unsigned int count)
{  
	if (index < TAB_SIZE) 
	{
		tab[index].collisions_count = count;
		return true;
	}
	else
		return false;
}

bool hash_tab::set_empty_status(const int index, const bool status)
{  
	if (index < TAB_SIZE) 
	{
		tab[index].is_empty = status;
		return true;
	}
	else
		return false;
}
/**/
int hash_tab::h(const char *key)
{
	int result(0);
	int index(0);

	for(int i=0; i < SIZE_KEY - 1; i++)
		result += int(key[i])*int(pow(3.0, i));
	// mod - %

	int h_1 = hash_one(result),
		h_2 = hash_two(result);

	index = h_1;


	for(int i=0; i < TAB_SIZE; i++)
	{
		index = (h_1 + i*h_2) % TAB_SIZE;
		if (index > TAB_SIZE || index < 0) break;
		if (tab[index].is_empty)
		{
			return index;
			/*tab[index].hash_key = key;
			tab[index].is_empty = false;
			break;*/
		}
		else tab[index].collisions_count++;
	}
	return -1;
}

bool hash_tab::find_htab(const char *key, int &index)
{
	/*int i(0);

	for(int i = 0; i < TAB_SIZE; i++)
	{
		index = h(key);
		if (!strcmp(tab[index].hash_key.c_str(), key)) break;
	}

	if (!strcmp(tab[index].koliz.c_str(), key)) return true; 
	else index = 0; return false; */
	int result(0);

	for(int i=0; i < SIZE_KEY - 1; i++)
		result += int(key[i])*int(pow(3.0, i));
	// mod - %

	int h_1 = hash_one(result),
		h_2 = hash_two(result);

	index = h_1;

	int i(0);
	bool str_compare(false);

	do
	{
		index = (h_1 + i*h_2) % TAB_SIZE;
		if (index > TAB_SIZE || index < 0) return false;
		if (!strcmp(tab[index].hash_key.c_str(), key)) { str_compare = true; break;}
		i++;
	}
	while(!tab[index].is_empty);

	if (str_compare) return true;
	else return false;
}

bool hash_tab::add_to_tab(const char *key)
{
	int index(0); // индекс ключа

	index = h(key);

	if (index >= 0)
	{
		tab[index].hash_key = key;
		tab[index].is_empty = false;
		return true;
	}
	else return false;
}

void hash_tab::random_keys(const int rand_tab_size)
{
	int index(0);
	char *key = new char[SIZE_KEY];

	for(int i=0; i < rand_tab_size; i++)
	{
		for(int j=0; j < SIZE_KEY - 1; j++)
		{
			if (j == 2)
				key[j] = '-';
			else 
				key[j] = char(rand() % 9 + 48);
		}
		key[9] = '\0';
		add_to_tab(key);
	}
	delete [] key;
}

bool hash_tab::del_htab(const char *key)
{
	int index(0);//индекс ключа

	if (find_htab(key, index)) //если ключ, который мы хотим удалить найден, то удаляем)
	{
		tab[index].collisions_count = 0;
		tab[index].is_empty = true;
		tab[index].hash_key = "0";
		return true;
	}
	else return false;
}

void hash_tab::del_all_htab()
{
	for(int i=0; i < TAB_SIZE; i++)
	{
		tab[i].collisions_count = 0;
		tab[i].is_empty = true;
		tab[i].hash_key = "0";
	}
}

void hash_tab::readfromtxt(const char *TAB_FILENAME)
{
	std::ifstream fin;
	fin.open(TAB_FILENAME);
	if (fin)
	{
		for(int i=0; i < TAB_SIZE; i++)
		{
			fin >> tab[i].hash_key;
			fin >> tab[i].collisions_count;
			if (tab[i].hash_key.length() < SIZE_KEY) tab[i].is_empty = true;
			else tab[i].is_empty = false;
		}
	}
	fin.close();
}

void hash_tab::write2txt(const char *TAB_FILENAME) const
{
	std::ofstream fout;
	fout.open(TAB_FILENAME);

	if (fout)
	{
		for(int i=0; i < TAB_SIZE; i++)
		{
			fout << tab[i].hash_key;
			fout << " ";
			fout << tab[i].collisions_count;
			fout << "\n";
		}
	}
	fout.close();
}

void hash_tab::get_collisions_test(const char *TXT_FILENAME)
{
	random_keys(3*TAB_SIZE);

	std::ofstream fout;
	fout.open(TXT_FILENAME);

	if (fout)
	{
		for(int i=0; i < TAB_SIZE; i++)
		{
			fout << tab[i].collisions_count;
			fout << "\n";
		}
	}
	fout.close();
}