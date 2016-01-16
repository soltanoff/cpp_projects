/* ============ Поиск во всех структурах ============= */
#pragma once
#include "doctor.h"
#include "patient.h"
#include "refferal.h"
#include "anytools.h"
/* ======= Определения ближайшей даты выписки ======= */
// коллекция выписок
struct ref_collect
{
	refferal *ref; // указатель на выписку
	ref_collect *next; // указатель на следующий элемент коллекции
	ref_collect(refferal *reffer) // параметрический конструктор
	{
		ref = reffer;
		next = NULL;
	}
};
// список приемных дней врача
struct busy_day
{
	bool is_full; // заполнен ли день
	SYSTEMTIME current_date; // текущая дата
	SYSTEMTIME earliest_date; // ближайшее время на прием
	ref_collect *collection; // текущие пациенты на этот день
	busy_day *next; // указатель на следующую структуру
	busy_day(refferal *reffer) // параметрический конструктор
	{
		next = NULL;
		is_full = false;
		current_date = datecpy(reffer->time);
		collection = new ref_collect(reffer);
	}
};
// освобождение памяти от списка приемных дней врача
// как создали, так и удаляем
void del_busy_time(busy_day **current_work)
{
	busy_day *cur_w = *current_work;
	if (*current_work)
	{
		busy_day *cur_w = *current_work;
		while (*current_work)
		{
			// удаляем коллекцию выписок
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
			// удаляем элемент списка
			*current_work = cur_w->next;
			delete cur_w;
		}
	}
}
// функция копирования адресов ячеек памяти всех элементов списка Т в вектор 
template<class T>void trans_to_vector(T *phead, vector<T*> &vec)
{
	T *current = phead;
	while (current)
	{
		vec.push_back(current);
		current = current->next;
	}
}
// функция переопределения указателей списка Т согласно упорядочности вектора
template<class T>void back_links(T **phead, vector<T*> &vec)
{
	*phead = vec[0];
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (i == vec.size() - 1) vec[i]->next = NULL;
		else vec[i]->next = vec[i + 1];
	}
}
// функция сортировки коллекции направлений врача по времени
// сортировка усовершенственным пузырьком :D
void sort_refcollect(ref_collect **ref)
{
	if (*ref)
	{
		vector<ref_collect*> vec; // вектор хранящий в себе элементы списка
		bool changed(true); // флаг изменений в векторе(были ли перестановки элементов)
		ref_collect *temp; // временный элемент(для перестановки)
		// копируем адреса элементов из списка в вектор
		trans_to_vector(*ref, vec);
		// сортируем пока есть какие-либо изменения
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
		// реконструируем список упорядочными элементами
		back_links(ref, vec);
	}
}
// функция сортировки список приемных дней врача по дате
// сортировка усовершенственным пузырьком :D
void sort_busyday(busy_day **current_work)
{
	if (*current_work)
	{
		vector<busy_day*> vec; // вектор хранящий в себе элементы списка
		bool changed(true); // флаг изменений в векторе(были ли перестановки элементов)
		busy_day *temp; // временный элемент(для перестановки)
		// копируем адреса элементов из списка в вектор
		trans_to_vector(*current_work, vec);
		// сортируем пока есть какие-либо изменения
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
		// реконструируем список упорядочными элементами
		back_links(current_work, vec);
	}
}
// функция создания списка приемных дней врача
void create_reception(busy_day **current_work, refferal *ref)
{
	if (!(*current_work)) // если список пуст, то создадим первый элемент
	{
		(*current_work) = new busy_day(ref);
	}
	else// иначе
	{
		bool writted(false); // флаг указываеющий на то, что была произведена запись
		busy_day *cur_w = *current_work;
		while (cur_w) // гуляем по списку пока не дойдем до конца
		{
			if (date_cmp(cur_w->current_date, ref->time) == 0) // если дата выписки совпадает с полем списка приемных дней, то
			{// то пополняем коллекцию выписок
				ref_collect *cur_r = cur_w->collection;
				while (cur_r->next)
					cur_r = cur_r->next;
				cur_r->next = new ref_collect(ref);
				writted = true;
				break;
			}
			cur_w = cur_w->next;
		}
		if (!writted) // если выписка не попала ни в какую коллекцию, то создаем новый элемент списка приемных дней
		{
			busy_day *cur_w = *current_work;
			while (cur_w->next)
				cur_w = cur_w->next;
			cur_w->next = new busy_day(ref);
		}
	}
	
}
// debug-вывод результатов формирования занятости
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
				cout << " Ближайшее время приема: "
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
// функция определяющая ближайшую дату приема пациента врачем. 
// Возвращает true если дата определена
bool def_earliest_date(node *p, ref_collect *collection, SYSTEMTIME &earliest_date)
{
	ref_collect *current = collection; // коллекция направлений врача
	bool diff_fail(false); // ошибка при подсчете разницы между датами

	SYSTEMTIME temp = diff_time(current->ref->time, p->key.reception.time[0], &diff_fail);
	if (temp.wMinute > 30 || temp.wHour >= 1) // если разница между началом раб. дня и первой выпиской больше 30 мин или более, то
	{
		earliest_date = timecpy(p->key.reception.time[0]); // определяем ближайшую дату(начало раб. дня)
		return true;
	}
	else // иначе смотрим далее
	{
		while (current->next) // гуляем по списку, пока не попадем в последний элемент
		{
			temp = diff_time((current->next)->ref->time, current->ref->time, &diff_fail);
			if (temp.wMinute > 30 || temp.wHour >= 1)// если разница между одной выпиской и другой выпиской больше 30 мин или более, то
			{
				earliest_date = timecpy(current->ref->time); // то назначем время ближайшей выписки
				datecpy(current->ref->time, earliest_date);
				half_hour_increase(earliest_date); // причем увеличиваем время на 30 мин

				temp = diff_time((current->next)->ref->time, earliest_date, &diff_fail);
				if (temp.wMinute > 30 || temp.wHour >= 1) return true; // если после увелечения время осталось с прежней разницей, то сообщаем о том, что дата найдена
			}
			current = current->next;
		}
		// если мы ничего не нашли до этого, про аналогично проверяем последний элемент
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
// функция определяющая вхождение даты приема пациента
// Возвращает true если дата не занята
bool def_vacant_date(ref_collect *collection, SYSTEMTIME picked_time)
{
	ref_collect *current = collection; // коллекция направлений врача
	bool diff_fail(false); // ошибка при подсчете разницы между датами
	while (current)
	{
		if (time_cmp(current->ref->time, picked_time) == 0) // если выбранное время пользователем совпадает с временем выписки, то
		{
			return false; // дата занята
		}
		else
		{// если выбранное время пользователем лежит в окресности (время выписки - 30 мин; время выписки + 30 мин), то дата занята
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
// функция анализирующая дату приема
// Возвращает true если дата не занята
bool vacant_day_analyzing(SYSTEMTIME picked_time, refferal *phead, node *p)
{
	bool vacant(true);
	if (phead)
	{
		SYSTEMTIME cur_time;
		GetSystemTime(&cur_time);

		bool result(false); // true если дата определена
		busy_day *current_work = NULL; // список приемных дней врача

		refferal *cur = phead; // список направлений
		do // ищем выписки относящиеся к нужному врачу
		{	// если нашли выписку, то заполняем список приемных дней
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
		if (current_work) // если список создан, то определяем ближайшую дату приема
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
// функция анализирующая ближайшую дату приема
// Возвращает true если дата определена
bool employment_analyzing(SYSTEMTIME &earliest_date, refferal *phead, node *p)
{ 
	bool result(false); // true если дата определена
	if (phead)
	{
		SYSTEMTIME cur_time, earliest_date_from_diff;
		GetSystemTime(&cur_time);
		GetSystemTime(&earliest_date);
		// определяем дату через смещения
		earliest_date_from_diff = timecpy(p->key.reception.time[0]);
		diff_compensate(p->key.reception.work_day, earliest_date_from_diff);
		// проверяем эту дату на вхождение после чего выясняем нужно ли сравнивать эту дату с датой получаемой из списка
		bool need_compare = vacant_day_analyzing(earliest_date_from_diff, phead, p);

		bool result(false); // true если дата определена
		busy_day *current_work = NULL; // список приемных дней врача
		//svector_doc search; // вектор хранящий информацию о враче

		//search_doc_name(p, search, docname); // ищем врача

		refferal *cur = phead; // список направлений
		do // ищем выписки относящиеся к нужному врачу
		{	// если нашли выписку, то заполняем список приемных дней
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
		if (current_work) // если список создан, то определяем ближайшую дату приема
		{
			while (cur_w)
			{
				cur_w->is_full = !def_earliest_date(p, cur_w->collection, cur_w->earliest_date);
				datecpy(cur_w->current_date, cur_w->earliest_date);
				cur_w = cur_w->next;
			}
		}
		sort_busyday(&current_work); // сортируем список по дате, для определения самой ближайшей даты
		// гуляем по списку пока не встретим свободный день, если не найдем, то сообщаем с какой даты можно записать врача
		cur_w = current_work;
		while (cur_w)
		{
			if (!cur_w->is_full)
			{// определяем дату опираясь на текущую дату календаря
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
						int diff = num_of_week_in_year(cur_w->earliest_date) - num_of_week_in_year(cur_time); // смотрим насколько велика разница
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
					result = false; // время последней выписки потребуется для установления начальных границ прниема пациентов
				}
			}
			cur_w = cur_w->next;
		}

		del_busy_time(&current_work); // освобождаем память от списка
	}
	return result;
}
/* ==== Функции вывода данных для функций поиска ===== */
// функция вывода таблицы с информацией об пациентах
void pat_out(svector_pat search)
{
	// данные для формирования шапки
	const short COUNT_COL(5); // число столбцов
	string name_col[] = { "Рег.номер", "ФИО ", "День рождения", "Адрес", "Место работы" }; // имена столбцов
	short size_col[COUNT_COL] = { 11, 34, 14, 65, 30 }; // размеры столбцов

	short size_tab = print_head(name_col, size_col, COUNT_COL); // вывод шапки таблицы
	for (unsigned int i = 0; i < search.size(); i++) // вывод элементов таблицы
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
	print_end(size_tab); // вывод нижней границы таблицы
}
// функция вывода списка назначенных врачей
void print_doc_list(node *p, refferal *phead, string key_patient)
{
	int i(1);
	refferal *current = phead;
	svector_doc search;
	cout << "Список назначенных врачей: \n";
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
// функция вывода списка назначенных пациентов
void print_pat_list(refferal *phead, hash_tab *tab, char *namedoctor)
{
	int i(1), index(0);
	refferal *current = phead;
	cout << "Список назначенных пациентов: \n";
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
/* ============= Функции поиска patient ============== */
// функция поиска пациента по имени
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
// меню поиска пациентов
void pat_search_menu(svector_pat &search, hash_tab *tab, node *p, refferal *phead)
{
	int s_choice(0), index; // выбор критерия, индекс пациента в таблице
	char key[SIZE_KEY]; // рег. номер пациента
	string val; // ФИО пациента

	cout << "Выберите критерий поиска пациента:\n"
		<< " 1. По ФИО\n"
		<< " 2. Регистрационному номеру\n"
		<< " 0. Отмена действий\n"
		<< "\nВведите номер выбранного критерия: ";
	s_choice = input_control(4, 0);
	switch (s_choice)
	{
	case 1:
		cout << "\n Введите ФИО: ";
		input_control(val);
		search_pat_name(tab, val, search);
		pat_out(search);
		break;
	case 2:
		cout << "\n Введите регистрационный номер (MM-MMMMMM): ";
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
/* ============= Функции поиска doctor =============== */
// функция вывода результатов поиска врачей
void doc_out(svector_doc search, refferal *ref, hash_tab *phead, bool patlist = false)
{
	if (patlist) // если требуется список назначенных пациентов, то
	{
		for (unsigned int i = 0; i < search.size(); i++)
		{
			cout << "\t"
				<< i + 1 << ") "
				<< search[i]->key.name << ", "
				<< search[i]->key.noun << ", кабинет: "
				<< search[i]->key.cabinet << ", график работы: ";
			show_reception(search[i]->key.reception);
			print_pat_list(ref, phead, search[i]->key.name);
			cout << endl;
		}
	}
	else // иначе выводим таблицу информации
	{
		// данные для формирования шапки
		const short COUNT_COL(5); // число столбцов
		svector_doc pointers;
		string name_col[] = { " ## ", "ФИО ", "Должность", "№ каб", " График работы " }; // имена столбцов
		short size_col[COUNT_COL] = { 4, 34, 11, 7, 78 }; // размеры столбцов

		short size_tab = print_head(name_col, size_col, COUNT_COL); // вывод шапки таблицы
		for (unsigned int i = 0; i < search.size(); i++) // вывод элементов таблицы
		{
			cout << "|";
			print_row(i + 1, size_col[0], int_size(i + 1));
			print_row(search[i]->key.name, size_col[1], strlen(search[i]->key.name));
			print_row(search[i]->key.noun, size_col[2], search[i]->key.noun.length());
			print_row(search[i]->key.cabinet, size_col[3], int_size(search[i]->key.cabinet));
			show_reception(search[i]->key.reception);
		}
		print_end(size_tab); // вывод нижней границы таблицы
	}
}
// меню поиска врачей
void doc_search_menu(node *p, hash_tab *phead, refferal *ref, svector_doc &search)
{
	int s_choice(0); // критерий выбора
	string val; // фио врача

	cout << "Выберите критерий поиска врача:\n"
		<< " 1. По ФИО\n"
		<< " 2. По должности\n"
		<< " 0. Отмена действий\n"
		<< "\nВведите номер выбранного критерия: ";
	s_choice = input_control(4, 0);
	switch (s_choice)
	{
	case 1:
		cout << "\n Введите ФИО: ";
		input_control(val);
		search_doc_name(p, search, val); // поиск по фио
		doc_out(search, ref, phead, true);
		break;
	case 2:
		cout << "\n Введите долность врача: ";
		input_control(val);
		search_doc_noun(p, search, val); // поиск по должности
		doc_out(search, NULL, NULL, false);
		break;
	case 0:
		break;
	}
	search.clear();
	search.shrink_to_fit();
}
/* ================ Удаление элемента ================ */
// меню удаления врача
void doc_del_menu(node **p, svector_doc &search, refferal **phead, refferal **ptail)
{
	int choice(0); // выбор врача
	string answer; // ответ на вопрос да/нет
	string val; // фио врача
	refferal *current = *phead;
	svector_ref search_r; // вектор найденных выписок 

	cout << "\n Введите фамилию врача: ";
	input_control(val);
	search_doc_name(*p, search, val); // поиск врача по фио

	if (!search.empty()) // если нашли что-то, то
	{
		if (search.size() >= 2) // если найден больше чем 1 врач, то
		{
			doc_out(search);
			cout << "Выберите номер записи для дальнейшего удаления: ";
			choice = input_control(search.size(), 1);
		}
		else choice = 1; // иначе зачем запрос, если запись всего одна?

		cout << "\nВыбрана запись: "
			<< search[choice - 1]->key.name << " "
			<< search[choice - 1]->key.noun << "\n";

		cout << "\nСобираетесь ли вы удалить запись? д/н ";
		answer_control(answer);
		if (answer[0] == 'д')
		{
			/* ===== Удаление всех выписок на данного врача ====== */
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
			*p = del_node(*p, search[choice - 1]->key); // удаление узла дерева(врача)
			cout << "\n=== Запись удалена ===\n";
		}
		else cout << "\n=== Запись не удалена ===\n";
	}
	else cout << "\n=== Запись не найдена ===\n";
	cout << endl;
	search.clear();
	search.shrink_to_fit();
}
// удаление пациента по ключу
void del_pat(hash_tab *tab, refferal **phead, refferal **ptail)
{
	char key[SIZE_KEY]; // рег. номер пациента
	int index(0); // № ячейки пациента в таблице
	string answer; // ответ на запрос да/нет

	refferal *current = *phead;
	svector_ref search; // вектор найденных виписок 

	bool retry(false); // флаг повторения действий
	do // выбор пациента
	{
		int index(0); // индекс пациента в таблице
		retry = false;
		cout << "Введите регистрационный номер пациента[MM-NNNNNN]: ";
		key_input(key);
		retry = !find_htab(tab, key, index); // поиск ключа в таблице, если найден, то retry = false
		if (retry) // если не найден пациент, то повторить ввод
		{
			cout << "\nНесуществующий регистрационный номер!\n";
			htab_list_query(tab); // запрос на вывод таблицы пациентов
		}
		else // если найден пациент, то
		{
			cout << "Найден больной: "
				<< tab[index].pat.name << " ";
			tab[index].pat.bday[0] < 10 ? cout << "0" << tab[index].pat.bday[0] : cout << tab[index].pat.bday[0];
			tab[index].pat.bday[1] < 10 ? cout << ".0" << tab[index].pat.bday[1] : cout << "." << tab[index].pat.bday[1];
			cout << "." << tab[index].pat.bday[2] << endl;

			cout << "Удалить данные о пациенте? (д/н) ";
			answer_control(answer);
			if (answer[0] == 'д')
			{
				/* ========= Удаление всех выписок пациента ========== */
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
				del_htab(tab, key); // удаление данных из хэш-таблицы
				cout << " Запись удалена.\n";
			}
			else
			{
				cout << "\n=== Пациент не выбран ===\n";
				htab_list_query(tab); // запрос на вывод таблицы пациентов
				retry = true;
			}
		}
	} while (retry);
}