/* =================== ���������� =================== */
#include "hospital.h"
/* ================== ������� ���� ================== */
// ���� ������ � ����������
void pat_menu(hash_tab **pat, node *doc, refferal **ref_head, refferal **ref_tail)
{
	bool end(false); // ���� ��������� ������(������� � ������� ����)
	svector_pat search; // ������ ��������� ������ ������
	int choice; // ����� ��������
	string answer; // ����� �� ������ �\�

	while(!end) 
	{
		system("cls");
		cout<<"======= ������ � ���������� =======\n"
			<<" 1. ����������� ������ ��������\n"
			<<" 2. �������� ������ � �������\n"
			<<" 3. ������ ���� �������\n"
			<<" 4. �������� ������ � ���� �������\n"
			<<" 5. �����\n"
			<<" 0. ����� � ����\n"
			<<"===================================\n"
			<<"\n������� �������: ";
		choice = input_control(5, 0);
		if ((choice >= 2 && choice <= 5) && !(have_pat(*pat)))
		{
			cout << "\n�� ��������� �����! ������ #2 - #5 ����������.\n";
			system("pause");
			choice = -1;
		}
		switch(choice)
		{
		case 1:
			cout<<" ����������� ��������:\n";
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
			cout << " ������� ������ � ���� �������? \n(����� �������� ��� ���������� � �������� ������������ �� �����) �/� ";
			answer_control(answer);
			if (answer[0] == '�')
			{
				htab_init(*pat);
				del_list(ref_head, ref_tail);
				cout << " ������ �������.\n";
				system("pause");
			}
			else
			{
				cout << " ������ �� �������.\n";
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
// ���� ������ � �������
void doc_menu(node **doc, hash_tab *tab, refferal **ref_head, refferal **ref_tail)
{
	bool end(false); // ���� ��������� ������(������� � ������� ����)
	svector_doc search; // ������ ��������� ������ ������
	int choice; // ����� ��������
	string answer; // ����� �� ������ �\�

	while(!end) 
	{
		system("cls");
		cout<<"======= ������ � ��������� =======\n"
			<<" 1. �������� ������ �����\n"
			<<" 2. �������� �������� � �����\n"
			<<" 3. ������ ���� ������\n"
			<<" 4. �������� ������ � ���� ������\n"
			<<" 5. �����\n"
			<<" 0. ����� � ����\n"
			<<"==================================\n"
			<<"\n������� �������: ";
		choice = input_control(5, 0);
		if (!(*doc) && (choice >= 2 && choice <= 5))
		{
			cout << "\n�� �������� �����! ������ #2 - #4 ����������.\n";
			system("pause");
			choice = -1;
		}
		switch(choice)
		{
		case 1:
			cout<<" ����������� �����:\n";
			*doc = add_node(*doc, doc_init());
			cout << "\n=== ���������� �������� ===\n";
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
			cout << " ������� ������ � ���� ������? \n(����� �������� ��� ���������� � �������� ������������ �� �����) �/� ";
			answer_control(answer);
			if (answer[0] == '�')
			{
				remove_all_doc(doc);
				del_list(ref_head, ref_tail);
				cout << " ������ �������.\n";
				system("pause");
			}
			else
			{
				cout << " ������ �� �������.\n";
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
// ���� ������ � ���������
void ref_menu(refferal **ref_head, refferal **ref_tail, node *doc, hash_tab *pat)
{
	bool end(false); // ���� ��������� ������(������� � ������� ����)
	svector_ref search; // ������ ��������� ������� �����������
	svector_doc search_doc; // ������ ��������� ������ ������
	int choice; // ����� ��������

	while(!end) 
	{
		system("cls");
		cout<<"======== ������ � ������������� ========\n"
			<<" 1. �������� ����� �����������\n"
			<<" 2. ������� �����������\n"
			<<" 3. ������ ���� �����������\n"
			<<" 4. �������� ������ ���� �����������\n"
			<<" 0. ����� � ����\n"
			<<"========================================\n"
			<<"\n������� �������: ";
		choice = input_control(4, 0);
		if (!(*ref_head) && (choice >= 2 && choice <= 4))
		{
			cout << "\n�� ����������� �����! ������ #2 - #4 ����������.\n";
			system("pause");
			choice = -1;
		}
		switch(choice)
		{
		case 1:
			cout<<" ����������� �����������:\n";
			add_to_list(ref_head, ref_tail, doc, pat, search_doc);
			cout << "\n=== ����������� ��������� ===\n";
			system("pause");
			break;
		case 2:
			del_ref_menu(ref_head, ref_tail);
			system("pause");
			break;
		case 3:
			cout<<" ������ ���� �����������:\n";
			print_list(*ref_head, pat);
			system("pause");
			break;
		case 4:
			del_list(ref_head, ref_tail);
			cout<<" ������ �������.\n";
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
// ������� ���� 
void menu()
{
	bool end(false); // ���� ��������� ������
	int choice; // �������� ������
	/* =============== ��������� ============== */
	node *doc = NULL; // ���-������ ������ ������
	refferal *ref_head = NULL, *ref_tail = NULL; // ����������� ������ ����������� 
	hash_tab *pat = new hash_tab[TAB_SIZE]; // ���-������� ������ ���������
	/* ============ ������ �� ������ ========== */
	read_pat_base(pat);
	read_doc_base(&doc);
	read_ref_base(&ref_head, &ref_tail);

	while(!end) 
	{
		system("cls");
		cout<<"=========== ���� ============\n"
			<<" 1. ������ � ���������� \n"
			<<" 2. ������ � �������������\n"
			<<" 3. ������ � ���������\n"
			<<" 4. ��������� ���������\n"
			<<" 0. �����\n"
			<<"=============================\n"
			<<"\n������� �������: ";
		choice = input_control(4, 0);
		if (choice == 2 && !(doc || have_pat(pat)))
		{
			if (have_pat(pat)) cout << "\n�� ������ �����! ����� #2 ����������.\n";
			else cout << "\n�� ��������� �����! ����� #2 ����������.\n";
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
	cout<<"�������� ������...\n";
	/* ============ ������ � ����� ============ */
	fill_pat_base(pat);
	fill_doc_base(doc);
	fill_ref_base(ref_head);/**/
	/* ==== �������� ������������ �������� ==== */
	del_list(&ref_head, &ref_tail);
	remove_all_doc(&doc);
	delete [] pat;
}
/* ================== ������� main ================== */
int main()
{
	set_console_settings(); // ��������� �������� �������
	menu(); // ������ �������� ����
	//test_memory_leaks(); // ������������� �����a ������
	is_memory_leaks(); // ������� �� ������ ������
	return 0;
}