/* ===== АВЛ-дерево (симметричный обход). Доктор ===== */
#pragma once
#include "anytools.h"
#include "patient.h"
#include "date.h"
/* =============== Структуры данных ================== */
// структура данных врача
struct doctor
{
	char name[NAME_DOC_SIZE]; // фамилия и инициалы
	string noun; // должность
	unsigned int cabinet; // номер кабинета
	date reception; // график работы
};
// узел дерева
struct node
{
    doctor key; // ключ узла дерева
    int height; // высота поддерева с корнем в данном узле
    node* left; // указатель на левое поддерево
    node* right;// указатель на правое поддерево
};
// вектор найденных данных, помещенных функцией поиска
typedef vector<struct node*> svector_doc;
/* =================== АВЛ-дерево ==================== */
// возвращаем высоту узла 
// если узел не существует, то возвращаем нуль
int get_height(node* p) 
{
	if (p) return p->height;
	else return NULL;
}
// возвращаем разницу высот правого и левого поддеревьев для заданного узла
int diff_height(node* p)
{
	int temp = get_height(p->right) - get_height(p->left);
	return temp;
}
// востановление корректного значения высоты при поворотах
void normalize_height(node *p)
{
	int left(0), right(0);
	left = get_height(p->left);
	right = get_height(p->right);

	if (left > right) p->height = left + 1;
	else p->height = right + 1;
}
// малое ПРАВОЕ вращение около данного узла
node* to_right_rotate(node *p)
{
    node* rotate = p->left;
	p->left = rotate->right;
	rotate->right = p;

	normalize_height(p);
	normalize_height(rotate);
	return rotate;
}
// малое ЛЕВОЕ вращение около данного узла
node* to_left_rotate(node *p) 
{
	node* rotate = p->right;
	p->right = rotate->left;
	rotate->left = p;

	normalize_height(p);
	normalize_height(rotate);
	return rotate;
}
// балансировка узла p
node* balance_tree(node *p)  
{
	normalize_height(p);

	if (diff_height(p) == 2) // если разница высот правого и левого поддеревьев для заданного узла = 2, то
    {
		if (diff_height(p->right) < 0) // смотрим отрицательна ли разница высот для правого поддерева
			p->right = to_right_rotate(p->right); // то осуществляем правый малый поворот
		return to_left_rotate(p); // совершаем правый малый поворт для баланса
    }
	if (diff_height(p) == -2) // если разница высот правого и левого поддеревьев для заданного узла = -2, то
	{
		if (diff_height(p->left) > 0) // смотрим положительна ли разница высот для левого поддерева
			p->left = to_left_rotate(p->left); // то осуществляем правый малый поворот
		return to_right_rotate(p); // совершаем левый малый поворт для баланса
	}

	return p;
}
// добавление нового узела в дерево
node* add_node(node *p, doctor k)  
{
	if (!p) // если дерево пустое, то создадим первый узел
	{
		node *temp = new node;
		temp->key = k;
		temp->height = 1;
		temp->right = NULL;
		temp->left = NULL;
		return temp;
	}
	if (toupperstr(k.name) < toupperstr(p->key.name)) // если поле ключа нового узла меньше поля ключа уже имеющегося узла
	{
		p->left = add_node(p->left, k); // то вставляем в правое поддерево
	}
	else
	{
		p->right = add_node(p->right, k); // иначе в левое
	}
	return balance_tree(p); // т.к. все рекурсивно, то обязательно делаем балансировку
}
// поиск узла с минимальным значением в поддеревьях узла p 
node* get_min_node(node *p) 
{
	if (p->left) return get_min_node(p->left);
	else return p;
}
// "удаление" узла с минимальным значением из дерева p
// происходит на самом деле смещение элементов, ибо узел минимальным ключом 
// сменит место дислокации
node* del_min(node *p) 
{
    if(!p->left) return p->right;

	p->left = del_min(p->left);

	return balance_tree(p);
}
// поиск и удаление узла по имени врача
node* del_node(node *p, doctor k) 
{
    if(!p) return NULL; // если узел отсутствует, то возвращаем нуль

	if (toupperstr(k.name) < toupperstr(p->key.name)) // если задданый ключ меньше ключа текущего узла
	{
		p->left = del_node(p->left, k); // то спускаемся налево
	}
	else
	{
		if (toupperstr(k.name) > toupperstr(p->key.name)) // если задданый ключ больше ключа текущего узла
		{
			p->right = del_node(p->right, k); // то спускаемся направо
		}
		else //if(string_cmp(k.name, p->key.name)) // если мы нашли нужный узел, то удаляем его и балансируем дерево
		{
			node* Left = p->left; // запоминаем левое поддерево
			node* Right = p->right; // запоминаем правое поддерево
			delete p; // удаляем узел

			if (!Right) return Left; // правого поддерева нет, то возвращаем правое поддерево
			node* min = get_min_node(Right); // иначе ищем узел с минимальным ключом и возвращаем его
			min->right = del_min(Right); // т.к. будем его возвращать, то придется затереть его прошлое место обитания в правом поддереве
			min->left = Left; // присваиваем левое поддерево, которые запомнили 
			p = balance_tree(min); // балансируем
			//return del_node(p, k); // вызываем удаление, для удаления повторяющихся узлов.
		}
	}
	return balance_tree(p); // балансируем
}
// вывод в виде дерева, онли дэбаг
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
// собираем все указатели узлов в один массив. Используем симметричный обход
void get_all_pointers(svector_doc &temp, node *p) 
{
	if (p->left) get_all_pointers(temp, p->left);
	temp.push_back(p);
	if (p->right) get_all_pointers(temp, p->right);
}
// удаляем все узлы дерева р
void remove_all_doc(node **p) 
{
	if (*p) // если дерево есть, то
	{
		svector_doc temp;
		get_all_pointers(temp, *p); // собираем все указатели узлов в один массив.

		for(unsigned int i=0; i<temp.size(); i++) // освобождаем память 
		{
			delete temp[i];
		}

		temp.clear();
		temp.shrink_to_fit();
	}
	*p = NULL; // присваиваем указателю нуль, ибо все уже удалено
}
/* ========= Функции инициализации графика =========== */
// заполнение рабочих дней графика врача
void fill_workday(doctor &doc)
{
	int index(0), day_count(0);
	bool fail(false);
	short val;
	// первоначально инициализируем массив рабочих дней
	for (index = 0; index < WORKDAY_SIZE; index++)
		doc.reception.work_day[index] = NON_WEEK_DAY;

	cout << "Введите количество рабочих дней врачах[1-5]: ";
	day_count = input_control(5, 1);

	cout << "Выберите рабочие дни врача:\n"
		<< " 1. Понедельник\n"
		<< " 2. Вторник\n"
		<< " 3. Среда\n"
		<< " 4. Четверг\n"
		<< " 5. Пятница\n"
		<< " 6. Суббота\n"
		<< " 7. Воскресенье\n\n";
	// заполняем массив рабочих дней
	for (index = 0; index < day_count; index++)
	{
		cout << "Выберите день недели #" << index + 1 << ": ";
		do
		{
			if (fail) cout << "Данный день недели есть в графике работы, выберете другой: ";
			val = input_control(7, 0);
			fail = check_copyday(doc.reception.work_day, val); // проверка на повторяющиеся дни недели
		} 
		while (fail);
		if (val == 7) val -= 7; // воскресенье имеет № 0, понедельник - № 1, ..., суббота - № 6
		doc.reception.work_day[index] = val;
		//if (doc.reception.work_day[index] == 7) doc.reception.work_day[index] -= 7;
	}
	sort_worday(doc.reception.work_day); // сортировка дней недели в массиве рабочих дней
}
// заполнение времени работы графика врача
void fill_worktime(doctor &doc)
{
	bool fail(false);
	SYSTEMTIME temp;

	cout << "Часы работы врача(не более 8 часов): ";
	cout << " Введите время начала раб. дня(9 - 15): "
		<< "\n  Часы: ";
	doc.reception.time[0].wHour = input_control(15, 9);
	cout << "  Минуты: ";
	doc.reception.time[0].wMinute = input_control(59, 0);

	do
	{
		fail = false;
		cout << " Введите время окончания раб. дня(15 - 20): "
			<< "\n  Часы: ";
		doc.reception.time[1].wHour = input_control(20, 15);
		cout << "  Минуты: ";
		doc.reception.time[1].wMinute = input_control(59, 0);
		/*if (doc.reception.time[1].wHour <= doc.reception.time[0].wHour)
			fail = true;
		else
		{/**/
		temp = diff_time(doc.reception.time[1], doc.reception.time[0]); // если разница в часах > 8 часов, то
		if (temp.wHour > 8)
		{
			fail = true;
			//if (fail) 
			cout << "Ошибка. Повторите ввод!\n";
		}
	} while (fail);
}
// вывод рабочих дней графика врача
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
/* ========= Функции инициализации Доктора =========== */
// функция заполнения данных о враче
doctor doc_init()
{
	doctor temp;
	bool fail(false);
	cout<<"Введите фамилию и инициалы врача: ";
	do
	{
		fail = false;
		input_control(temp.name);
		if (strlen(temp.name) <= 3)
		{
			cout << "Ошибка ввода данных! Повторите ввод: ";
			fail = true;
		}
	} while (fail);
	cout<<"Введите должность врача: ";
	input_control(temp.noun);
	cout<<"Введите кабинет врача: ";
	temp.cabinet = input_control(999, 1);
	fill_workday(temp);
	fill_worktime(temp);

	return temp;
}
// функция вывода строк таблицы с информацией о врачах
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
// функция вывода таблицы с информацией о врачах
void doc_out(node *p)//, int &i)
{
	int i(0); // номер записи в столбце ##
	// данные для формирования шапки
	const short COUNT_COL(5); // число столбцов
	string name_col[] = { " ## ", "ФИО ", "Должность", "№ каб", " График работы " }; // имена столбцов
	short size_col[COUNT_COL] = { 4, 34, 11, 7, 84 }; // размеры столбцов

	short size_tab = print_head(name_col, size_col, COUNT_COL); // вывод шапки таблицы
	view_members(p, i, size_col);// pointers); // вывод элементов таблицы
	print_end(size_tab); // вывод нижней границы таблицы
}
// функция вывод списка врачей при поиске
void doc_out(svector_doc search)
{
	int j(0);
	for (unsigned int i = 0; i < search.size(); i++)
	{
		j++;
		cout << j << ") "
			<< search[i]->key.name << ", "
			<< search[i]->key.noun << ", № кабинета: "
			<< search[i]->key.cabinet << " " << endl;
	}
}
/* =============== Работа с файлом =================== */
// считываем данные из файла и заполняем ими АВЛ-дерево
void read_doc_base(node **p)
{
	char flag; // флаг ограничивающий считывание рабочих дней
	fstream fin;
	fin.open(_DOC_FILENAME, ios_base::in);

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
				doctor temp;
				// первоначально инициализируем массив рабочих дней
				for (int k = 0; k < WORKDAY_SIZE; k++)
					temp.reception.work_day[k] = NON_WEEK_DAY;

				fin.getline(temp.name, 1000, '\n');
				fin >> temp.noun
					>> temp.cabinet;
				int j(0);
				while (true)
				{
					fin.get(flag);
					if (flag != '#') fin.putback(flag); // считываем пока не встретим флаг
					else break;
					fin >> temp.reception.work_day[j];
					j++;
				}
				for (int k = 0; k < 2; k++) // считываем время приема врача
				{
					fin >> temp.reception.time[k].wHour;
					fin >> temp.reception.time[k].wMinute;
				}
				if (!fin.eof()) fin.get();
				*p = add_node(*p, temp);
			}
		}
	}
	else { cout << "\nВнимание: Не удалось открыть Базу Данных докторов!\n\n"; system("pause"); }
	fin.sync();
	fin.clear();
	fin.close();
}
// вывод всех узлов дерева в файл методом симметричного обхода
void doc_to_file(fstream &fout, node *p)
{
	if (p)
	{
		if (p->left) 
			doc_to_file(fout, p->left);

		fout << '\n' << p->key.name
			 << '\n' << p->key.noun
			 << ' ' << p->key.cabinet << ' ';
		// записываем рабочие дни графика приема врача
		for (int i = 0; p->key.reception.work_day[i] < NON_WEEK_DAY && i < NON_WEEK_DAY - 1; i++)
		{
			fout << p->key.reception.work_day[i];
			p->key.reception.work_day[i + 1] < NON_WEEK_DAY ? fout << ' ' : fout;
		}
		fout << "# "; // добавляем флаг конца рабочих дней
		for (int i = 0; i < 2; i++) // добавляем время приема врача
		{
			fout << p->key.reception.time[i].wHour
				<< ' ' << p->key.reception.time[i].wMinute;
			i == 0 ? fout << ' ' : fout;
		}

		if (p->right) 
			doc_to_file(fout, p->right);
	}
}
// вывод дерева в файл
void fill_doc_base(node *p)
{
	doctor temp;

	fstream fout;
	fout.open(_DOC_FILENAME, ios_base::out);
	if(fout)
	{
		fout << "==========База данных докторов===========";
		if (p) doc_to_file(fout, p);
	}
	else { cout<<"\nВнимание: Не удалось открыть Базу Данных докторов!\n\n"; system("pause"); }
	fout.close();
}
/* ============= Функции поиска doctor =============== */
// поиск врачей по ФИО
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
// поиск врачей по должности
void search_doc_noun(node *p, svector_doc &search, string noun)
{
	if (!p) return;

	if (p->left) search_doc_noun(p->left, search, noun);

	if (string_cmp(noun, p->key.noun)) search.push_back(p);

	if (p->right) search_doc_noun(p->right, search, noun);
}