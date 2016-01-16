/* ===== ���-������ (������������ �����). ������ ===== */
#pragma once
#include "anytools.h"
#include "patient.h"
#include "date.h"
/* =============== ��������� ������ ================== */
// ��������� ������ �����
struct doctor
{
	char name[NAME_DOC_SIZE]; // ������� � ��������
	string noun; // ���������
	unsigned int cabinet; // ����� ��������
	date reception; // ������ ������
};
// ���� ������
struct node
{
    doctor key; // ���� ���� ������
    int height; // ������ ��������� � ������ � ������ ����
    node* left; // ��������� �� ����� ���������
    node* right;// ��������� �� ������ ���������
};
// ������ ��������� ������, ���������� �������� ������
typedef vector<struct node*> svector_doc;
/* =================== ���-������ ==================== */
// ���������� ������ ���� 
// ���� ���� �� ����������, �� ���������� ����
int get_height(node* p) 
{
	if (p) return p->height;
	else return NULL;
}
// ���������� ������� ����� ������� � ������ ����������� ��� ��������� ����
int diff_height(node* p)
{
	int temp = get_height(p->right) - get_height(p->left);
	return temp;
}
// ������������� ����������� �������� ������ ��� ���������
void normalize_height(node *p)
{
	int left(0), right(0);
	left = get_height(p->left);
	right = get_height(p->right);

	if (left > right) p->height = left + 1;
	else p->height = right + 1;
}
// ����� ������ �������� ����� ������� ����
node* to_right_rotate(node *p)
{
    node* rotate = p->left;
	p->left = rotate->right;
	rotate->right = p;

	normalize_height(p);
	normalize_height(rotate);
	return rotate;
}
// ����� ����� �������� ����� ������� ����
node* to_left_rotate(node *p) 
{
	node* rotate = p->right;
	p->right = rotate->left;
	rotate->left = p;

	normalize_height(p);
	normalize_height(rotate);
	return rotate;
}
// ������������ ���� p
node* balance_tree(node *p)  
{
	normalize_height(p);

	if (diff_height(p) == 2) // ���� ������� ����� ������� � ������ ����������� ��� ��������� ���� = 2, ��
    {
		if (diff_height(p->right) < 0) // ������� ������������ �� ������� ����� ��� ������� ���������
			p->right = to_right_rotate(p->right); // �� ������������ ������ ����� �������
		return to_left_rotate(p); // ��������� ������ ����� ������ ��� �������
    }
	if (diff_height(p) == -2) // ���� ������� ����� ������� � ������ ����������� ��� ��������� ���� = -2, ��
	{
		if (diff_height(p->left) > 0) // ������� ������������ �� ������� ����� ��� ������ ���������
			p->left = to_left_rotate(p->left); // �� ������������ ������ ����� �������
		return to_right_rotate(p); // ��������� ����� ����� ������ ��� �������
	}

	return p;
}
// ���������� ������ ����� � ������
node* add_node(node *p, doctor k)  
{
	if (!p) // ���� ������ ������, �� �������� ������ ����
	{
		node *temp = new node;
		temp->key = k;
		temp->height = 1;
		temp->right = NULL;
		temp->left = NULL;
		return temp;
	}
	if (toupperstr(k.name) < toupperstr(p->key.name)) // ���� ���� ����� ������ ���� ������ ���� ����� ��� ���������� ����
	{
		p->left = add_node(p->left, k); // �� ��������� � ������ ���������
	}
	else
	{
		p->right = add_node(p->right, k); // ����� � �����
	}
	return balance_tree(p); // �.�. ��� ����������, �� ����������� ������ ������������
}
// ����� ���� � ����������� ��������� � ����������� ���� p 
node* get_min_node(node *p) 
{
	if (p->left) return get_min_node(p->left);
	else return p;
}
// "��������" ���� � ����������� ��������� �� ������ p
// ���������� �� ����� ���� �������� ���������, ��� ���� ����������� ������ 
// ������ ����� ����������
node* del_min(node *p) 
{
    if(!p->left) return p->right;

	p->left = del_min(p->left);

	return balance_tree(p);
}
// ����� � �������� ���� �� ����� �����
node* del_node(node *p, doctor k) 
{
    if(!p) return NULL; // ���� ���� �����������, �� ���������� ����

	if (toupperstr(k.name) < toupperstr(p->key.name)) // ���� �������� ���� ������ ����� �������� ����
	{
		p->left = del_node(p->left, k); // �� ���������� ������
	}
	else
	{
		if (toupperstr(k.name) > toupperstr(p->key.name)) // ���� �������� ���� ������ ����� �������� ����
		{
			p->right = del_node(p->right, k); // �� ���������� �������
		}
		else //if(string_cmp(k.name, p->key.name)) // ���� �� ����� ������ ����, �� ������� ��� � ����������� ������
		{
			node* Left = p->left; // ���������� ����� ���������
			node* Right = p->right; // ���������� ������ ���������
			delete p; // ������� ����

			if (!Right) return Left; // ������� ��������� ���, �� ���������� ������ ���������
			node* min = get_min_node(Right); // ����� ���� ���� � ����������� ������ � ���������� ���
			min->right = del_min(Right); // �.�. ����� ��� ����������, �� �������� �������� ��� ������� ����� �������� � ������ ���������
			min->left = Left; // ����������� ����� ���������, ������� ��������� 
			p = balance_tree(min); // �����������
			//return del_node(p, k); // �������� ��������, ��� �������� ������������� �����.
		}
	}
	return balance_tree(p); // �����������
}
// ����� � ���� ������, ���� �����
void debug_out(node *p, int l) 
{
	if (p)
	{
		debug_out(p->right, l+1);

		for (int i=1; i<=l; i++) 
			cout<<"   ";
		cout<<p->key.name<<endl;

		debug_out(p->left, l+1);
	}
}
// �������� ��� ��������� ����� � ���� ������. ���������� ������������ �����
void get_all_pointers(svector_doc &temp, node *p) 
{
	if (p->left) get_all_pointers(temp, p->left);
	temp.push_back(p);
	if (p->right) get_all_pointers(temp, p->right);
}
// ������� ��� ���� ������ �
void remove_all_doc(node **p) 
{
	if (*p) // ���� ������ ����, ��
	{
		svector_doc temp;
		get_all_pointers(temp, *p); // �������� ��� ��������� ����� � ���� ������.

		for(unsigned int i=0; i<temp.size(); i++) // ����������� ������ 
		{
			delete temp[i];
		}

		temp.clear();
		temp.shrink_to_fit();
	}
	*p = NULL; // ����������� ��������� ����, ��� ��� ��� �������
}
/* ========= ������� ������������� ������� =========== */
// ���������� ������� ���� ������� �����
void fill_workday(doctor &doc)
{
	int index(0), day_count(0);
	bool fail(false);
	short val;
	// ������������� �������������� ������ ������� ����
	for (index = 0; index < WORKDAY_SIZE; index++)
		doc.reception.work_day[index] = NON_WEEK_DAY;

	cout << "������� ���������� ������� ���� ������[1-5]: ";
	day_count = input_control(5, 1);

	cout << "�������� ������� ��� �����:\n"
		<< " 1. �����������\n"
		<< " 2. �������\n"
		<< " 3. �����\n"
		<< " 4. �������\n"
		<< " 5. �������\n"
		<< " 6. �������\n"
		<< " 7. �����������\n\n";
	// ��������� ������ ������� ����
	for (index = 0; index < day_count; index++)
	{
		cout << "�������� ���� ������ #" << index + 1 << ": ";
		do
		{
			if (fail) cout << "������ ���� ������ ���� � ������� ������, �������� ������: ";
			val = input_control(7, 0);
			fail = check_copyday(doc.reception.work_day, val); // �������� �� ������������� ��� ������
		} 
		while (fail);
		if (val == 7) val -= 7; // ����������� ����� � 0, ����������� - � 1, ..., ������� - � 6
		doc.reception.work_day[index] = val;
		//if (doc.reception.work_day[index] == 7) doc.reception.work_day[index] -= 7;
	}
	sort_worday(doc.reception.work_day); // ���������� ���� ������ � ������� ������� ����
}
// ���������� ������� ������ ������� �����
void fill_worktime(doctor &doc)
{
	bool fail(false);
	SYSTEMTIME temp;

	cout << "���� ������ �����(�� ����� 8 �����): ";
	cout << " ������� ����� ������ ���. ���(9 - 15): "
		<< "\n  ����: ";
	doc.reception.time[0].wHour = input_control(15, 9);
	cout << "  ������: ";
	doc.reception.time[0].wMinute = input_control(59, 0);

	do
	{
		fail = false;
		cout << " ������� ����� ��������� ���. ���(15 - 20): "
			<< "\n  ����: ";
		doc.reception.time[1].wHour = input_control(20, 15);
		cout << "  ������: ";
		doc.reception.time[1].wMinute = input_control(59, 0);
		/*if (doc.reception.time[1].wHour <= doc.reception.time[0].wHour)
			fail = true;
		else
		{/**/
		temp = diff_time(doc.reception.time[1], doc.reception.time[0]); // ���� ������� � ����� > 8 �����, ��
		if (temp.wHour > 8)
		{
			fail = true;
			//if (fail) 
			cout << "������. ��������� ����!\n";
		}
	} while (fail);
}
// ����� ������� ���� ������� �����
void print_workday(doctor doc)
{
	if (doc.reception.work_day[0] != NON_WEEK_DAY)
	{
		for (int i = 0; i < WORKDAY_SIZE - 1; i++)
		{
			if (doc.reception.work_day[i] != NON_WEEK_DAY)
				cout << get_day_of_week(doc.reception.work_day[i]) << " ";
		}
	}
}
/* ========= ������� ������������� ������� =========== */
// ������� ���������� ������ � �����
doctor doc_init()
{
	doctor temp;
	bool fail(false);
	cout<<"������� ������� � �������� �����: ";
	do
	{
		fail = false;
		input_control(temp.name);
		if (strlen(temp.name) <= 3)
		{
			cout << "������ ����� ������! ��������� ����: ";
			fail = true;
		}
	} while (fail);
	cout<<"������� ��������� �����: ";
	input_control(temp.noun);
	cout<<"������� ������� �����: ";
	temp.cabinet = input_control(999, 1);
	fill_workday(temp);
	fill_worktime(temp);

	return temp;
}
// ������� ������ ����� ������� � ����������� � ������
void view_members(node *p, int &i, short *size_col)//svector_doc &pointers)//int &i, string *name_col, short *size_col)
{
	if (p)
	{
		if (p->left)
			view_members(p->left, i, size_col);// , pointers);

		cout << "|";
		print_row(i + 1, size_col[0], int_size(i + 1));
		print_row(p->key.name, size_col[1], strlen(p->key.name));
		print_row(p->key.noun, size_col[2], p->key.noun.length());
		print_row(p->key.cabinet, size_col[3], int_size(p->key.cabinet));
		show_reception(p->key.reception);
		i++;
		if (p->right)
			view_members(p->right, i, size_col);// , pointers);
	}
}
// ������� ������ ������� � ����������� � ������
void doc_out(node *p)//, int &i)
{
	int i(0); // ����� ������ � ������� ##
	// ������ ��� ������������ �����
	const short COUNT_COL(5); // ����� ��������
	string name_col[] = { " ## ", "��� ", "���������", "� ���", " ������ ������ " }; // ����� ��������
	short size_col[COUNT_COL] = { 4, 34, 11, 7, 84 }; // ������� ��������

	short size_tab = print_head(name_col, size_col, COUNT_COL); // ����� ����� �������
	view_members(p, i, size_col);// pointers); // ����� ��������� �������
	print_end(size_tab); // ����� ������ ������� �������
}
// ������� ����� ������ ������ ��� ������
void doc_out(svector_doc search)
{
	int j(0);
	for (unsigned int i = 0; i < search.size(); i++)
	{
		j++;
		cout << j << ") "
			<< search[i]->key.name << ", "
			<< search[i]->key.noun << ", � ��������: "
			<< search[i]->key.cabinet << " " << endl;
	}
}
/* =============== ������ � ������ =================== */
// ��������� ������ �� ����� � ��������� ��� ���-������
void read_doc_base(node **p)
{
	char flag; // ���� �������������� ���������� ������� ����
	fstream fin;
	fin.open(_DOC_FILENAME, ios_base::in);

	if (fin)
	{
		// ������� �� ���������� �������� � �����
		fin.seekg(0, ios_base::end); // ��������� � �����
		int j = int(fin.tellg()); // �������� ���������� ��������, ������� �� ������ ��� ����������� � ����� �����
		fin.seekg(43, ios::beg); // ��������� � ������ ����� ��������� �����
		// 43 - ������ ����� �����, ���� � ����� ������ 43 ��������, ��
		//fin.seekg(0, ios::beg);
		if (j > 43)
		{
			while (!fin.eof())
			{
				doctor temp;
				// ������������� �������������� ������ ������� ����
				for (int k = 0; k < WORKDAY_SIZE; k++)
					temp.reception.work_day[k] = NON_WEEK_DAY;

				fin.getline(temp.name, 1000, '\n');
				fin >> temp.noun
					>> temp.cabinet;
				int j(0);
				while (true)
				{
					fin.get(flag);
					if (flag != '#') fin.putback(flag); // ��������� ���� �� �������� ����
					else break;
					fin >> temp.reception.work_day[j];
					j++;
				}
				for (int k = 0; k < 2; k++) // ��������� ����� ������ �����
				{
					fin >> temp.reception.time[k].wHour;
					fin >> temp.reception.time[k].wMinute;
				}
				if (!fin.eof()) fin.get();
				*p = add_node(*p, temp);
			}
		}
	}
	else { cout << "\n��������: �� ������� ������� ���� ������ ��������!\n\n"; system("pause"); }
	fin.sync();
	fin.clear();
	fin.close();
}
// ����� ���� ����� ������ � ���� ������� ������������� ������
void doc_to_file(fstream &fout, node *p)
{
	if (p)
	{
		if (p->left) 
			doc_to_file(fout, p->left);

		fout << '\n' << p->key.name
			 << '\n' << p->key.noun
			 << ' ' << p->key.cabinet << ' ';
		// ���������� ������� ��� ������� ������ �����
		for (int i = 0; p->key.reception.work_day[i] < NON_WEEK_DAY && i < NON_WEEK_DAY - 1; i++)
		{
			fout << p->key.reception.work_day[i];
			p->key.reception.work_day[i + 1] < NON_WEEK_DAY ? fout << ' ' : fout;
		}
		fout << "# "; // ��������� ���� ����� ������� ����
		for (int i = 0; i < 2; i++) // ��������� ����� ������ �����
		{
			fout << p->key.reception.time[i].wHour
				<< ' ' << p->key.reception.time[i].wMinute;
			i == 0 ? fout << ' ' : fout;
		}

		if (p->right) 
			doc_to_file(fout, p->right);
	}
}
// ����� ������ � ����
void fill_doc_base(node *p)
{
	doctor temp;

	fstream fout;
	fout.open(_DOC_FILENAME, ios_base::out);
	if(fout)
	{
		fout << "==========���� ������ ��������===========";
		if (p) doc_to_file(fout, p);
	}
	else { cout<<"\n��������: �� ������� ������� ���� ������ ��������!\n\n"; system("pause"); }
	fout.close();
}
/* ============= ������� ������ doctor =============== */
// ����� ������ �� ���
void search_doc_name(node *p, svector_doc &search, string name)
{
	if (!p) return;

	if (toupperstr(name.c_str()) <= toupperstr(p->key.name))
	{
		search_doc_name(p->left, search, name);
	}
	else
	{
		if (toupperstr(name.c_str()) > toupperstr(p->key.name))
		{
			search_doc_name(p->right, search, name);
		}
	}
	if (string_cmp(name, p->key.name))
		search.push_back(p);
	if (toupperstr(name.c_str()) < toupperstr(p->key.name))
		search_doc_name(p->right, search, name);
}
// ����� ������ �� ���������
void search_doc_noun(node *p, svector_doc &search, string noun)
{
	if (!p) return;

	if (p->left) search_doc_noun(p->left, search, noun);

	if (string_cmp(noun, p->key.noun)) search.push_back(p);

	if (p->right) search_doc_noun(p->right, search, noun);
}