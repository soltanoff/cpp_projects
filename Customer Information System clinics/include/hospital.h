/* ============ ����� �� ���� ���������� ============= */
#pragma once
#include "doctor.h"
#include "patient.h"
#include "refferal.h"
#include "anytools.h"
/* ======= ����������� ��������� ���� ������� ======= */
// ��������� �������
struct ref_collect
{
	refferal *ref; // ��������� �� �������
	ref_collect *next; // ��������� �� ��������� ������� ���������
	ref_collect(refferal *reffer) // ��������������� �����������
	{
		ref = reffer;
		next = NULL;
	}
};
// ������ �������� ���� �����
struct busy_day
{
	bool is_full; // �������� �� ����
	SYSTEMTIME current_date; // ������� ����
	SYSTEMTIME earliest_date; // ��������� ����� �� �����
	ref_collect *collection; // ������� �������� �� ���� ����
	busy_day *next; // ��������� �� ��������� ���������
	busy_day(refferal *reffer) // ��������������� �����������
	{
		next = NULL;
		is_full = false;
		current_date = datecpy(reffer->time);
		collection = new ref_collect(reffer);
	}
};
// ������������ ������ �� ������ �������� ���� �����
// ��� �������, ��� � �������
void del_busy_time(busy_day **current_work)
{
	busy_day *cur_w = *current_work;
	if (*current_work)
	{
		busy_day *cur_w = *current_work;
		while (*current_work)
		{
			// ������� ��������� �������
			cur_w = *current_work;
			ref_collect *cur_r = cur_w->collection;
			if (cur_r)
			{
				while (cur_w->collection)
				{
					cur_r = cur_w->collection;
					cur_w->collection = cur_r->next;
					delete cur_r;
				}
			}
			// ������� ������� ������
			*current_work = cur_w->next;
			delete cur_w;
		}
	}
}
// ������� ����������� ������� ����� ������ ���� ��������� ������ � � ������ 
template<class T>void trans_to_vector(T *phead, vector<T*> &vec)
{
	T *current = phead;
	while (current)
	{
		vec.push_back(current);
		current = current->next;
	}
}
// ������� ��������������� ���������� ������ � �������� ������������� �������
template<class T>void back_links(T **phead, vector<T*> &vec)
{
	*phead = vec[0];
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (i == vec.size() - 1) vec[i]->next = NULL;
		else vec[i]->next = vec[i + 1];
	}
}
// ������� ���������� ��������� ����������� ����� �� �������
// ���������� ����������������� ��������� :D
void sort_refcollect(ref_collect **ref)
{
	if (*ref)
	{
		vector<ref_collect*> vec; // ������ �������� � ���� �������� ������
		bool changed(true); // ���� ��������� � �������(���� �� ������������ ���������)
		ref_collect *temp; // ��������� �������(��� ������������)
		// �������� ������ ��������� �� ������ � ������
		trans_to_vector(*ref, vec);
		// ��������� ���� ���� �����-���� ���������
		while (changed)
		{
			changed = false;
			for (unsigned int i = 0; i < vec.size() - 1; i++)
			{
				if (time_cmp(vec[i]->ref->time, vec[i]->ref->time) == 1)
				{
					temp = vec[i];
					vec[i] = vec[i + 1];
					vec[i + 1] = temp;
					changed = true;
				}
				if (time_cmp(vec[vec.size() - 1 - i]->ref->time, vec[vec.size() - 2 - i]->ref->time) == 2)
				{
					temp = vec[vec.size() - 1 - i];
					vec[vec.size() - 1 - i] = vec[vec.size() - 2 - i];
					vec[vec.size() - 2 - i] = temp;
					changed = true;
				}
			}
		}
		// �������������� ������ ������������ ����������
		back_links(ref, vec);
	}
}
// ������� ���������� ������ �������� ���� ����� �� ����
// ���������� ����������������� ��������� :D
void sort_busyday(busy_day **current_work)
{
	if (*current_work)
	{
		vector<busy_day*> vec; // ������ �������� � ���� �������� ������
		bool changed(true); // ���� ��������� � �������(���� �� ������������ ���������)
		busy_day *temp; // ��������� �������(��� ������������)
		// �������� ������ ��������� �� ������ � ������
		trans_to_vector(*current_work, vec);
		// ��������� ���� ���� �����-���� ���������
		while (changed)
		{
			changed = false;
			for (unsigned int i = 0; i < vec.size() - 1; i++)
			{
				if (date_cmp(vec[i]->current_date, vec[i]->current_date) == 1)
				{
					temp = vec[i];
					vec[i] = vec[i + 1];
					vec[i + 1] = temp;
					changed = true;
				}
				if (date_cmp(vec[vec.size() - 1 - i]->current_date, vec[vec.size() - 2 - i]->current_date) == 2)
				{
					temp = vec[vec.size() - 1 - i];
					vec[vec.size() - 1 - i] = vec[vec.size() - 2 - i];
					vec[vec.size() - 2 - i] = temp;
					changed = true;
				}
			}
		}
		// �������������� ������ ������������ ����������
		back_links(current_work, vec);
	}
}
// ������� �������� ������ �������� ���� �����
void create_reception(busy_day **current_work, refferal *ref)
{
	if (!(*current_work)) // ���� ������ ����, �� �������� ������ �������
	{
		(*current_work) = new busy_day(ref);
	}
	else// �����
	{
		bool writted(false); // ���� ������������ �� ��, ��� ���� ����������� ������
		busy_day *cur_w = *current_work;
		while (cur_w) // ������ �� ������ ���� �� ������ �� �����
		{
			if (date_cmp(cur_w->current_date, ref->time) == 0) // ���� ���� ������� ��������� � ����� ������ �������� ����, ��
			{// �� ��������� ��������� �������
				ref_collect *cur_r = cur_w->collection;
				while (cur_r->next)
					cur_r = cur_r->next;
				cur_r->next = new ref_collect(ref);
				writted = true;
				break;
			}
			cur_w = cur_w->next;
		}
		if (!writted) // ���� ������� �� ������ �� � ����� ���������, �� ������� ����� ������� ������ �������� ����
		{
			busy_day *cur_w = *current_work;
			while (cur_w->next)
				cur_w = cur_w->next;
			cur_w->next = new busy_day(ref);
		}
	}
	
}
// debug-����� ����������� ������������ ���������
void debugout_of_analyzing(busy_day *current_work, bool with_earl_date = false)
{
	if (current_work)
	{
		int i(1);
		busy_day *cur = current_work;
		while (cur)
		{
			cout << i << ") ";
			cout << " "
				<< cur->current_date.wDay << " "
				<< get_month(cur->current_date) << " "
				<< cur->current_date.wYear;
			if (with_earl_date)
			{
				cout << " ��������� ����� ������: "
					<< cur->earliest_date.wDay << " "
					<< get_month(cur->earliest_date) << " "
					<< cur->earliest_date.wYear << " - ";
				cur->earliest_date.wHour < 10 ? cout << "0" << cur->earliest_date.wHour : cout << cur->earliest_date.wHour;
				cout << ":";
				cur->earliest_date.wMinute < 10 ? cout << "0" << cur->earliest_date.wMinute : cout << cur->earliest_date.wMinute;
				cout << endl;
			}
			else cout << endl;
			int k(1);
			ref_collect *cur_r = cur->collection;
			while (cur_r)
			{
				cout << "\t[" << k << "]"
					<< " " << cur_r->ref->key_patient
					<< " - " << cur_r->ref->namedoctor << " | ";
				cur_r->ref->time.wHour < 10 ? cout << "0" << cur_r->ref->time.wHour : cout << cur_r->ref->time.wHour;
				cout << ":";
				cur_r->ref->time.wMinute < 10 ? cout << "0" << cur_r->ref->time.wMinute : cout << cur_r->ref->time.wMinute;
				cout << endl;
				cur_r = cur_r->next;
				k++;
			}
			i++;
			cur = cur->next;
		}
	}
}/**/
// ������� ������������ ��������� ���� ������ �������� ������. 
// ���������� true ���� ���� ����������
bool def_earliest_date(node *p, ref_collect *collection, SYSTEMTIME &earliest_date)
{
	ref_collect *current = collection; // ��������� ����������� �����
	bool diff_fail(false); // ������ ��� �������� ������� ����� ������

	SYSTEMTIME temp = diff_time(current->ref->time, p->key.reception.time[0], &diff_fail);
	if (temp.wMinute > 30 || temp.wHour >= 1) // ���� ������� ����� ������� ���. ��� � ������ �������� ������ 30 ��� ��� �����, ��
	{
		earliest_date = timecpy(p->key.reception.time[0]); // ���������� ��������� ����(������ ���. ���)
		return true;
	}
	else // ����� ������� �����
	{
		while (current->next) // ������ �� ������, ���� �� ������� � ��������� �������
		{
			temp = diff_time((current->next)->ref->time, current->ref->time, &diff_fail);
			if (temp.wMinute > 30 || temp.wHour >= 1)// ���� ������� ����� ����� �������� � ������ �������� ������ 30 ��� ��� �����, ��
			{
				earliest_date = timecpy(current->ref->time); // �� �������� ����� ��������� �������
				datecpy(current->ref->time, earliest_date);
				half_hour_increase(earliest_date); // ������ ����������� ����� �� 30 ���

				temp = diff_time((current->next)->ref->time, earliest_date, &diff_fail);
				if (temp.wMinute > 30 || temp.wHour >= 1) return true; // ���� ����� ���������� ����� �������� � ������� ��������, �� �������� � ���, ��� ���� �������
			}
			current = current->next;
		}
		// ���� �� ������ �� ����� �� �����, ��� ���������� ��������� ��������� �������
		temp = diff_time(p->key.reception.time[1], current->ref->time, &diff_fail);
		if (temp.wMinute >= 30 || temp.wHour >= 1)
		{
			earliest_date = timecpy(current->ref->time);
			datecpy(current->ref->time, earliest_date);
			half_hour_increase(earliest_date);

			temp = diff_time(p->key.reception.time[1], earliest_date, &diff_fail);
			if (temp.wMinute > 30 || temp.wHour >= 1) return true;
		}
		return false;
	}
}
// ������� ������������ ��������� ���� ������ ��������
// ���������� true ���� ���� �� ������
bool def_vacant_date(ref_collect *collection, SYSTEMTIME picked_time)
{
	ref_collect *current = collection; // ��������� ����������� �����
	bool diff_fail(false); // ������ ��� �������� ������� ����� ������
	while (current)
	{
		if (time_cmp(current->ref->time, picked_time) == 0) // ���� ��������� ����� ������������� ��������� � �������� �������, ��
		{
			return false; // ���� ������
		}
		else
		{// ���� ��������� ����� ������������� ����� � ���������� (����� ������� - 30 ���; ����� ������� + 30 ���), �� ���� ������
			if (time_cmp(current->ref->time, picked_time) == 1) 
			{
				SYSTEMTIME temp = diff_time(current->ref->time, picked_time, &diff_fail);
				if (temp.wMinute < 30 && temp.wHour < 1) 
				{
					return false;
				}
			}
			else
			{
				SYSTEMTIME temp = diff_time(picked_time, current->ref->time, &diff_fail);
				if (temp.wMinute < 30 && temp.wHour < 1)
				{
					return false;
				}
			}
		}
		current = current->next;
	}
	return true;
}
// ������� ������������� ���� ������
// ���������� true ���� ���� �� ������
bool vacant_day_analyzing(SYSTEMTIME picked_time, refferal *phead, node *p)
{
	bool vacant(true);
	if (phead)
	{
		SYSTEMTIME cur_time;
		GetSystemTime(&cur_time);

		bool result(false); // true ���� ���� ����������
		busy_day *current_work = NULL; // ������ �������� ���� �����

		refferal *cur = phead; // ������ �����������
		do // ���� ������� ����������� � ������� �����
		{	// ���� ����� �������, �� ��������� ������ �������� ����
			if (string_cmp(p->key.name, cur->namedoctor)) create_reception(&current_work, cur);
			cur = cur->next;
		} while (cur != phead);

		busy_day *cur_w = current_work;
		while (cur_w)
		{
			sort_refcollect(&(cur_w)->collection);
			cur_w = cur_w->next;
		}

		cur_w = current_work;
		if (current_work) // ���� ������ ������, �� ���������� ��������� ���� ������
		{
			while (cur_w)
			{
				//cur_w->is_full = !def_earliest_date(p, cur_w->collection, cur_w->earliest_date);
				//datecpy(cur_w->current_date, cur_w->earliest_date);
				if (date_cmp(cur_w->current_date, picked_time) == 0)
				{
					vacant = def_vacant_date(cur_w->collection, picked_time);
				}
				cur_w = cur_w->next;
			}
		}
	}
	return vacant;
}
// ������� ������������� ��������� ���� ������
// ���������� true ���� ���� ����������
bool employment_analyzing(SYSTEMTIME &earliest_date, refferal *phead, node *p)
{ 
	bool result(false); // true ���� ���� ����������
	if (phead)
	{
		SYSTEMTIME cur_time, earliest_date_from_diff;
		GetSystemTime(&cur_time);
		GetSystemTime(&earliest_date);
		// ���������� ���� ����� ��������
		earliest_date_from_diff = timecpy(p->key.reception.time[0]);
		diff_compensate(p->key.reception.work_day, earliest_date_from_diff);
		// ��������� ��� ���� �� ��������� ����� ���� �������� ����� �� ���������� ��� ���� � ����� ���������� �� ������
		bool need_compare = vacant_day_analyzing(earliest_date_from_diff, phead, p);

		bool result(false); // true ���� ���� ����������
		busy_day *current_work = NULL; // ������ �������� ���� �����
		//svector_doc search; // ������ �������� ���������� � �����

		//search_doc_name(p, search, docname); // ���� �����

		refferal *cur = phead; // ������ �����������
		do // ���� ������� ����������� � ������� �����
		{	// ���� ����� �������, �� ��������� ������ �������� ����
			if (string_cmp(p->key.name, cur->namedoctor)) create_reception(&current_work, cur);
			cur = cur->next;
		} while (cur != phead);

		busy_day *cur_w = current_work;
		while (cur_w)
		{
			sort_refcollect(&(cur_w)->collection);
			cur_w = cur_w->next;
		}

		cur_w = current_work;
		if (current_work) // ���� ������ ������, �� ���������� ��������� ���� ������
		{
			while (cur_w)
			{
				cur_w->is_full = !def_earliest_date(p, cur_w->collection, cur_w->earliest_date);
				datecpy(cur_w->current_date, cur_w->earliest_date);
				cur_w = cur_w->next;
			}
		}
		sort_busyday(&current_work); // ��������� ������ �� ����, ��� ����������� ����� ��������� ����
		// ������ �� ������ ���� �� �������� ��������� ����, ���� �� ������, �� �������� � ����� ���� ����� �������� �����
		cur_w = current_work;
		while (cur_w)
		{
			if (!cur_w->is_full)
			{// ���������� ���� �������� �� ������� ���� ���������
				//if (date_cmp(cur_w->earliest_date, cur_time) >= 0 && date_cmp(cur_w->earliest_date, cur_time) <= 1)// && time_cmp(cur_w->earliest_date, cur_time) >= 0)
				//{
				if (date_cmp(cur_w->earliest_date, cur_time) == 0)
				{
					earliest_date = timecpy(cur_w->earliest_date);
					datecpy(cur_w->earliest_date, earliest_date);
					del_busy_time(&current_work);
					return true;
				}
				else
				{
					if (date_cmp(cur_w->earliest_date, cur_time) == 1)
					{
						int diff = num_of_week_in_year(cur_w->earliest_date) - num_of_week_in_year(cur_time); // ������� ��������� ������ �������
						if (diff > 2)
						{
							earliest_date = timecpy(p->key.reception.time[0]);
							diff_compensate(p->key.reception.work_day, earliest_date);
						}
						else
						{
							earliest_date = timecpy(cur_w->earliest_date);
							datecpy(cur_w->earliest_date, earliest_date);
							del_busy_time(&current_work);
						}
						if (need_compare) 
						{
							if (date_cmp(earliest_date, earliest_date_from_diff) == 1)
							{
								earliest_date = earliest_date_from_diff;
							}
						}
						return true;
					}
				}
				//}
			}
			else
			{
				if (!cur_w->next)
				{
					earliest_date = timecpy(cur_w->earliest_date);
					datecpy(cur_w->earliest_date, earliest_date);
					result = false; // ����� ��������� ������� ����������� ��� ������������ ��������� ������ ������� ���������
				}
			}
			cur_w = cur_w->next;
		}

		del_busy_time(&current_work); // ����������� ������ �� ������
	}
	return result;
}
/* ==== ������� ������ ������ ��� ������� ������ ===== */
// ������� ������ ������� � ����������� �� ���������
void pat_out(svector_pat search)
{
	// ������ ��� ������������ �����
	const short COUNT_COL(5); // ����� ��������
	string name_col[] = { "���.�����", "��� ", "���� ��������", "�����", "����� ������" }; // ����� ��������
	short size_col[COUNT_COL] = { 11, 34, 14, 65, 30 }; // ������� ��������

	short size_tab = print_head(name_col, size_col, COUNT_COL); // ����� ����� �������
	for (unsigned int i = 0; i < search.size(); i++) // ����� ��������� �������
	{
		cout << "|";
		print_row(search[i]->pat.key, size_col[0], search[i]->pat.key.length());
		print_row(search[i]->pat.name, size_col[1], search[i]->pat.name.length());
		cout << "  ";
		search[i]->pat.bday[0] < 10 ? cout << "0" << search[i]->pat.bday[0] : cout << search[i]->pat.bday[0];
		search[i]->pat.bday[1]< 10 ? cout << ".0" << search[i]->pat.bday[1] : cout << "." << search[i]->pat.bday[1];
		cout << "." << search[i]->pat.bday[2] << " |";
		print_row(search[i]->pat.adress, size_col[3], search[i]->pat.adress.length());
		print_row(search[i]->pat.workplace, size_col[4], search[i]->pat.workplace.length());
		cout << endl;
	}
	print_end(size_tab); // ����� ������ ������� �������
}
// ������� ������ ������ ����������� ������
void print_doc_list(node *p, refferal *phead, string key_patient)
{
	int i(1);
	refferal *current = phead;
	svector_doc search;
	cout << "������ ����������� ������: \n";
	if (current)
	{
		do
		{
			
			if (current->key_patient == key_patient)
			{
				search_doc_name(p, search, current->namedoctor);
				cout << "\t" << i << ") " << current->namedoctor << ", " << search[0]->key.noun <<endl;
				i++;
			}
			search.clear();
			current = current->next;
		} while (current != phead);
	}
}
// ������� ������ ������ ����������� ���������
void print_pat_list(refferal *phead, hash_tab *tab, char *namedoctor)
{
	int i(1), index(0);
	refferal *current = phead;
	cout << "������ ����������� ���������: \n";
	if (current)
	{
		do
		{
			if (!strcmp(current->namedoctor, namedoctor))
			{
				find_htab(tab, current->key_patient, index);
				cout << "\t" << i << ") " << current->key_patient
					<< " " << tab[index].pat.name << endl;
				i++;
			}
			current = current->next;
		} while (current != phead);
	}
}
/* ============= ������� ������ patient ============== */
// ������� ������ �������� �� �����
void search_pat_name(hash_tab *tab, string name, svector_pat &search)
{
	int j(0);
	for (int i = 0; i < TAB_SIZE; i++)
	{
		if (tab[i].is_empty) continue;
		else
		{
			if (string_cmp(name, tab[i].pat.name))
			{
				search.push_back(&tab[i]);
			}
		}
	}
}
// ���� ������ ���������
void pat_search_menu(svector_pat &search, hash_tab *tab, node *p, refferal *phead)
{
	int s_choice(0), index; // ����� ��������, ������ �������� � �������
	char key[SIZE_KEY]; // ���. ����� ��������
	string val; // ��� ��������

	cout << "�������� �������� ������ ��������:\n"
		<< " 1. �� ���\n"
		<< " 2. ���������������� ������\n"
		<< " 0. ������ ��������\n"
		<< "\n������� ����� ���������� ��������: ";
	s_choice = input_control(4, 0);
	switch (s_choice)
	{
	case 1:
		cout << "\n ������� ���: ";
		input_control(val);
		search_pat_name(tab, val, search);
		pat_out(search);
		break;
	case 2:
		cout << "\n ������� ��������������� ����� (MM-MMMMMM): ";
		key_input(key);
		if (find_htab(tab, key, index))
		{
			search.push_back(&tab[index]);
			pat_out(search);
			print_doc_list(p, phead, key);
		}
		break;
	case 0:
		break;
	}
	search.clear();
	search.shrink_to_fit();
}
/* ============= ������� ������ doctor =============== */
// ������� ������ ����������� ������ ������
void doc_out(svector_doc search, refferal *ref, hash_tab *phead, bool patlist = false)
{
	if (patlist) // ���� ��������� ������ ����������� ���������, ��
	{
		for (unsigned int i = 0; i < search.size(); i++)
		{
			cout << "\t"
				<< i + 1 << ") "
				<< search[i]->key.name << ", "
				<< search[i]->key.noun << ", �������: "
				<< search[i]->key.cabinet << ", ������ ������: ";
			show_reception(search[i]->key.reception);
			print_pat_list(ref, phead, search[i]->key.name);
			cout << endl;
		}
	}
	else // ����� ������� ������� ����������
	{
		// ������ ��� ������������ �����
		const short COUNT_COL(5); // ����� ��������
		svector_doc pointers;
		string name_col[] = { " ## ", "��� ", "���������", "� ���", " ������ ������ " }; // ����� ��������
		short size_col[COUNT_COL] = { 4, 34, 11, 7, 78 }; // ������� ��������

		short size_tab = print_head(name_col, size_col, COUNT_COL); // ����� ����� �������
		for (unsigned int i = 0; i < search.size(); i++) // ����� ��������� �������
		{
			cout << "|";
			print_row(i + 1, size_col[0], int_size(i + 1));
			print_row(search[i]->key.name, size_col[1], strlen(search[i]->key.name));
			print_row(search[i]->key.noun, size_col[2], search[i]->key.noun.length());
			print_row(search[i]->key.cabinet, size_col[3], int_size(search[i]->key.cabinet));
			show_reception(search[i]->key.reception);
		}
		print_end(size_tab); // ����� ������ ������� �������
	}
}
// ���� ������ ������
void doc_search_menu(node *p, hash_tab *phead, refferal *ref, svector_doc &search)
{
	int s_choice(0); // �������� ������
	string val; // ��� �����

	cout << "�������� �������� ������ �����:\n"
		<< " 1. �� ���\n"
		<< " 2. �� ���������\n"
		<< " 0. ������ ��������\n"
		<< "\n������� ����� ���������� ��������: ";
	s_choice = input_control(4, 0);
	switch (s_choice)
	{
	case 1:
		cout << "\n ������� ���: ";
		input_control(val);
		search_doc_name(p, search, val); // ����� �� ���
		doc_out(search, ref, phead, true);
		break;
	case 2:
		cout << "\n ������� �������� �����: ";
		input_control(val);
		search_doc_noun(p, search, val); // ����� �� ���������
		doc_out(search, NULL, NULL, false);
		break;
	case 0:
		break;
	}
	search.clear();
	search.shrink_to_fit();
}
/* ================ �������� �������� ================ */
// ���� �������� �����
void doc_del_menu(node **p, svector_doc &search, refferal **phead, refferal **ptail)
{
	int choice(0); // ����� �����
	string answer; // ����� �� ������ ��/���
	string val; // ��� �����
	refferal *current = *phead;
	svector_ref search_r; // ������ ��������� ������� 

	cout << "\n ������� ������� �����: ";
	input_control(val);
	search_doc_name(*p, search, val); // ����� ����� �� ���

	if (!search.empty()) // ���� ����� ���-��, ��
	{
		if (search.size() >= 2) // ���� ������ ������ ��� 1 ����, ��
		{
			doc_out(search);
			cout << "�������� ����� ������ ��� ����������� ��������: ";
			choice = input_control(search.size(), 1);
		}
		else choice = 1; // ����� ����� ������, ���� ������ ����� ����?

		cout << "\n������� ������: "
			<< search[choice - 1]->key.name << " "
			<< search[choice - 1]->key.noun << "\n";

		cout << "\n����������� �� �� ������� ������? �/� ";
		answer_control(answer);
		if (answer[0] == '�')
		{
			/* ===== �������� ���� ������� �� ������� ����� ====== */
			do
			{
				if (string_cmp(current->namedoctor, search[choice - 1]->key.name))
					search_r.push_back(current);
				current = current->next;
			} while (current != (*phead));
			if (!search.empty())
			{
				for (unsigned int i = 0; i < search_r.size(); i++)
				{
					refferal *temp = get_before(search_r[i]);
					if (search_r[i]->next == search_r[i])
					{
						delete search_r[i];
						*phead = *ptail = NULL;
					}
					else
					{
						temp->next = search_r[i]->next;
						(*ptail) = get_before(*phead);
						delete search_r[i];
					}
				}
			}
			/* =================================================== */
			*p = del_node(*p, search[choice - 1]->key); // �������� ���� ������(�����)
			cout << "\n=== ������ ������� ===\n";
		}
		else cout << "\n=== ������ �� ������� ===\n";
	}
	else cout << "\n=== ������ �� ������� ===\n";
	cout << endl;
	search.clear();
	search.shrink_to_fit();
}
// �������� �������� �� �����
void del_pat(hash_tab *tab, refferal **phead, refferal **ptail)
{
	char key[SIZE_KEY]; // ���. ����� ��������
	int index(0); // � ������ �������� � �������
	string answer; // ����� �� ������ ��/���

	refferal *current = *phead;
	svector_ref search; // ������ ��������� ������� 

	bool retry(false); // ���� ���������� ��������
	do // ����� ��������
	{
		int index(0); // ������ �������� � �������
		retry = false;
		cout << "������� ��������������� ����� ��������[MM-NNNNNN]: ";
		key_input(key);
		retry = !find_htab(tab, key, index); // ����� ����� � �������, ���� ������, �� retry = false
		if (retry) // ���� �� ������ �������, �� ��������� ����
		{
			cout << "\n�������������� ��������������� �����!\n";
			htab_list_query(tab); // ������ �� ����� ������� ���������
		}
		else // ���� ������ �������, ��
		{
			cout << "������ �������: "
				<< tab[index].pat.name << " ";
			tab[index].pat.bday[0] < 10 ? cout << "0" << tab[index].pat.bday[0] : cout << tab[index].pat.bday[0];
			tab[index].pat.bday[1] < 10 ? cout << ".0" << tab[index].pat.bday[1] : cout << "." << tab[index].pat.bday[1];
			cout << "." << tab[index].pat.bday[2] << endl;

			cout << "������� ������ � ��������? (�/�) ";
			answer_control(answer);
			if (answer[0] == '�')
			{
				/* ========= �������� ���� ������� �������� ========== */
				do
				{
					if (string_cmp(current->key_patient, key))
						search.push_back(current);
					current = current->next;
				} while (current != (*phead));

				if (!search.empty())
				{
					for (unsigned int i = 0; i < search.size(); i++)
					{
						refferal *temp = get_before(search[i]);
						if (search[i]->next == search[i])
						{
							delete search[i];
							*phead = *ptail = NULL;
						}
						else
						{
							temp->next = search[i]->next;
							(*ptail) = get_before(*phead);
							delete search[i];
						}
					}
				}
				/* =================================================== */
				del_htab(tab, key); // �������� ������ �� ���-�������
				cout << " ������ �������.\n";
			}
			else
			{
				cout << "\n=== ������� �� ������ ===\n";
				htab_list_query(tab); // ������ �� ����� ������� ���������
				retry = true;
			}
		}
	} while (retry);
}