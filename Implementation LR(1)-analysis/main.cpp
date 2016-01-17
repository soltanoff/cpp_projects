#include <stdio.h>
#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

const short TBL_W = 12;  // количество строк 
const short TBL_H = 10;	 // количество столбцов 
const unsigned short MAX_STACK_SIZE = 100; // максимальный размер стека (надеюсь 100 элементов нам хватит) [А ЧТО? А ВДРУГ?!]

struct TABLE_REC // эта структура будет отвечать за массив-таблицу, каждая ячейка которого будет содержать по 2 параметра.
{
	char Type; // тип команды: [E,D,S,R] и ничего кроме них.
	unsigned short Index; // и индекс, который служит отсылкой либо к правилу свёртки либо является новым состоянием
};

struct RULE // пришлось использовать ещё одну структуру для правил.
{
	char *L_Rule; // Левая часть правила (используется при эмуляции)
	char *R_Rule; // Правая часть равила (используется при свёртке)
};

const char TOP_STR[TBL_W]={'S','A','L','c','a',',','e','(',')','$'};//шапка таблицы, список всех терминалов\не терминалов

const RULE RULES[5]={{"S","caA"},{"A","(L)"},{"A",""},{"L","e,L"},{"L","e"}}; // список правил: левая правая часть

const TABLE_REC TABLE[TBL_W][TBL_H]= 
	{//	----(S)------(A)------(L)------(c)------(a)------(,)------(e)------(()------())------($)----
/*(0)*/ {{'D', 0},{'E', 0},{'E', 0},{'S', 1},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0}},
/*(1)*/	{{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'S', 2},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0}},
/*(2)*/	{{'E', 0},{'S', 3},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'S', 4},{'E', 0},{'R', 2}},
/*(3)*/	{{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'R', 0}},
/*(4)*/	{{'E', 0},{'E', 0},{'S', 5},{'E', 0},{'E', 0},{'E', 0},{'S', 8},{'E', 0},{'E', 0},{'E', 0}},
/*(5)*/	{{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'S', 6},{'E', 0}},
/*(6)*/	{{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'R', 1}},
/*(7)*/	{{'E', 0},{'S', 8},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0}},
/*(8)*/	{{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'S', 9},{'E', 0},{'E', 0},{'R', 4},{'E', 0}},
/*(9)*/	{{'E', 0},{'E', 0},{'S',10},{'E', 0},{'E', 0},{'E', 0},{'S', 8},{'E', 0},{'E', 0},{'E', 0}},
/*(10)*/{{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'E', 0},{'R', 3},{'E', 0}},
	};

int main()
{
	string str; // объявляем нашу строку
	cout << "Grammar:\n"
		<< "<S>::= c a <A>\n"
		<< "<A>::= ( <L> )\n"
		<< "<A>::= lambda\n"
		<< "<L>::= e , <L>\n"
		<< "<L>::= e\n" << endl;
	cout << "Input string: "; 
	cin.clear();
	cin.sync(); 
	cin >> str;
	cout << endl;
	str.append("$"); // цепляем к нашей строке символ конца ...

	short sost_stack[MAX_STACK_SIZE] = {0}; // стек состояний (состояния в стеке тоже отсчитываются от 0)
	char char_stack[MAX_STACK_SIZE] = {}; // стек символов (пока пустой)

	short sost_count = 0; // счётчик позицый стека состояний
	short char_count = -1; // счётчик позицый стека символов
	short curr_char = 0; // текущий символ нашей строки
	short step = 1; // счётчик шагов (так интереснее)

	bool exit_fg=false; // флаг выхода из цыкла (традициии нарушать нельзя)) )
	bool simulation=false; // симулирующий флаг)) он же флаг симуляции

	while (!exit_fg)
	{		
		//				ВИЗУАЛИЗАТОР	(начало)						
		cout << "\nStep #" << step << ":" << endl; // вывод шага
		
		string char_marker; // строка, которая будет выделять рассматриваемый символ

		for (short n=0; n<str.length();n++) // тут мы эту строку создаём
			if (n==curr_char) char_marker.append("|");
			else char_marker.append("=");

		cout << char_marker << endl; // тут эту строку выводим
		cout << str << endl;	// выводим исходную строку
		cout << char_marker << endl; // тут сного выводим ЭТУ строку

		cout << "\n#<stack states>#\t\t#<stack symbols>#" << endl;
		for (short n=max(sost_count,char_count); n>=0; n--)		// тут мы выводим сразу 2 стека (использую max(sost_count,char_count), но думаю что если просто взят sost_count то тоже будет стабильно )))
			cout << "|\t" << sost_stack[n] << "\t|\t\t|\t" << char_stack[n] << "\t|" << endl;
		cout << "-----------------\t\t-----------------\n" << endl;
		//				ВИЗУАЛИЗАТОР	(конец)							

		short i,j=-1; // координаты правила в таблице

		i=sost_stack[sost_count]; 
		
		for (short n=0; n<=TBL_W; n++)// ищим столбец
			if (TOP_STR[n]==str[curr_char]) // когда символ в "шапке" таблицы совпадает с текущим символом исходной строки
			{
				j=n; // получаем индекс столбца
				break; // и заканчиваем это безобразие
			}

		if (j==-1) // проверка на косякоустойчивость
		{
			cout << "ERROR! Symbol ["<< str [curr_char] <<"] is not a terminal / non-terminal for the machine!" << endl;
			break; // прерываем выполнение главного цыкла
		}

		//cout << "TABLE [" << i << "," << j << "] = " << TABLE[i][j].Type << TABLE[i][j].Index << "\n" << endl;

		switch (TABLE[i][j].Type)	// смотрим получившуюся команду в таблице
		{
		case 'E': // приехали, строка не есть тру
			cout << "String is incorrect!" << endl;
			exit_fg=true; // разрешаем завершить главный цыкл
			break; //...
		case 'D': // ДОПУСК!
			cout << "String is correct!" << endl;
			exit_fg=true; // с чистой совестью завершаем цыкл
			break; //...
		case 'S': // SHIFT
			//cout << "<Вносим [" << TABLE[i][j].Index << "] в стек состояний и [" << str[curr_char] << "] в стек символов.>" << endl;
			sost_count++; // следующая ПУСТАЯ ячечка стека состояний
			sost_stack[sost_count]=TABLE[i][j].Index; // закидываем в стек номер правила S (индекс)
			char_count++; // следующая ПУСТАЯ ячечка стека символов
			char_stack[char_count]=str[curr_char]; // закидываем в стек текущий символ исходной строки

			if (simulation) // если была симуляция
			{
				//cout << "За одно выкинем из исходной строки сэмулированое [" << str[curr_char] << "]" << endl;
				str.erase(curr_char,1); // удаляем симулированный символ
				simulation=false; // отключаем режим симуляции
			} else curr_char++; // если симуляции не было то переключаемся на следующий символ в исходной строке

			break; //...
		case 'R': // Свёртка
			//cout << "<Сворачиваем правило: " << RULES[TABLE[i][j].Index].L_Rule << "::=" << RULES[TABLE[i][j].Index].R_Rule << ">" << endl;
			short ch=strlen(RULES[TABLE[i][j].Index].R_Rule); // количество символов которые надо выкинуть из стека (длинна от правой части правила)
			while (ch!=0) // пока не занулиться
			{
				char_stack[char_count]=NULL; // чистим
				char_count--; // уменьшаем индекс

				sost_stack[sost_count]=NULL; // чистим
				sost_count--; // уменьшаем индекс

				ch--; // уменьшаем счётчик
			}

			//cout << "В исходной строке эмулируем чтение [" << RULES[TABLE[i][j].Index].L_Rule << "]" << endl;

			simulation=true; // включаем режим симуляции
			str.insert(curr_char,RULES[TABLE[i][j].Index].L_Rule,0,1); // добавляем в строку  левую часть правила (ДА, РАБОТАЕТ ТОЛЬКО ЕСЛИ ПРАВИЛО ЗАДАНО 1 ЛИТЕРОЙ)
			
			break; //...		
		}
		step++; // СЛЕДУЮЩИЙ ШАГ
	}

	system("pause"); 
	return 0;
}