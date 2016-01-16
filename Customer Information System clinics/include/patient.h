/* ====== ���-������� (������� �����������). ������� ====== */
#pragma once
#include "anytools.h"

int _pat_count = 1; // ������� ���������

/* =================== ��������� ������ =================== */
// ��������� ������ ��������
struct patient
{
	string key;// ������ �����: MM-NNNNNN
	string name; // ��� 
	unsigned int bday[3]; // ���� ��������
	string adress; // �����
	string workplace; // ����� ������
};
// ��������� ������ ���-�������
struct hash_tab
{
	patient pat; // ��������� ������ �������� 
	bool is_empty; // ����� �� ������?
};
// ������ ��������� ������, ���������� �������� ������
typedef vector<struct hash_tab*> svector_pat;
/* ================= ������� ����������� ================== */
// ������ �����������
int hash_one(int key) { return (key % TAB_SIZE); }
// ������ �����������
int hash_two(int key) { return ((key % (TAB_SIZE - 1)) + 1); }
// ������� ����������� ����������� (� 1, � 2)
int h(hash_tab *tab, string key)
{
	int result(0); // ��������� ������������ ����� �������� �����
	int index(0); // ������ ���-�������
	for(int i=0; i < SIZE_KEY - 1; i++)
		result += int(key[i])*int(pow(3.0, i));
	int h_1 = hash_one(result),
		h_2 = hash_two(result);
	// ��������� ��������� ������ �� ������ ������� �����������
	//index = h_1;
	for(int i=0; i < TAB_SIZE; i++)
	{
		// ��������� ������ �� ������ ������� ������
		index = (h_1 + i*h_2) % TAB_SIZE;
		// ���� ������ "������" ��� � ������� �������, ��� � � �������, ��
		if (index > TAB_SIZE || index < 0) break;
		if (tab[index].is_empty)
			return index;
	}
	return -1;
}
// ��������� ������������� ����� �������� hash_tab � patient
void htab_init(hash_tab *tab)
{
	for(int i = 0; i < TAB_SIZE; i++)
	{
		tab[i].pat.key = "0";
		tab[i].pat.name = "0";
		tab[i].pat.bday[0] = 0;
		tab[i].pat.bday[1] = 0;
		tab[i].pat.bday[2] = 0;
		tab[i].pat.adress = "0";
		tab[i].pat.workplace = "0";
		tab[i].is_empty = true;
	}
}
// ����� �������� �� �����
// �������� ���� � �������, ��� ��� ���������� �������� ��������
bool find_htab(hash_tab *tab, string key, int &index)
{
	int result(0); // ��������� ������������ ����� �������� �����
	for(int i=0; i < SIZE_KEY - 1; i++)
		result += int(key[i])*int(pow(3.0, i));
	int h_1 = hash_one(result),
		h_2 = hash_two(result);
	// ��������� ��������� ������ �� ������ ������� �����������
	//index = h_1;
	int i(0); 
	bool str_compare(false);
	do
	{
		index = (h_1 + i*h_2) % TAB_SIZE;
		if (index > TAB_SIZE || index < 0) return false;
		// ������ ��������: ������ �� ���� �������� � ������ � ��������� patient
		// ������� ������, ������� �������� �����
		if (!strcmp(tab[index].pat.key.c_str(), key.c_str())) 
		{ 
			str_compare = true; // ���� �������, �� �� �����, ��� ������
			break;
		}
		i++;
	}
	while(!tab[index].is_empty);
	if (str_compare) return true;
	else return false;
}
// ���������� ������ � �������
bool add_to_tab(hash_tab *tab, patient pat)
{
	int index(0); // ������ ������ �������
	index = h(tab, pat.key);  // ��������� ������� ����� 2-�� �����������
	// ���� ���������� �������, �� �������� � ��������, ��� ������ ��������(return true)
	if (index >= 0)
	{
		tab[index].pat.key = pat.key;
		tab[index].pat.name = pat.name;
		tab[index].pat.bday[0] = pat.bday[0];
		tab[index].pat.bday[1] = pat.bday[1];
		tab[index].pat.bday[2] = pat.bday[2];
		tab[index].pat.adress = pat.adress;
		tab[index].pat.workplace = pat.workplace;
		tab[index].is_empty = false;
		return true;
	}
	else return false; // ����� �������, ��� ������ �� ���������
}
// �������� ������ �� ������� �� �����
bool del_htab(hash_tab *tab, string key)
{
	int index(0);//������ �����
	if (find_htab(tab, key, index)) //���� ����, ������� �� ����� ������� ������, �� �������)
	{
		tab[index].pat.key = "0";
		tab[index].pat.name = "0";
		tab[index].pat.bday[0] = 0;
		tab[index].pat.bday[1] = 0;
		tab[index].pat.bday[2] = 0;
		tab[index].pat.adress = "0";
		tab[index].pat.workplace = "0";
		tab[index].is_empty = true;
		return true; // �������� �� ������� ��������
	}
	else return false; // ���� ������ �� �������, �� �������� �� �� ������� ��������
}
// �������� ������� �� ������� ���� �� ����� ������
bool have_pat(hash_tab *tab)
{
	for (int i = 0; i < TAB_SIZE; i++)
	{
		if (tab[i].is_empty) continue;
		else return true;
	}
	return false;
}
/* =================== ������ � ������ =================== */
// ��������� ������ �� ����� � ��������� ��� ���-�������
bool read_pat_base(hash_tab *tab)
{
	bool fail(false); // ���� ��� �������� �� ������� ������ � �����
	ifstream fin;
	fin.open(_PAT_FILENAME);
	// ������ �������� ������(������ ������ �������) � ������� � ���������
	// ���������� ���� ���������, ������� ����������� ��������� � �����
	if (fin)
	{
		// ������� �� ���������� �������� � �����
		fin.seekg(0, ios_base::end); // ��������� � �����
		int i = int(fin.tellg()); // �������� ���������� ��������, ������� �� ������ ��� ����������� � ����� �����
		fin.seekg(42, fin.beg); // ��������� � ������ ����� ��������� �����
		// 42 - ������ ����� �����, ���� � ����� ������ 42 ��������, ��
		if (i <= 42) fail = true;
		else // ����� ��������� ���� � �����
		{
			for(int i=0; i < TAB_SIZE; i++)
			{
				char temp[SIZE_ADRESS];
				fin >> tab[i].pat.key; fin.get();
				fin.getline(temp, 1024, '\n');
				tab[i].pat.name = temp;
				fin >> tab[i].pat.bday[0]
					>> tab[i].pat.bday[1]
					>> tab[i].pat.bday[2]; fin.get();
				fin.getline(temp, 1024, '\n');
				tab[i].pat.workplace = temp;
				fin.getline(temp, 1024, '#');
				tab[i].pat.adress = temp;				

				if (!fin.eof()) fin.get();
				//if (flag != '#') fin.putback(flag); // ��������� ���� �� �������� ����
				// ���� �� ������� ����(������ ����� �� ���������), �� 
				if (tab[i].pat.key.length() < SIZE_KEY-2) tab[i].is_empty = true;
				// ����� �������, ��� ������ ���������
				else { tab[i].is_empty = false; _pat_count++; }
				// ���� ��� ������ ������� ������, �� ������� �� ����
				/*if (fin.fail()) 
				{ 
					fail = true;
					break;
				}*/
			}
		}
	} 
	// ���� ������, �� ������� ���������
	else { cout << "\n��������: �� ������� ������� ���� ������ ���������!\n\n"; system("pause"); fail = true; }
	// ���� ������, �� ��������� ��������� ������������� ����� ��������
	if (fail) htab_init(tab);
	// ������, �������������� ����� �� ������
	fin.sync();
	fin.clear();
	fin.close();
	// ���������� ���� ��������� ������
	return fail;
}
// ����� ���� ����� ���-������� � ����
void pat_to_file(ofstream &fout, hash_tab *tab)
{
	for(int i=0; i < TAB_SIZE; i++)
	{
		fout << '\n'<< tab[i].pat.key
			 << '\n' << tab[i].pat.name 
			 << '\n' << tab[i].pat.bday[0]
			 << ' ' << tab[i].pat.bday[1]
			 << ' ' << tab[i].pat.bday[2]
			 << '\n' << tab[i].pat.workplace
			 << '\n' << tab[i].pat.adress << '#';
	}
}
// ����� ������� � ����
void fill_pat_base(hash_tab *tab)
{
	ofstream fout;
	fout.open(_PAT_FILENAME);
	if (fout)
	{
		// ������ ����� ������� � ���� �����
		fout << "=========���� ������ ���������===========";
		// ����� ��� � ��� ���-�������
		pat_to_file(fout, tab);
	}
	else { cout << "\n��������: �� ������� ������� ���� ������ ���������!\n\n"; system("pause"); }
	fout.close();
}
/* ====================== DEBUGING ======================= */
void random_keys(hash_tab *tab)
{
	int index(0);
	char key[SIZE_KEY];

	for(int i=0; i < TAB_SIZE; i++)
	{
		for(int j=0; j < SIZE_KEY - 1; j++)
		{
			if (j == 2)
				key[j] = '-';
			else 
				key[j] = char(rand() % 9 + 48);
		}
		key[9] = '\0';
	}
}
/* =========== ������� ������������� �������� ============ */
// �������� ����� �� ������ ���������� "������� ��������" � ������� ���������
string key_creator(short num_place)
{
	// ���� ������� ��-������
	short null_count(0); // ������ ����� � ����� ����� "-"
	char buff[20]; // ����� ��� �������� ����� � ������
	char temp_num[3]; // ��������� ������ � � ������� ��������
	char temp_count[7]; // ��������� ������ � � ������� ��������
	string key; // ����
	// ��������� � temp_num � �������
	strcpy(temp_num, _itoa(num_place, buff, RADIX));
	// ��������� � temp_count � ������� ��������
	strcpy(temp_count, _itoa(_pat_count, buff, RADIX));
	// ���� � ������� <= 9, �� ��������� 0 � �����
	if (num_place <= 9) key += "0";
	key += temp_num;
	key += "-";
	// ���� �� ����� ���� "0�-", ���� "��-"
	// ������ �� ������� ������ � � ������� �������� ��������� ���-�� ����� ����� "-"
	null_count = SIZE_KEY - 1 - strlen(temp_count);
	for(int i = 3; i < null_count; i++)
		key += "0";
	key += temp_count;
	// ���������� ����
	return key;
}
// ������� ���������� ������ � ��������
patient pat_init()
{
	bool fail(false);
	short num_place(0);
	patient temp;
	char adress[SIZE_ADRESS];
	cout<<"������� ��� ��������: ";
	do
	{
		fail = false;
		input_control(temp.name);
		if (temp.name.length() <= 3)
		{
			cout << "������ ����� ������! ��������� ����: ";
			fail = true;
		}
	} while (fail);
	input_control(temp.bday);
	cin.get();
	cout<<"������� ����� ������/����� ��������: ";
	cin.getline(adress, SIZE_ADRESS, '\n');
	temp.workplace = adress;
	cout<<"������� ������ ��������: ";
	cin.getline(adress, SIZE_ADRESS, '\n');
	temp.adress = adress;
	cout<<"������� ����� ������� ��������: ";
	num_place = input_control(99, 1);
	temp.key = key_creator(num_place);
	_pat_count++;
	cout << "\n=== ������� ���������������! ===\n";
	return temp;
}
// ������� ������ ������� � ����������� �� ���������
void pat_out(hash_tab *tab)
{
	// ������ ��� ������������ �����
	const short COUNT_COL(5); // ����� ��������
	string name_col[] = { "���.�����", "��� ", "���� ��������", "�����", "����� ������" }; // ����� ��������
	short size_col[COUNT_COL] = { 11, 34, 14, 65, 30 }; // ������� ��������
	
	short size_tab = print_head(name_col, size_col, COUNT_COL); // ����� ����� �������
	for(int i = 0; i < TAB_SIZE; i++) // ����� ��������� �������
	{
		if (tab[i].is_empty) continue; // ���� �����, �� ����������
		else
		{
			cout << "|";
			print_row(tab[i].pat.key, size_col[0], tab[i].pat.key.length());
			print_row(tab[i].pat.name, size_col[1], tab[i].pat.name.length());
			cout << "  ";
			tab[i].pat.bday[0] < 10 ? cout << "0" << tab[i].pat.bday[0] : cout << tab[i].pat.bday[0];
			tab[i].pat.bday[1] < 10 ? cout << ".0" << tab[i].pat.bday[1] : cout << "." << tab[i].pat.bday[1];
			cout << "." << tab[i].pat.bday[2] << " |";
			print_row(tab[i].pat.adress, size_col[3], tab[i].pat.adress.length());
			print_row(tab[i].pat.workplace, size_col[4], tab[i].pat.workplace.length());
			cout << endl;
		}
	}
	print_end(size_tab); // ����� ������ ������� �������
}