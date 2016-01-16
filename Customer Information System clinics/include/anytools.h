/* ============ �������� �����, � ������ ============ */
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
/* =================== ���������� =================== */
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <Windows.h>
using namespace std;
/* =================== ��������� ==================== */
const int CURRENT_YEAR = 2015; // ������� ��� (��� �������� �� ����)
const int NAME_DOC_SIZE = 25; // ������ ��� �����
const int TAB_SIZE = 2000; // ������ ���-�������
const int SIZE_NAME = 100; // ������ ��� ��������
const int SIZE_KEY = 10; // ������ ����� ���-�������(���. ������ ��������)
const int SIZE_ADRESS = 256; // ������ ���� �������
const short RADIX = 10;  // ������� ��������� (��� �-��� _itoa)
const short COMMA_LIMIT = 2; // ������ ���������� ������� � ������
const short SPACE_LIMIT = 4; // ������ ���������� �������� � ������
const short WORKDAY_SIZE = 7; // ���������� ���� � ������
const short NON_WEEK_DAY = 8; // �� ������������ ���� ������
const char *RUS_S = "�����������娸����������������������������������������������������"; // ������� ����.��. (��� �-��� toupperchar)
const char *_DOC_FILENAME = "doctors.dat"; // ��� �����, �������� ���������� � ��������
const char *_PAT_FILENAME = "patients.dat"; // ��� �����, �������� ���������� � ���������
const char *_REF_FILENAME = "refferals.dat"; // ��� �����, �������� ���������� � ��������(�����������)
const char *CNSL_COLS = "160"; // �����(�����) �������� �������
const char *CNSL_LINES = "50"; // �����(�����) ����� �������
/* ============= ������� ������ ������� ============= */
// ����� ����� �������
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
// ����� ������ �������
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
// ����� ������ ������� �������
inline void print_end(short cmp_size)
{
	for (int i = 0; i < cmp_size; i++)
		cout << "=";
	cout << endl;
}
/* ================================================== */
// ���������� ���������� ���� � �����
short int_size(int a) 
{
	char buff[SIZE_NAME];
	string str = _itoa(a, buff, RADIX);
	return short(str.length());
}
// �������������� ��������� �������� �������
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
// ��� �� �������� ������ ������
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
// ��������� ������ ������
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

	//������ ������
	
	/*for(int i=0; i<9; i++)
		delete [] mass[i];

	delete [] mass;*/
	
}
// �������� ����� �� ������. ������ �����: MM-MMMMMM
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
// ������� ��� ����� �����
void key_input(string &key) 
{
	while (true)
	{
		cin.sync();
		cin.clear();
		cin>>key;
		if (cin.fail() || key_control(key))
		{
			cout<<"������ ����� ������! ��������� ����: ";
			continue;
		}
		else break;
	}
}
// ������� ��� ����� �����
void key_input(char *key) 
{
	while (true)
	{
		cin.sync();
		cin.clear();
		cin>>key;
		if (cin.fail() || key_control(key))
		{
			cout<<"������ ����� ������! ��������� ����: ";
			continue;
		}
		else break;
	}
}
// ������� ��� ����� ����� �� ��������� [min; max]
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
			cout<<"������ ����� ������! ��������� ����: ";
			continue;
		}
		else break;
	}
	return a;
}
// ������� ��� ����� ���� �������� ��������
void input_control(unsigned int *date) 
{
	while (true)
	{
		cin.sync();
		cin.clear();
		cout<<"\n ������� ���� ��������:\n";
		cout<<"\t1. ����: ";
		cin>>date[0];
		cout<<"\t2. �����: ";
		cin>>date[1];
		cout<<"\t3. ���: ";
		cin>>date[2];
		if (cin.fail() || !(date[0] >= 1 && date[0] <= 31) || 
						  !(date[1] >= 1 && date[1] <= 12) ||
						  !(date[2] >= 1950 && date[2] <= CURRENT_YEAR))
		{
			cout<<"\n������ ����� ������!\n";
			continue;
		}
		else break;
	}
}
// �������� ������ �� ��������� �������
bool _have_any_symbols(string str)
{
	for(unsigned int i=0; i<str.length(); i++)
		if (!((str[i] == '.') || (str[i] == ' ') ||
			  (str[i] >= -64 && str[i] <= -1) || // ������� �������� �������� � ������� ��������
			  (str[i] >=  65 && str[i] <= 90) || // ������ �������� ��������
			  (str[i] >=  97 && str[i] <= 122))) // ������ ������� ��������
			return true;
	return false;
}
// �������� ������ �� ������� ����� �������
bool is_many_comma(char *str)
{
	short count(0);
	for (unsigned int i = 0; i < strlen(str); i++)
		if ((str[i] == '.'))
			count++;
	if (count >  COMMA_LIMIT) return true;
	else return false;
}
// �������� ������ �� ������� ���������� ��������
bool is_many_space(char *str)
{
	short count(0);
	for (unsigned int i = 0; i < strlen(str); i++)
		if ((str[i] == ' '))
			count++;
	if (count >  SPACE_LIMIT) return true;
	else return false;
}
// ������� ����� ������
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
			cout << "������ ����� ������! ��������� ����: ";
			continue;
		}
		else break;
	}
	str = temp;
}
// ������� ����� ������
void input_control(char *str)
{
	while (true)
	{
		cin.sync();
		cin.clear();
		cin.getline(str, SIZE_NAME, '\n');
		if (cin.fail() || _have_any_symbols(str) || is_many_comma(str) || is_many_space(str))
		{
			cout<<"������ ����� ������! ��������� ����: ";
			continue;
		}
		else break;
	}
}
// ������� �������� ������ (��/���) �� 2-��� ������ 
void answer_control(string &answer) 
{
	while(true)
	{
		cin>>answer;
		if ((answer.length() > 1 || (answer[0] != '�' && answer[0] != '�')))
		{
			cout<<"������ ����� ������! ��������� ����: ";
			continue;
		}
		else break;
	}
}
// ���������� ������ �������� ��������
char toupperchar(const char symbol) 
{
	bool flag(false);
	char result;//���������
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
// ���������� ������ � ��������� �������� ��������
string toupperstr(const char *str) 
{
	string temp;
	for(unsigned int i = 0; str[i]!='\0'; i++)
		temp.push_back(toupperchar(str[i]));

	return temp;
}
// ������ �������� ������ ����� � ������
bool string_cmp(string word, string str)
{
	if (word.length() <= str.length()) // ���� ������ ����� ������ ��� ����� ������� ������, ��
	{
		for (unsigned int i = 0; i < word.length(); i++) // �� ����������� ����������
		{
			if (toupperchar(str[i]) != toupperchar(word[i])) // ���������� ��������� �������
				return false; // ���� ������� ������, �� ���������� false
		}
	}
	else return false; // ���� ������ ����� ������ ������, �� ����� ����������
	return true;
}