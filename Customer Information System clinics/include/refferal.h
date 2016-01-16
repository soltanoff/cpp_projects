/* == Список (циклический, однонаправленный). Выдача/возврат направлений == */
#pragma once
#include "hospital.h"
/* =================== Структуры данных =================== */
// структура данных направления
struct refferal
{
	string key_patient;// ключ формата: MM-NNNNNN
	char namedoctor[SIZE_NAME];
	SYSTEMTIME time;
	refferal *next; // указатель на следующий элемент списка
};
// вектор найденных данных, помещенных функцией поиска
typedef vector<struct refferal*> svector_ref;
/* ================== Циклический список =================== */
// запрос на список пациентов
void htab_list_query(hash_tab *htab)
{
	string answer;
	cout << "Вывести список пациентов? д/н ";
	answer_control(answer);
	if (answer[0] == 'д')
	{
		pat_out(htab);
		system("pause");
	}
}
// запрос на список врачей
void doc_list_query(node *p)
{
	string answer;
	cout << "Вывести список врачей? д/н ";
	answer_control(answer);
	if (answer[0] == 'д')
	{
		doc_out(p);
		system("pause");
	}
}
// функция анализирующая занятость врача
// Возвращает true если дата определена
bool employment_analyzing(SYSTEMTIME &earliest_date, refferal *phead, node *p);
// функция анализирующая дату приема
// Возвращает true если дата не занята
bool vacant_day_analyzing(SYSTEMTIME picked_time, refferal *phead, node *p);
// функция заполнения данных о выписке
refferal *ref_init(refferal *phead, node *p, hash_tab *htab, svector_doc &search)
{
	SYSTEMTIME cur_time; // дата направления
	GetSystemTime(&cur_time); // текущая дата

	refferal *temp = new refferal; // новая выписка
	int choice(0); // выбор нужного врача
	string answer; // ответ на вопрос да/нет
	string val; // имя врача

	bool retry(false); // флаг повторения действий
	do // выбор пациента
	{
		int index(0); // индекс пациента в таблице
		retry = false;
		cout << "Введите регистрационный номер пациента[MM-NNNNNN]: ";
		key_input(temp->key_patient);
		retry = !find_htab(htab, temp->key_patient, index); // поиск ключа в таблице, если найден, то retry = false
		if (retry) // если не найден пациент, то повторить ввод
		{
			cout << "\nНесуществующий регистрационный номер!\n";
			htab_list_query(htab); // запрос на вывод таблицы пациентов
		}
		else // если найден пациент, то
		{
			cout << "\nВыбрана запись: "
				<< htab[index].pat.name << ", ";
			htab[index].pat.bday[0] < 10 ? cout << "0" << htab[index].pat.bday[0] : cout << htab[index].pat.bday[0];
			htab[index].pat.bday[1] < 10 ? cout << ".0" << htab[index].pat.bday[1] : cout << "." << htab[index].pat.bday[1];
			cout << "." << htab[index].pat.bday[2];
			cout << "\nНазначить этому пациенту врача? д/н ";
			answer_control(answer); // тот ли пациент найден?
			if (answer[0] != 'д') // если не тот, то повторяем ввод
			{
				htab_list_query(htab); // запрос на вывод таблицы пациентов
				retry = true;
			}
			else cout << "\n=== Пациент выбран ===\n";
		}
	} while (retry);

	do // выбор врача
	{
		retry = false;
		cout << "Введите ФИО врача: ";
		input_control(val);
		search_doc_name(p, search, val); // ищем врача

		if (!search.empty()) // если не нашли, то повторяем ввод
		{
			doc_out(search); // выводим список найденых врачей
			if (search.size() >= 2) // если врачей больше чем 2, то делаем запрос
			{
				cout << "Выберите номер записи: ";
				choice = input_control(search.size(), 1);
			}
			else choice = 1; // иначе врач 1, и нет смысла запрашивать выбор врача

			cout << "\nВыбрана запись: "
				<< search[choice - 1]->key.name << " "
				<< search[choice - 1]->key.noun << "\n";

			cout << "\nНазначить пациенту этого врача? д/н ";
			answer_control(answer); // тот ли врач найден?
			if (answer[0] == 'д')
			{
				strcpy(temp->namedoctor, search[choice - 1]->key.name);
				cout << "\n=== Врач назначен ===\n";
			}
			else // если не тот, то повторяем ввод
			{
				search.clear();
				doc_list_query(p); // запрос на вывод таблицы врачей
				retry = true;
			}
		}
		else
		{
			cout << "\n=== Врач не найден! ===\n";
			doc_list_query(p);
			retry = true; // если не нашли, то повторяем ввод
		}
	} while (retry);

	show_reception(search[choice - 1]->key.reception); // вывод графика работы врача

	SYSTEMTIME time; // Ближайшее время приема
	GetSystemTime(&time);
	bool picked_time(false); // выбрана ли определенная анализатором ближайшая дата?
	bool analyz_success = employment_analyzing(time, phead, search[choice - 1]); // если y врача заняты дни, то нужно определить границы
	bool time_control(false); // нужны ли границы времени НН:ММ
	int hour(0), minute(0); // границы времени НН:ММ
	if (analyz_success)
	{
		cout << "Ближайшее время приема: "
			<< time.wDay << " "
			<< get_month(time) << " "
			<< time.wYear << " - ";
		time.wHour < 10 ? cout << "0" << time.wHour : cout << time.wHour;
		cout << ":";
		time.wMinute < 10 ? cout << "0" << time.wMinute : cout << time.wMinute;
		cout << endl;
		cout << "\nНазначить пациента эту дату? д/н ";
		answer_control(answer);
		if (answer[0] == 'д')
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
		do // назначение даты приема
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
			cout << "\nВрач может принять начиная с: "
				<< time.wDay << " "
				<< get_month(time) << " "
				<< time.wYear << " - ";
			time.wHour < 10 ? cout << "0" << time.wHour : cout << time.wHour;
			cout << ":";
			time.wMinute < 10 ? cout << "0" << time.wMinute : cout << time.wMinute;
			cout << endl;
			short min_day(0), min_month(0); // минимальный допустимый день и месяц
			retry = false;
			cout << "Введите дату приема: "
				<< "\n  Год: ";
			temp->time.wYear = input_control(cur_time.wYear + 200, cur_time.wYear);
			if (temp->time.wYear > cur_time.wYear) // если введенный год больше текущего, то граница = 1
			{
				min_day = 1;
				min_month = 1;
			}
			else // иначе границы = текущая дата
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
			cout << "  Номер месяца: ";
			temp->time.wMonth = input_control(12, min_month);
			if (temp->time.wMonth > min_month) min_day = 1;
			cout << "  День: ";
			temp->time.wDay = input_control(31, min_day);

			int week = what_day(temp->time); // определяем по дате день недели
			for (int i = 0; i < WORKDAY_SIZE; i++)
			{
				if (week != search[choice - 1]->key.reception.work_day[i]) retry = true; // если дня недели нет в графике работы, то ищем дальше
				else { retry = false; break; } // если нашли день недели в графике, то завершаем поиск
			}
			if (retry) cout << "В это время врач не работает!\n";
		} while (retry);

		do
		{
			retry = false;
			cout << "Введите время приема: "
				<< "\n  Часы: ";
			temp->time.wHour = input_control(20, 9);
			cout << "  Минуты: ";
			temp->time.wMinute = input_control(59, 0);

			if (!is_time_visit(search[choice - 1]->key.reception.time, temp->time)) retry = true;

			if (retry) cout << "В это время врач не работает!\n";
			if (analyz_success && !vacant_day_analyzing(temp->time, phead, search[choice - 1]))
			{
				cout << "Это время занято!\n";
				retry = true;
			}
		} while (retry);
	}
	cout << endl;
	search.clear();
	search.shrink_to_fit();
	return temp;
}
// функция вывода таблицы с информацией о выписках
void print_list(refferal *phead, hash_tab *pat)
{
	int i(0), index(0);
    refferal *current = phead;
	// данные для формирования шапки
	const short COUNT_COL(5); // число столбцов
	string name_col[] = { " ## ", "Рег.номер", "ФИО пациента", "ФИО врача ", "Дата приема" }; // имена столбцов
	short size_col[COUNT_COL] = { 4, 11, 38, 28, 30 }; // размеры столбцов

	short size_tab = print_head(name_col, size_col, COUNT_COL); // вывод шапки таблицы
    // вывод строк таблицы
	if (!current) cout<<"Список пуст!\n";
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
	print_end(size_tab); // вывод нижней границы таблицы
}
// подсчет кол-ва выданных направлений (нужно для сортировки)
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
// получаем адрес предыдущего элемента для р
refferal *get_before(refferal *p)
{
	refferal *current = p;
	while(current->next != p)
		current = current->next;
	return current;
}
// сортировка списка методом извлечения
void sort_list(refferal **phead, refferal **ptail)
{
	if (*phead) // если список существует, то
	{
		int count = list_count(*phead); // получаем кол-во элементов списка
		if (count > 2) // если элементов больше чем 2
		{
			refferal *link; // звено для реализации перестановки элементов
			refferal *max; // элемент с максимальным значением(поле ФИО врача)
			refferal *before_m; // элемент стоящий за *max
			refferal *tail; // элемент указывающий на границу рассматриваемых элементов списка
			refferal *before_t; // элемент стоящий за *tail
			refferal *current = *phead;

			tail = *ptail; // начальная граница: конец списка
			// рассматриваем элементы списка, причем с каждым разом кол-во рассмотренных элементов уменьшается на 1
			for(int i = count; i > 0; i--)
			{
				max = *phead;
				current = (*phead);
				// определяем максимальный элемент
				for(int j = 0; j < i; j++)
				{
					if (toupperstr(current->namedoctor) >= toupperstr(max->namedoctor))
						max = current;
					current = current->next;
				}

				before_m = get_before(max); // элемент стоящий за *max
				before_t = get_before(tail); // элемент стоящий за *tail
			
				if (max != tail) // если хвост равен максимальному элементу, то пропускаем его и смещаемся, иначе
				{
					if (max == *phead && tail == *ptail) // если максимальный элемент первый и граница равна концу списка, то
					{// меняем элементы местами
						before_t->next = max; 
						link = max->next;
						max->next = tail;
						tail->next = link;

						*phead = tail;
					}
					else // иначе
					{
						if (max->next == tail) // если максимальный элемент предпоследний, то
						{// реализуем смещение
							link = tail->next;
							before_m->next = tail;
							tail->next = max;
							max->next = link;
							before_t = tail;
							if (max == *phead) *phead = tail;

						}
						else // иначе
						{// реализуем смещение
							before_t->next = max;
							link = max->next;
							max->next = tail->next;
							tail->next = link;
							before_m->next = tail;
							if (max == *phead) *phead = tail;
						}
					}
				}
				tail = before_t; // смещаем границу рассматриваемой области
				*ptail = get_before(*phead); // обновляем хвост списка
			}
		}
		else // если элементов меньше чем 2, то
		{// реализуем смещение
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
// функция регистрации направления
void add_to_list(refferal **phead, refferal **ptail, node *p, hash_tab *htab, svector_doc &search)
{
    refferal *current = ref_init(*phead, p, htab, search); // заполняем данные о выписке
	current->next = NULL;

    if (!(*phead)) *phead = current;
    else (*ptail)->next = current;
    (*ptail) = current;

	(*ptail)->next = *phead; // зацикливаем список
	sort_list(phead, ptail); // сортируем
}
// функция добавления направления при чтении из файла
void add_to_list(refferal **phead, refferal **ptail, refferal *ref)
{
	refferal *current = ref;
	current->next = NULL;

	if (!(*phead)) *phead = current;
	else (*ptail)->next = current;
	(*ptail) = current;

	(*ptail)->next = *phead; // зацикливаем
	sort_list(phead, ptail); // сортируем
}
// удаление одной выписки из списка 
void del_ref(refferal **phead, refferal **ptail, svector_ref &search, string val, bool key = false)
{
	int choice(0); // номер элемента на удаление
	string answer; // ответ на запрос да/нет
	refferal *current = *phead;

	if (key) // если поиск элемента на удаление идет по ключу, то ищем по ключу
	{
		do
		{
			if (string_cmp(val, current->key_patient))
				search.push_back(current);
			current = current->next;
		} while (current != (*phead));
	}
	else // иначе ищем по фамилии врача
	{
		do
		{
			if (string_cmp(val, current->namedoctor))
				search.push_back(current);
			current = current->next;
		} while (current != (*phead));
	}
	if (!search.empty()) // если нашли, то
	{
		cout << "Результат: \n";
		for (unsigned int i = 0; i < search.size(); i++)
		{
			cout << i + 1 << ") " << "\n\tРег. номер: "
				<< search[i]->key_patient << "\n\tФИО врача: "
				<< search[i]->namedoctor << "\n\tДата приема: "
				<< " "
				<< search[i]->time.wDay << " "
				<< get_month(search[i]->time) << " "
				<< search[i]->time.wYear << endl;
		}
		if (search.size() >= 2) // если найденных записей больше чем 2, то запрашиваем
		{
			cout << "Выберите номер записи для дальнейшего удаления: ";
			choice = input_control(search.size(), 1);
		}
		else choice = 1; // иначе зачем выбирать если запись одна?

		cout << "\nВыбрана запись: \n Рег. номер: "
			<< search[choice - 1]->key_patient << "\n ФИО врача: "
			<< search[choice - 1]->namedoctor << "\n Дата приема: "
			<< " "
			<< search[choice - 1]->time.wDay << " "
			<< get_month(search[choice - 1]->time) << " "
			<< search[choice - 1]->time.wYear << endl;

		cout << "\nСобираетесь ли вы удалить запись? д/н ";
		answer_control(answer);
		if (answer[0] == 'д')
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
			cout << "\n=== Запись удалена ===\n";
		}
		else cout << "\n=== Запись не удалена ===\n";
	}
	cout << endl;
}
// меню возврата выписки
void del_ref_menu(refferal **phead, refferal **ptail)
{
	svector_ref search; // вектор данных выписок
	int s_choice(0); // выбор критерия
	string val; // фио врача
	if (*phead) // если список существует, то 
	{
		cout << "Выберите критерий для возврата выписки:\n"
			<< " 1. ФИО врача\n"
			<< " 2. Рег. номер пациента\n"
			<< " 0. Отмена действий\n"
			<< "\nВведите номер выбранного критерия: ";
		s_choice = input_control(4, 0);
		switch (s_choice)
		{
		case 1:
			cout << "\n Введите ФИО врача: ";
			input_control(val);
			del_ref(phead, ptail, search, val);
			break;
		case 2:
			cout << "\n Введите рег. номер пациента[MM-NNNNNN]: ";
			key_input(val);
			del_ref(phead, ptail, search, val, true);
			break;
		case 0:
			break;
		}
	}
	else cout << " Список пуст.\n"; // иначе сообщаем об отсутсвии элементов
}
// удаление и освобождение памяти списка
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
/* =============== Работа с файлом =================== */
// считываем данные из файла и заполняем ими список
void read_ref_base(refferal **phead, refferal **ptail)
{
	fstream fin;
	fin.open(_REF_FILENAME, ios_base::in);

	if (fin)
	{
		// смотрим на количество символов в файле
		fin.seekg(0, ios_base::end); // смещаемся в конец
		int j = int(fin.tellg()); // получаем количество символов, которые мы прошли при продвежению к концу файла
		fin.seekg(43, ios::beg); // смещаемся в начало файла пропуская шапку
		// 43 - размер шапки файла, если в файле больше 43 символов, то
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
				add_to_list(phead, ptail, temp); // заполняем список
				if (!fin.eof()) fin.get();
			}
		}
	}
	else { cout << "\nВнимание: Не удалось открыть Базу Данных направлений!\n\n"; system("pause"); }
	fin.sync();
	fin.clear();
	fin.close();
}
// вывод всех элементов списка в файл
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
// вывод списка в файл
void fill_ref_base(refferal *phead)
{
	fstream fout;
	fout.open(_REF_FILENAME, ios_base::out);
	if(fout)
	{
		fout << "==========База данных выписок============";
		if (phead) ref_to_file(fout, phead);
	}
	else { cout<<"\nВнимание: Не удалось открыть Базу Данных Выписок!\n\n"; system("pause"); }
	fout.close();
}