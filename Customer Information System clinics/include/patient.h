/* ====== Хэш-таблица (двойное хэширование). Пациент ====== */
#pragma once
#include "anytools.h"

int _pat_count = 1; // Порядок клиентуры

/* =================== Структуры данных =================== */
// структура данных пациента
struct patient
{
	string key;// формат ключа: MM-NNNNNN
	string name; // ФИО 
	unsigned int bday[3]; // дата рождения
	string adress; // адрес
	string workplace; // место работы
};
// структура ячейки хэш-таблицы
struct hash_tab
{
	patient pat; // структура данных пациента 
	bool is_empty; // пуста ли ячейка?
};
// вектор найденных данных, помещенных функцией поиска
typedef vector<struct hash_tab*> svector_pat;
/* ================= Двойное хэширование ================== */
// первое хэширование
int hash_one(int key) { return (key % TAB_SIZE); }
// второе хэширование
int hash_two(int key) { return ((key % (TAB_SIZE - 1)) + 1); }
// функция реализующая хэширование (и 1, и 2)
int h(hash_tab *tab, string key)
{
	int result(0); // результат суммирования кодов символов ключа
	int index(0); // индекс хэш-таблицы
	for(int i=0; i < SIZE_KEY - 1; i++)
		result += int(key[i])*int(pow(3.0, i));
	int h_1 = hash_one(result),
		h_2 = hash_two(result);
	// формируем начальный индекс на основе первого хэширования
	//index = h_1;
	for(int i=0; i < TAB_SIZE; i++)
	{
		// формируем индекс на основе пустоты ячейки
		index = (h_1 + i*h_2) % TAB_SIZE;
		// если индекс "убежал" как в большую сторону, так и в меньшую, то
		if (index > TAB_SIZE || index < 0) break;
		if (tab[index].is_empty)
			return index;
	}
	return -1;
}
// начальная инициализация полей структур hash_tab и patient
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
// поиск пациента по ключу
// хэшируем ключ и смотрим, что под полученным индексом хранится
bool find_htab(hash_tab *tab, string key, int &index)
{
	int result(0); // результат суммирования кодов символов ключа
	for(int i=0; i < SIZE_KEY - 1; i++)
		result += int(key[i])*int(pow(3.0, i));
	int h_1 = hash_one(result),
		h_2 = hash_two(result);
	// формируем начальный индекс на основе первого хэширования
	//index = h_1;
	int i(0); 
	bool str_compare(false);
	do
	{
		index = (h_1 + i*h_2) % TAB_SIZE;
		if (index > TAB_SIZE || index < 0) return false;
		// чуткая проверка: совпал ли ключ введеный с ключом в структуре patient
		// промахи бывали, поэтому проверка нужна
		if (!strcmp(tab[index].pat.key.c_str(), key.c_str())) 
		{ 
			str_compare = true; // если совпало, то мы нашли, что искали
			break;
		}
		i++;
	}
	while(!tab[index].is_empty);
	if (str_compare) return true;
	else return false;
}
// добавление записи в таблицу
bool add_to_tab(hash_tab *tab, patient pat)
{
	int index(0); // индекс ячейки таблицы
	index = h(tab, pat.key);  // получение индекса путем 2-го хэширования
	// если хэшировать удалось, то добаляем и сообщаем, что удачно добавили(return true)
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
	else return false; // иначе говорим, что запись не добавлена
}
// удаление записи из таблицы по ключу
bool del_htab(hash_tab *tab, string key)
{
	int index(0);//индекс ключа
	if (find_htab(tab, key, index)) //если ключ, который мы хотим удалить найден, то удаляем)
	{
		tab[index].pat.key = "0";
		tab[index].pat.name = "0";
		tab[index].pat.bday[0] = 0;
		tab[index].pat.bday[1] = 0;
		tab[index].pat.bday[2] = 0;
		tab[index].pat.adress = "0";
		tab[index].pat.workplace = "0";
		tab[index].is_empty = true;
		return true; // сообщаем об удачном удалении
	}
	else return false; // если запись не найдена, то сообщаем об не удачном удалении
}
// проверка таблицы на наличие хотя бы одной записи
bool have_pat(hash_tab *tab)
{
	for (int i = 0; i < TAB_SIZE; i++)
	{
		if (tab[i].is_empty) continue;
		else return true;
	}
	return false;
}
/* =================== Работа с файлом =================== */
// считываем данные из файла и заполняем ими хэш-таблицу
bool read_pat_base(hash_tab *tab)
{
	bool fail(false); // флаг для проверки на наличие ошибок в файле
	ifstream fin;
	fin.open(_PAT_FILENAME);
	// файлик заполнен нулями(пустые ячейки таблицы) и данными о пациентах
	// считывание идет полностью, таблица заполняется полностью с файла
	if (fin)
	{
		// смотрим на количество символов в файле
		fin.seekg(0, ios_base::end); // смещаемся в конец
		int i = int(fin.tellg()); // получаем количество символов, которые мы прошли при продвежению к концу файла
		fin.seekg(42, fin.beg); // смещаемся в начало файла пропуская шапку
		// 42 - размер шапки файла, если в файле меньше 42 символов, то
		if (i <= 42) fail = true;
		else // иначе считываем инфу с файла
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
				//if (flag != '#') fin.putback(flag); // считываем пока не встретим флаг
				// если мы считали нули(размер ключа не совпадает), то 
				if (tab[i].pat.key.length() < SIZE_KEY-2) tab[i].is_empty = true;
				// иначе говорим, что ячейка заполнена
				else { tab[i].is_empty = false; _pat_count++; }
				// если при чтении найдена ошибка, то говорим об этом
				/*if (fin.fail()) 
				{ 
					fail = true;
					break;
				}*/
			}
		}
	} 
	// если ошибка, то выводим сообщение
	else { cout << "\nВнимание: Не удалось открыть Базу Данных пациентов!\n\n"; system("pause"); fail = true; }
	// если ошибка, то совершаем начальную инициализацию полей структур
	if (fail) htab_init(tab);
	// чистим, синхронизируем поток от ошибок
	fin.sync();
	fin.clear();
	fin.close();
	// возвращаем флаг состояния ошибок
	return fail;
}
// вывод всех ячеек хэш-таблицы в файл
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
// вывод таблицы в файл
void fill_pat_base(hash_tab *tab)
{
	ofstream fout;
	fout.open(_PAT_FILENAME);
	if (fout)
	{
		// первым делом выводим в файл шапку
		fout << "=========База данных пациентов===========";
		// затем уже и всю хэш-таблицу
		pat_to_file(fout, tab);
	}
	else { cout << "\nВнимание: Не удалось открыть Базу Данных Пациентов!\n\n"; system("pause"); }
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
/* =========== Функции инициализации Пациента ============ */
// создание ключа на основе введенного "участка пациента" и порядка клиентуры
string key_creator(short num_place)
{
	// ключ формата ММ-ММММММ
	short null_count(0); // размер нулей в ключе после "-"
	char buff[20]; // буфер для перевода числа в строку
	char temp_num[3]; // временная строка с № участка пациента
	char temp_count[7]; // временная строка с № порядка пациента
	string key; // ключ
	// добавляем в temp_num № участка
	strcpy(temp_num, _itoa(num_place, buff, RADIX));
	// добавляем в temp_count № порядка пациента
	strcpy(temp_count, _itoa(_pat_count, buff, RADIX));
	// если № участка <= 9, то добавляем 0 в перед
	if (num_place <= 9) key += "0";
	key += temp_num;
	key += "-";
	// того мы имеем либо "0М-", либо "ММ-"
	// исходя из размера строки с № порядка пациента формируем кол-во нулей после "-"
	null_count = SIZE_KEY - 1 - strlen(temp_count);
	for(int i = 3; i < null_count; i++)
		key += "0";
	key += temp_count;
	// возвращаем ключ
	return key;
}
// функция заполнения данных о пациенте
patient pat_init()
{
	bool fail(false);
	short num_place(0);
	patient temp;
	char adress[SIZE_ADRESS];
	cout<<"Введите ФИО пациента: ";
	do
	{
		fail = false;
		input_control(temp.name);
		if (temp.name.length() <= 3)
		{
			cout << "Ошибка ввода данных! Повторите ввод: ";
			fail = true;
		}
	} while (fail);
	input_control(temp.bday);
	cin.get();
	cout<<"Введите место работы/учебы пациента: ";
	cin.getline(adress, SIZE_ADRESS, '\n');
	temp.workplace = adress;
	cout<<"Введите адресс пациента: ";
	cin.getline(adress, SIZE_ADRESS, '\n');
	temp.adress = adress;
	cout<<"Введите номер участка пациента: ";
	num_place = input_control(99, 1);
	temp.key = key_creator(num_place);
	_pat_count++;
	cout << "\n=== Пациент зарегистрирован! ===\n";
	return temp;
}
// функция вывода таблицы с информацией об пациентах
void pat_out(hash_tab *tab)
{
	// данные для формирования шапки
	const short COUNT_COL(5); // число столбцов
	string name_col[] = { "Рег.номер", "ФИО ", "Дата рождения", "Адрес", "Место работы" }; // имена столбцов
	short size_col[COUNT_COL] = { 11, 34, 14, 65, 30 }; // размеры столбцов
	
	short size_tab = print_head(name_col, size_col, COUNT_COL); // вывод шапки таблицы
	for(int i = 0; i < TAB_SIZE; i++) // вывод элементов таблицы
	{
		if (tab[i].is_empty) continue; // если пусто, то пропускаем
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
	print_end(size_tab); // вывод нижней границы таблицы
}