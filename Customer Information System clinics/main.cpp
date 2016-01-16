/* =================== Библиотеки =================== */
#include "hospital.h"
/* ================== Функции меню ================== */
// меню работы с пациентами
void pat_menu(hash_tab **pat, node *doc, refferal **ref_head, refferal **ref_tail)
{
	bool end(false); // флаг остановки работы(возврат в главное меню)
	svector_pat search; // вектор найденных данных врачей
	int choice; // выбор критерия
	string answer; // ответ на вопрос д\н

	while(!end) 
	{
		system("cls");
		cout<<"======= Работа с клиентурой =======\n"
			<<" 1. Регистрация нового больного\n"
			<<" 2. Удаление данных о больном\n"
			<<" 3. Список всех больных\n"
			<<" 4. Удаление данных о всех больных\n"
			<<" 5. Поиск\n"
			<<" 0. Назад в меню\n"
			<<"===================================\n"
			<<"\nВведите команду: ";
		choice = input_control(5, 0);
		if ((choice >= 2 && choice <= 5) && !(have_pat(*pat)))
		{
			cout << "\nБД пациентов пуста! Пункты #2 - #5 недоступны.\n";
			system("pause");
			choice = -1;
		}
		switch(choice)
		{
		case 1:
			cout<<" Регистрация больного:\n";
			add_to_tab(*pat, pat_init());
			system("pause");
			break;
		case 2:
			del_pat(*pat, ref_head, ref_tail);
			system("pause");
			break;
		case 3:
			pat_out(*pat);
			system("pause");
			break;
		case 4:
			cout << " Удалить данные о всех больных? \n(Также удалится вся информация о выданных направлениях на прием) д/н ";
			answer_control(answer);
			if (answer[0] == 'д')
			{
				htab_init(*pat);
				del_list(ref_head, ref_tail);
				cout << " Записи удалены.\n";
				system("pause");
			}
			else
			{
				cout << " Записи НЕ удалены.\n";
				system("pause");
			}
			break;
		case 5:
			pat_search_menu(search, *pat, doc, *ref_head);
			system("pause");
			break;
		case 0:
			end = true;
			break;
		default:
			break;
		}
		search.clear();
		search.shrink_to_fit();
	}
}
// меню работы с врачами
void doc_menu(node **doc, hash_tab *tab, refferal **ref_head, refferal **ref_tail)
{
	bool end(false); // флаг остановки работы(возврат в главное меню)
	svector_doc search; // вектор найденных данных врачей
	int choice; // выбор критерия
	string answer; // ответ на вопрос д\н

	while(!end) 
	{
		system("cls");
		cout<<"======= Работа с докторами =======\n"
			<<" 1. Добавить нового врача\n"
			<<" 2. Удаление сведений о враче\n"
			<<" 3. Список всех врачей\n"
			<<" 4. Удаление данных о всех врачах\n"
			<<" 5. Поиск\n"
			<<" 0. Назад в меню\n"
			<<"==================================\n"
			<<"\nВведите команду: ";
		choice = input_control(5, 0);
		if (!(*doc) && (choice >= 2 && choice <= 5))
		{
			cout << "\nБД докторов пуста! Пункты #2 - #4 недоступны.\n";
			system("pause");
			choice = -1;
		}
		switch(choice)
		{
		case 1:
			cout<<" Регистрация врача:\n";
			*doc = add_node(*doc, doc_init());
			cout << "\n=== Специалист добавлен ===\n";
			system("pause");
			break; 
		case 2:
			doc_del_menu(doc, search, ref_head, ref_tail);
			system("pause");
			break;
		case 3:
			//debug_out(*doc, 0);
			doc_out(*doc);
			system("pause");
			break;
		case 4:
			cout << " Удалить данные о всех врачах? \n(Также удалится вся информация о выданных направлениях на прием) д/н ";
			answer_control(answer);
			if (answer[0] == 'д')
			{
				remove_all_doc(doc);
				del_list(ref_head, ref_tail);
				cout << " Записи удалены.\n";
				system("pause");
			}
			else
			{
				cout << " Записи НЕ удалены.\n";
				system("pause");
			}
			break;
		case 5:
			doc_search_menu(*doc, tab, *ref_head, search);
			system("pause");
			break;
		case 0:
			end = true;
			break;
		default:
			break;
		}
		search.clear();
		search.shrink_to_fit();
	}
}
// меню работы с выписками
void ref_menu(refferal **ref_head, refferal **ref_tail, node *doc, hash_tab *pat)
{
	bool end(false); // флаг остановки работы(возврат в главное меню)
	svector_ref search; // вектор найденных записей направлений
	svector_doc search_doc; // вектор найденных данных врачей
	int choice; // выбор критерия

	while(!end) 
	{
		system("cls");
		cout<<"======== Работа с направлениями ========\n"
			<<" 1. Добавить новое направление\n"
			<<" 2. Возврат направления\n"
			<<" 3. Список всех направлений\n"
			<<" 4. Удаление данных всех направлений\n"
			<<" 0. Назад в меню\n"
			<<"========================================\n"
			<<"\nВведите команду: ";
		choice = input_control(4, 0);
		if (!(*ref_head) && (choice >= 2 && choice <= 4))
		{
			cout << "\nБД направлений пуста! Пункты #2 - #4 недоступны.\n";
			system("pause");
			choice = -1;
		}
		switch(choice)
		{
		case 1:
			cout<<" Регистрация направления:\n";
			add_to_list(ref_head, ref_tail, doc, pat, search_doc);
			cout << "\n=== Направление добавлено ===\n";
			system("pause");
			break;
		case 2:
			del_ref_menu(ref_head, ref_tail);
			system("pause");
			break;
		case 3:
			cout<<" Список всех направлений:\n";
			print_list(*ref_head, pat);
			system("pause");
			break;
		case 4:
			del_list(ref_head, ref_tail);
			cout<<" Записи удалены.\n";
			system("pause");
			break;
		case 0:
			end = true;
			break;
		default:
			break;
		}
		search.clear();
		search.shrink_to_fit();
		search_doc.clear();
		search_doc.shrink_to_fit();
	}
}
// главное меню 
void menu()
{
	bool end(false); // флаг остановки работы
	int choice; // критерий выбора
	/* =============== Обявления ============== */
	node *doc = NULL; // АВЛ-дерево данных врачей
	refferal *ref_head = NULL, *ref_tail = NULL; // циклический список направлений 
	hash_tab *pat = new hash_tab[TAB_SIZE]; // хэш-таблица данных пациентов
	/* ============ Чтение из файлов ========== */
	read_pat_base(pat);
	read_doc_base(&doc);
	read_ref_base(&ref_head, &ref_tail);

	while(!end) 
	{
		system("cls");
		cout<<"=========== Меню ============\n"
			<<" 1. Работа с клиентурой \n"
			<<" 2. Работа с направлениями\n"
			<<" 3. Работа с докторами\n"
			<<" 4. Сохранить изменения\n"
			<<" 0. Выход\n"
			<<"=============================\n"
			<<"\nВведите команду: ";
		choice = input_control(4, 0);
		if (choice == 2 && !(doc || have_pat(pat)))
		{
			if (have_pat(pat)) cout << "\nБД врачей пуста! Пункт #2 недоступен.\n";
			else cout << "\nБД пациентов пуста! Пункт #2 недоступен.\n";
			system("pause");
			choice = -1;
		}
		switch(choice)
		{
		case 1:
			pat_menu(&pat, doc, &ref_head, &ref_tail);
			break;
		case 2:
			ref_menu(&ref_head, &ref_tail, doc, pat);
			break;
		case 3:
			doc_menu(&doc, pat, &ref_head, &ref_tail);
			break;
		case 4:
			fill_pat_base(pat);
			fill_doc_base(doc);
			fill_ref_base(ref_head);
			break;
		case 0:
			end = true;
			break;
		default:
			break;
		}
	}
	cout<<"Завершаю работу...\n";
	/* ============ Запись в файлы ============ */
	fill_pat_base(pat);
	fill_doc_base(doc);
	fill_ref_base(ref_head);/**/
	/* ==== Удаление динамических структур ==== */
	del_list(&ref_head, &ref_tail);
	remove_all_doc(&doc);
	delete [] pat;
}
/* ================== Функция main ================== */
int main()
{
	set_console_settings(); // установка настроек консоли
	menu(); // запуск главного меню
	//test_memory_leaks(); // искусственная утечкa памяти
	is_memory_leaks(); // поверка на утечки памяти
	return 0;
}