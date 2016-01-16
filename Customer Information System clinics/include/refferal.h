/* == ������ (�����������, ����������������). ������/������� ����������� == */
#pragma once
#include "hospital.h"
/* =================== ��������� ������ =================== */
// ��������� ������ �����������
struct refferal
{
	string key_patient;// ���� �������: MM-NNNNNN
	char namedoctor[SIZE_NAME];
	SYSTEMTIME time;
	refferal *next; // ��������� �� ��������� ������� ������
};
// ������ ��������� ������, ���������� �������� ������
typedef vector<struct refferal*> svector_ref;
/* ================== ����������� ������ =================== */
// ������ �� ������ ���������
void htab_list_query(hash_tab *htab)
{
	string answer;
	cout << "������� ������ ���������? �/� ";
	answer_control(answer);
	if (answer[0] == '�')
	{
		pat_out(htab);
		system("pause");
	}
}
// ������ �� ������ ������
void doc_list_query(node *p)
{
	string answer;
	cout << "������� ������ ������? �/� ";
	answer_control(answer);
	if (answer[0] == '�')
	{
		doc_out(p);
		system("pause");
	}
}
// ������� ������������� ��������� �����
// ���������� true ���� ���� ����������
bool employment_analyzing(SYSTEMTIME &earliest_date, refferal *phead, node *p);
// ������� ������������� ���� ������
// ���������� true ���� ���� �� ������
bool vacant_day_analyzing(SYSTEMTIME picked_time, refferal *phead, node *p);
// ������� ���������� ������ � �������
refferal *ref_init(refferal *phead, node *p, hash_tab *htab, svector_doc &search)
{
	SYSTEMTIME cur_time; // ���� �����������
	GetSystemTime(&cur_time); // ������� ����

	refferal *temp = new refferal; // ����� �������
	int choice(0); // ����� ������� �����
	string answer; // ����� �� ������ ��/���
	string val; // ��� �����

	bool retry(false); // ���� ���������� ��������
	do // ����� ��������
	{
		int index(0); // ������ �������� � �������
		retry = false;
		cout << "������� ��������������� ����� ��������[MM-NNNNNN]: ";
		key_input(temp->key_patient);
		retry = !find_htab(htab, temp->key_patient, index); // ����� ����� � �������, ���� ������, �� retry = false
		if (retry) // ���� �� ������ �������, �� ��������� ����
		{
			cout << "\n�������������� ��������������� �����!\n";
			htab_list_query(htab); // ������ �� ����� ������� ���������
		}
		else // ���� ������ �������, ��
		{
			cout << "\n������� ������: "
				<< htab[index].pat.name << ", ";
			htab[index].pat.bday[0] < 10 ? cout << "0" << htab[index].pat.bday[0] : cout << htab[index].pat.bday[0];
			htab[index].pat.bday[1] < 10 ? cout << ".0" << htab[index].pat.bday[1] : cout << "." << htab[index].pat.bday[1];
			cout << "." << htab[index].pat.bday[2];
			cout << "\n��������� ����� �������� �����? �/� ";
			answer_control(answer); // ��� �� ������� ������?
			if (answer[0] != '�') // ���� �� ���, �� ��������� ����
			{
				htab_list_query(htab); // ������ �� ����� ������� ���������
				retry = true;
			}
			else cout << "\n=== ������� ������ ===\n";
		}
	} while (retry);

	do // ����� �����
	{
		retry = false;
		cout << "������� ��� �����: ";
		input_control(val);
		search_doc_name(p, search, val); // ���� �����

		if (!search.empty()) // ���� �� �����, �� ��������� ����
		{
			doc_out(search); // ������� ������ �������� ������
			if (search.size() >= 2) // ���� ������ ������ ��� 2, �� ������ ������
			{
				cout << "�������� ����� ������: ";
				choice = input_control(search.size(), 1);
			}
			else choice = 1; // ����� ���� 1, � ��� ������ ����������� ����� �����

			cout << "\n������� ������: "
				<< search[choice - 1]->key.name << " "
				<< search[choice - 1]->key.noun << "\n";

			cout << "\n��������� �������� ����� �����? �/� ";
			answer_control(answer); // ��� �� ���� ������?
			if (answer[0] == '�')
			{
				strcpy(temp->namedoctor, search[choice - 1]->key.name);
				cout << "\n=== ���� �������� ===\n";
			}
			else // ���� �� ���, �� ��������� ����
			{
				search.clear();
				doc_list_query(p); // ������ �� ����� ������� ������
				retry = true;
			}
		}
		else
		{
			cout << "\n=== ���� �� ������! ===\n";
			doc_list_query(p);
			retry = true; // ���� �� �����, �� ��������� ����
		}
	} while (retry);

	show_reception(search[choice - 1]->key.reception); // ����� ������� ������ �����

	SYSTEMTIME time; // ��������� ����� ������
	GetSystemTime(&time);
	bool picked_time(false); // ������� �� ������������ ������������ ��������� ����?
	bool analyz_success = employment_analyzing(time, phead, search[choice - 1]); // ���� y ����� ������ ���, �� ����� ���������� �������
	bool time_control(false); // ����� �� ������� ������� ��:��
	int hour(0), minute(0); // ������� ������� ��:��
	if (analyz_success)
	{
		cout << "��������� ����� ������: "
			<< time.wDay << " "
			<< get_month(time) << " "
			<< time.wYear << " - ";
		time.wHour < 10 ? cout << "0" << time.wHour : cout << time.wHour;
		cout << ":";
		time.wMinute < 10 ? cout << "0" << time.wMinute : cout << time.wMinute;
		cout << endl;
		cout << "\n��������� �������� ��� ����? �/� ";
		answer_control(answer);
		if (answer[0] == '�')
		{
			picked_time = true;
			temp->time = timecpy(time);
			datecpy(time, temp->time);
		}
		else 
		{

			picked_time = false;
		}
	}
	if (!picked_time)
	{
		do // ���������� ���� ������
		{
			if (!retry && !analyz_success)
			{
				time.wHour = search[choice - 1]->key.reception.time[0].wHour;
				time.wMinute = search[choice - 1]->key.reception.time[0].wMinute;
				many_day_increase(time);
				if (!is_date_visit(search[choice - 1]->key.reception.work_day, time))
				{
					diff_compensate(search[choice - 1]->key.reception.work_day, time);
				}
			}
			cout << "\n���� ����� ������� ������� �: "
				<< time.wDay << " "
				<< get_month(time) << " "
				<< time.wYear << " - ";
			time.wHour < 10 ? cout << "0" << time.wHour : cout << time.wHour;
			cout << ":";
			time.wMinute < 10 ? cout << "0" << time.wMinute : cout << time.wMinute;
			cout << endl;
			short min_day(0), min_month(0); // ����������� ���������� ���� � �����
			retry = false;
			cout << "������� ���� ������: "
				<< "\n  ���: ";
			temp->time.wYear = input_control(cur_time.wYear + 200, cur_time.wYear);
			if (temp->time.wYear > cur_time.wYear) // ���� ��������� ��� ������ ��������, �� ������� = 1
			{
				min_day = 1;
				min_month = 1;
			}
			else // ����� ������� = ������� ����
			{
				if (!analyz_success)
				{
					min_day = time.wDay;
					min_month = time.wMonth;
				}
				else
				{
					min_day = cur_time.wDay;
					min_month = cur_time.wMonth;
				}
			}
			cout << "  ����� ������: ";
			temp->time.wMonth = input_control(12, min_month);
			if (temp->time.wMonth > min_month) min_day = 1;
			cout << "  ����: ";
			temp->time.wDay = input_control(31, min_day);

			int week = what_day(temp->time); // ���������� �� ���� ���� ������
			for (int i = 0; i < WORKDAY_SIZE; i++)
			{
				if (week != search[choice - 1]->key.reception.work_day[i]) retry = true; // ���� ��� ������ ��� � ������� ������, �� ���� ������
				else { retry = false; break; } // ���� ����� ���� ������ � �������, �� ��������� �����
			}
			if (retry) cout << "� ��� ����� ���� �� ��������!\n";
		} while (retry);

		do
		{
			retry = false;
			cout << "������� ����� ������: "
				<< "\n  ����: ";
			temp->time.wHour = input_control(20, 9);
			cout << "  ������: ";
			temp->time.wMinute = input_control(59, 0);

			if (!is_time_visit(search[choice - 1]->key.reception.time, temp->time)) retry = true;

			if (retry) cout << "� ��� ����� ���� �� ��������!\n";
			if (analyz_success && !vacant_day_analyzing(temp->time, phead, search[choice - 1]))
			{
				cout << "��� ����� ������!\n";
				retry = true;
			}
		} while (retry);
	}
	cout << endl;
	search.clear();
	search.shrink_to_fit();
	return temp;
}
// ������� ������ ������� � ����������� � ��������
void print_list(refferal *phead, hash_tab *pat)
{
	int i(0), index(0);
    refferal *current = phead;
	// ������ ��� ������������ �����
	const short COUNT_COL(5); // ����� ��������
	string name_col[] = { " ## ", "���.�����", "��� ��������", "��� ����� ", "���� ������" }; // ����� ��������
	short size_col[COUNT_COL] = { 4, 11, 38, 28, 30 }; // ������� ��������

	short size_tab = print_head(name_col, size_col, COUNT_COL); // ����� ����� �������
    // ����� ����� �������
	if (!current) cout<<"������ ����!\n";
	else
	{
		do
		{
			i++;
			cout << "|";
			find_htab(pat, current->key_patient, index);

			print_row(i, size_col[0], int_size(i));
			print_row(current->key_patient, size_col[1], current->key_patient.length());
			print_row(pat[index].pat.name, size_col[2], pat[index].pat.name.length());
			print_row(current->namedoctor, size_col[3], strlen(current->namedoctor));

			cout << " " 
				<< current->time.wDay << " "
				<< get_month(current->time) << " "
				<< current->time.wYear << " - ";
			current->time.wHour < 10 ? cout << "0" << current->time.wHour : cout << current->time.wHour;
			cout << ":";
			current->time.wMinute < 10 ? cout << "0" << current->time.wMinute : cout << current->time.wMinute;
			cout << endl;
			current = current->next;/**/
		}
		while(current != phead);
	}
	print_end(size_tab); // ����� ������ ������� �������
}
// ������� ���-�� �������� ����������� (����� ��� ����������)
int list_count(refferal *phead)
{
	int count(0);
	refferal *current = phead;
	if (current)
	{
		do
		{
			count++;
			current = current->next;
		}
		while(current != phead);
	}
	return count;
}
// �������� ����� ����������� �������� ��� �
refferal *get_before(refferal *p)
{
	refferal *current = p;
	while(current->next != p)
		current = current->next;
	return current;
}
// ���������� ������ ������� ����������
void sort_list(refferal **phead, refferal **ptail)
{
	if (*phead) // ���� ������ ����������, ��
	{
		int count = list_count(*phead); // �������� ���-�� ��������� ������
		if (count > 2) // ���� ��������� ������ ��� 2
		{
			refferal *link; // ����� ��� ���������� ������������ ���������
			refferal *max; // ������� � ������������ ���������(���� ��� �����)
			refferal *before_m; // ������� ������� �� *max
			refferal *tail; // ������� ����������� �� ������� ��������������� ��������� ������
			refferal *before_t; // ������� ������� �� *tail
			refferal *current = *phead;

			tail = *ptail; // ��������� �������: ����� ������
			// ������������� �������� ������, ������ � ������ ����� ���-�� ������������� ��������� ����������� �� 1
			for(int i = count; i > 0; i--)
			{
				max = *phead;
				current = (*phead);
				// ���������� ������������ �������
				for(int j = 0; j < i; j++)
				{
					if (toupperstr(current->namedoctor) >= toupperstr(max->namedoctor))
						max = current;
					current = current->next;
				}

				before_m = get_before(max); // ������� ������� �� *max
				before_t = get_before(tail); // ������� ������� �� *tail
			
				if (max != tail) // ���� ����� ����� ������������� ��������, �� ���������� ��� � ���������, �����
				{
					if (max == *phead && tail == *ptail) // ���� ������������ ������� ������ � ������� ����� ����� ������, ��
					{// ������ �������� �������
						before_t->next = max; 
						link = max->next;
						max->next = tail;
						tail->next = link;

						*phead = tail;
					}
					else // �����
					{
						if (max->next == tail) // ���� ������������ ������� �������������, ��
						{// ��������� ��������
							link = tail->next;
							before_m->next = tail;
							tail->next = max;
							max->next = link;
							before_t = tail;
							if (max == *phead) *phead = tail;

						}
						else // �����
						{// ��������� ��������
							before_t->next = max;
							link = max->next;
							max->next = tail->next;
							tail->next = link;
							before_m->next = tail;
							if (max == *phead) *phead = tail;
						}
					}
				}
				tail = before_t; // ������� ������� ��������������� �������
				*ptail = get_before(*phead); // ��������� ����� ������
			}
		}
		else // ���� ��������� ������ ��� 2, ��
		{// ��������� ��������
			if (toupperstr((*phead)->namedoctor) > toupperstr((*ptail)->namedoctor))
			{
				refferal *temp = *phead;

				*phead = *ptail;
				(*phead)->next = temp;
				temp->next = *phead;
				*ptail = temp;
			}
		}
	}
}
// ������� ����������� �����������
void add_to_list(refferal **phead, refferal **ptail, node *p, hash_tab *htab, svector_doc &search)
{
    refferal *current = ref_init(*phead, p, htab, search); // ��������� ������ � �������
	current->next = NULL;

    if (!(*phead)) *phead = current;
    else (*ptail)->next = current;
    (*ptail) = current;

	(*ptail)->next = *phead; // ����������� ������
	sort_list(phead, ptail); // ���������
}
// ������� ���������� ����������� ��� ������ �� �����
void add_to_list(refferal **phead, refferal **ptail, refferal *ref)
{
	refferal *current = ref;
	current->next = NULL;

	if (!(*phead)) *phead = current;
	else (*ptail)->next = current;
	(*ptail) = current;

	(*ptail)->next = *phead; // �����������
	sort_list(phead, ptail); // ���������
}
// �������� ����� ������� �� ������ 
void del_ref(refferal **phead, refferal **ptail, svector_ref &search, string val, bool key = false)
{
	int choice(0); // ����� �������� �� ��������
	string answer; // ����� �� ������ ��/���
	refferal *current = *phead;

	if (key) // ���� ����� �������� �� �������� ���� �� �����, �� ���� �� �����
	{
		do
		{
			if (string_cmp(val, current->key_patient))
				search.push_back(current);
			current = current->next;
		} while (current != (*phead));
	}
	else // ����� ���� �� ������� �����
	{
		do
		{
			if (string_cmp(val, current->namedoctor))
				search.push_back(current);
			current = current->next;
		} while (current != (*phead));
	}
	if (!search.empty()) // ���� �����, ��
	{
		cout << "���������: \n";
		for (unsigned int i = 0; i < search.size(); i++)
		{
			cout << i + 1 << ") " << "\n\t���. �����: "
				<< search[i]->key_patient << "\n\t��� �����: "
				<< search[i]->namedoctor << "\n\t���� ������: "
				<< " "
				<< search[i]->time.wDay << " "
				<< get_month(search[i]->time) << " "
				<< search[i]->time.wYear << endl;
		}
		if (search.size() >= 2) // ���� ��������� ������� ������ ��� 2, �� �����������
		{
			cout << "�������� ����� ������ ��� ����������� ��������: ";
			choice = input_control(search.size(), 1);
		}
		else choice = 1; // ����� ����� �������� ���� ������ ����?

		cout << "\n������� ������: \n ���. �����: "
			<< search[choice - 1]->key_patient << "\n ��� �����: "
			<< search[choice - 1]->namedoctor << "\n ���� ������: "
			<< " "
			<< search[choice - 1]->time.wDay << " "
			<< get_month(search[choice - 1]->time) << " "
			<< search[choice - 1]->time.wYear << endl;

		cout << "\n����������� �� �� ������� ������? �/� ";
		answer_control(answer);
		if (answer[0] == '�')
		{
			refferal *temp = get_before(search[choice - 1]);
			if (search[choice - 1]->next == search[choice - 1])
			{
				delete search[choice - 1];
				*phead = *ptail = NULL;
			}
			else
			{
				temp->next = search[choice - 1]->next;
				(*ptail) = get_before(*phead);
				delete search[choice - 1];
			}
			cout << "\n=== ������ ������� ===\n";
		}
		else cout << "\n=== ������ �� ������� ===\n";
	}
	cout << endl;
}
// ���� �������� �������
void del_ref_menu(refferal **phead, refferal **ptail)
{
	svector_ref search; // ������ ������ �������
	int s_choice(0); // ����� ��������
	string val; // ��� �����
	if (*phead) // ���� ������ ����������, �� 
	{
		cout << "�������� �������� ��� �������� �������:\n"
			<< " 1. ��� �����\n"
			<< " 2. ���. ����� ��������\n"
			<< " 0. ������ ��������\n"
			<< "\n������� ����� ���������� ��������: ";
		s_choice = input_control(4, 0);
		switch (s_choice)
		{
		case 1:
			cout << "\n ������� ��� �����: ";
			input_control(val);
			del_ref(phead, ptail, search, val);
			break;
		case 2:
			cout << "\n ������� ���. ����� ��������[MM-NNNNNN]: ";
			key_input(val);
			del_ref(phead, ptail, search, val, true);
			break;
		case 0:
			break;
		}
	}
	else cout << " ������ ����.\n"; // ����� �������� �� ��������� ���������
}
// �������� � ������������ ������ ������
void del_list(refferal **phead, refferal **ptail)
{
	if (*phead)
	{
		refferal * current;
		while(true)
		{
			current = *phead;
			*phead = current->next;
			if (current == *ptail) break;
			delete current;
		}
		delete current;
	}
	*phead = *ptail = NULL;
}
/* =============== ������ � ������ =================== */
// ��������� ������ �� ����� � ��������� ��� ������
void read_ref_base(refferal **phead, refferal **ptail)
{
	fstream fin;
	fin.open(_REF_FILENAME, ios_base::in);

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
				refferal *temp = new refferal;

				fin.getline(temp->namedoctor, 1000, '\n');
				fin >> temp->key_patient
					>> temp->time.wDay
					>> temp->time.wMonth
					>> temp->time.wYear
					>> temp->time.wHour
					>> temp->time.wMinute;
				add_to_list(phead, ptail, temp); // ��������� ������
				if (!fin.eof()) fin.get();
			}
		}
	}
	else { cout << "\n��������: �� ������� ������� ���� ������ �����������!\n\n"; system("pause"); }
	fin.sync();
	fin.clear();
	fin.close();
}
// ����� ���� ��������� ������ � ����
void ref_to_file(fstream &fout, refferal *phead)
{
	refferal *current = phead;
	do
	{
		fout << '\n' << current->namedoctor
			 << '\n' << current->key_patient
			 << ' ' << current->time.wDay
			 << ' ' << current->time.wMonth
			 << ' ' << current->time.wYear
			 << ' ' << current->time.wHour
			 << ' ' << current->time.wMinute;
		current = current->next;
	} while (current != phead);
}
// ����� ������ � ����
void fill_ref_base(refferal *phead)
{
	fstream fout;
	fout.open(_REF_FILENAME, ios_base::out);
	if(fout)
	{
		fout << "==========���� ������ �������============";
		if (phead) ref_to_file(fout, phead);
	}
	else { cout<<"\n��������: �� ������� ������� ���� ������ �������!\n\n"; system("pause"); }
	fout.close();
}