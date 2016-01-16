/* ============  онтроль ввода, и прочее ============ */
#pragma once
#define _CRT_SECURE_NO_WARNINGS
/* ================== Memory leaks ================== */
#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif
/* =================== Ѕиблиотеки =================== */
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <Windows.h>
using namespace std;
/* ===================  онстанты ==================== */
const int CURRENT_YEAR = 2015; // текущий год (дл€ проверки на ввод)
const int NAME_DOC_SIZE = 25; // размер ‘»ќ врача
const int TAB_SIZE = 2000; // размер хэш-таблицы
const int SIZE_NAME = 100; // размер ‘»ќ пациента
const int SIZE_KEY = 10; // размер ключа хэш-таблицы(рег. номера пациента)
const int SIZE_ADRESS = 256; // размер пол€ адресса
const short RADIX = 10;  // система счислени€ (дл€ ф-ции _itoa)
const short COMMA_LIMIT = 2; // предел количества зап€тых в строке
const short SPACE_LIMIT = 4; // предел количества пробелов в строке
const short WORKDAY_SIZE = 7; // количество дней в неделе
const short NON_WEEK_DAY = 8; // не существующий день недели
const char *RUS_S = "јаЅб¬в√гƒд≈е®Є∆ж«з»и…й кЋлћмЌнќоѕп–р—с“т”у‘ф’х÷ц„чЎшўщЏъџы№ьЁэёюя€"; // алфавит русс.€з. (дл€ ф-ции toupperchar)
const char *_DOC_FILENAME = "doctors.dat"; // им€ файла, хран€щий информацию о докторах
const char *_PAT_FILENAME = "patients.dat"; // им€ файла, хран€щий информацию о пациентах
const char *_REF_FILENAME = "refferals.dat"; // им€ файла, хран€щий информацию о выписках(направлений)
const char *CNSL_COLS = "160"; // число(длина) столбцов консоли
const char *CNSL_LINES = "50"; // число(длина) строк консоли
/* ============= ‘ункци€ вывода таблицы ============= */
// вывод шапки таблицы
inline short print_head(string *name_col, short *size_col, const short COUNT_COL)
{
	short cmp_len(COUNT_COL - 5);
	for (int i = 0; i < COUNT_COL; i++)
		cmp_len += size_col[i];
	cmp_len += COUNT_COL;

	for (int i = 0; i < cmp_len; i++)
		cout << "=";
	cout << endl << "|";
	for (int i = 0; i < COUNT_COL; i++)
	{
		short space = short((size_col[i] - name_col[i].length()) / 2.0);
		for (int j = 0; j < space; j++)
			cout << " ";
		cout << name_col[i];
		for (int j = 0; j < space; j++)
			cout << " ";
		cout << "|";
	}
	cout << endl;
	for (int i = 0; i < cmp_len; i++)
		cout << "=";
	cout << endl;
	return cmp_len;
}
// вывод строки таблицы
template <class T>inline void print_row(T str, short size, short size_str)
{
	short space = short((size - size_str) / 2.0);
	int lost(2 * space + size_str);

	for (int j = 0; j < space; j++)
		cout << " ";
	cout << str;
	if (lost < size) space += size - lost;
	for (int j = 0; j < space; j++)
		cout << " ";
	cout << "|";
}
// вывод нижней границы таблицы
inline void print_end(short cmp_size)
{
	for (int i = 0; i < cmp_size; i++)
		cout << "=";
	cout << endl;
}
/* ================================================== */
// определ€ем количество цифр в числе
short int_size(int a) 
{
	char buff[SIZE_NAME];
	string str = _itoa(a, buff, RADIX);
	return short(str.length());
}
// первоначальные установки настроек консоли
void set_console_settings()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	//system ("mode con: cols=<number> lines=<number>");
	char *console_setting = new char [30];

	strcpy(console_setting, "mode con: cols=");
	strcat_s(console_setting, 30, CNSL_COLS);
	strcat_s(console_setting, 30, "lines=");
	strcat_s(console_setting, 30, CNSL_LINES);

	system(console_setting);
	delete [] console_setting;
}
// код на проверку утечек пам€ти
void is_memory_leaks() 
{
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );

	_CrtDumpMemoryLeaks();
	system("pause");
}
// иммитаци€ утечки пам€ти
void test_memory_leaks() 
{
	int **mass = new int *[9];
	for(int i=0; i<10; i++)
		mass[i] = new int [9];

	for(int i=0; i<9; i++)
	{
		for(int j=0; j<9; j++)
		{
			mass[i][j] = i+j;
			cout<<mass[i][j]<<'\t';
		}
		cout<<endl;
	}

	//чистка пам€ти
	
	/*for(int i=0; i<9; i++)
		delete [] mass[i];

	delete [] mass;*/
	
}
// проверка ключа на ошибки. ‘ормат ключа: MM-MMMMMM
bool key_control(string key) 
{
	if (key.length() == SIZE_KEY - 1)
	{
		for(int i = 0; i < SIZE_KEY - 1; i++)
		{
			if (i == 2) 
			{
				if (key[i] != '-') return true;
				continue;
			}

			if (!(key[i] <= 57 && key[i] >= 48))
				return true;
		}
	}
	else return true;

	return false;
}
// функци€ дл€ ввода ключа
void key_input(string &key) 
{
	while (true)
	{
		cin.sync();
		cin.clear();
		cin>>key;
		if (cin.fail() || key_control(key))
		{
			cout<<"ќшибка ввода данных! ѕовторите ввод: ";
			continue;
		}
		else break;
	}
}
// функци€ дл€ ввода ключа
void key_input(char *key) 
{
	while (true)
	{
		cin.sync();
		cin.clear();
		cin>>key;
		if (cin.fail() || key_control(key))
		{
			cout<<"ќшибка ввода данных! ѕовторите ввод: ";
			continue;
		}
		else break;
	}
}
// функци€ дл€ ввода чисел из интервала [min; max]
int input_control(int max = 5, int min = 0) 
{
	int a(0);
	while (true)
	{
		cin.sync();
		cin.clear();
		cin>>a;
		if (cin.fail() || !(a >= min && a <= max))
		{
			cout<<"ќшибка ввода данных! ѕовторите ввод: ";
			continue;
		}
		else break;
	}
	return a;
}
// функци€ дл€ ввода даты рождени€ пациента
void input_control(unsigned int *date) 
{
	while (true)
	{
		cin.sync();
		cin.clear();
		cout<<"\n ¬ведите дату рождени€:\n";
		cout<<"\t1. ƒень: ";
		cin>>date[0];
		cout<<"\t2. ћес€ц: ";
		cin>>date[1];
		cout<<"\t3. √од: ";
		cin>>date[2];
		if (cin.fail() || !(date[0] >= 1 && date[0] <= 31) || 
						  !(date[1] >= 1 && date[1] <= 12) ||
						  !(date[2] >= 1950 && date[2] <= CURRENT_YEAR))
		{
			cout<<"\nќшибка ввода данных!\n";
			continue;
		}
		else break;
	}
}
// проверка строки на сторонние символы
bool _have_any_symbols(string str)
{
	for(unsigned int i=0; i<str.length(); i++)
		if (!((str[i] == '.') || (str[i] == ' ') ||
			  (str[i] >= -64 && str[i] <= -1) || // русский верхнего регистра и нижнего регистра
			  (str[i] >=  65 && str[i] <= 90) || // латынь верхнего регистра
			  (str[i] >=  97 && str[i] <= 122))) // латынь нижнего регистра
			return true;
	return false;
}
// проверка строки на большое число зап€тых
bool is_many_comma(char *str)
{
	short count(0);
	for (unsigned int i = 0; i < strlen(str); i++)
		if ((str[i] == '.'))
			count++;
	if (count >  COMMA_LIMIT) return true;
	else return false;
}
// проверка строки на большое количество пробелов
bool is_many_space(char *str)
{
	short count(0);
	for (unsigned int i = 0; i < strlen(str); i++)
		if ((str[i] == ' '))
			count++;
	if (count >  SPACE_LIMIT) return true;
	else return false;
}
// функци€ ввода строки
void input_control(string &str)
{
	char temp[SIZE_NAME];

	while (true)
	{
		cin.sync();
		cin.clear();
		cin.getline(temp, SIZE_NAME, '\n');
		if (cin.fail() || _have_any_symbols(temp) || is_many_comma(temp) || is_many_space(temp))
		{
			cout << "ќшибка ввода данных! ѕовторите ввод: ";
			continue;
		}
		else break;
	}
	str = temp;
}
// функци€ ввода строки
void input_control(char *str)
{
	while (true)
	{
		cin.sync();
		cin.clear();
		cin.getline(str, SIZE_NAME, '\n');
		if (cin.fail() || _have_any_symbols(str) || is_many_comma(str) || is_many_space(str))
		{
			cout<<"ќшибка ввода данных! ѕовторите ввод: ";
			continue;
		}
		else break;
	}
}
// функци€ контрол€ ответа (да/нет) на 2-ный вопрос 
void answer_control(string &answer) 
{
	while(true)
	{
		cin>>answer;
		if ((answer.length() > 1 || (answer[0] != 'д' && answer[0] != 'н')))
		{
			cout<<"ќшибка ввода данных! ѕовторите ввод: ";
			continue;
		}
		else break;
	}
}
// возвращает символ верхнего регистра
char toupperchar(const char symbol) 
{
	bool flag(false);
	char result;//результат
	for(unsigned int i=0; i<=strlen(RUS_S); i++)
	{
		if (symbol == RUS_S[i]) 
		{
			flag=true;
			if (i % 2 != 0) 
				result=RUS_S[i-1];
			else 
				result=RUS_S[i];
		}
	}
	if (!flag)
		result=toupper(symbol);
	return result;
}
// возвращает строку с символами верхнего регистра
string toupperstr(const char *str) 
{
	string temp;
	for(unsigned int i = 0; str[i]!='\0'; i++)
		temp.push_back(toupperchar(str[i]));

	return temp;
}
// пр€мой алгоритм поиска слова в тексте
bool string_cmp(string word, string str)
{
	if (word.length() <= str.length()) // если размер слова меньше или равен размеру тескта, то
	{
		for (unsigned int i = 0; i < word.length(); i++) // то посимвольно сравниваем
		{
			if (toupperchar(str[i]) != toupperchar(word[i])) // сравниваем прописные символы
				return false; // если символы разные, то возвращаем false
		}
	}
	else return false; // если размер слова больше текста, то поиск невозможен
	return true;
}