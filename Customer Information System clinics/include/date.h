/* ============ Работа с датой ============ */
#pragma once
#include "anytools.h"
// структура графика работы врача
struct date
{
	short work_day[WORKDAY_SIZE]; // массив рабочих дней врача
	SYSTEMTIME time[2]; // время работы врача
};
// вывод дня недели
string get_day_of_week(SYSTEMTIME date)
{
	string day;
	switch (date.wDayOfWeek)
	{
	case 0:
		day = "Воскресенье";
		break;
	case 1:
		day = "Понедельник";
		break;
	case 2:
		day = "Вторник";
		break;
	case 3:
		day = "Среда";
		break;
	case 4:
		day = "Четверг";
		break;
	case 5:
		day = "Пятница";
		break;
	case 6:
		day = "Суббота";
		break;
	default:
		day = "";
		break;
	}
	return day;
}
// вывод дня недели
string get_day_of_week(short date)
{
	string day;
	switch (date)
	{
	case 0:
		day = "Воскресенье";
		break;
	case 1:
		day = "Понедельник";
		break;
	case 2:
		day = "Вторник";
		break;
	case 3:
		day = "Среда";
		break;
	case 4:
		day = "Четверг";
		break;
	case 5:
		day = "Пятница";
		break;
	case 6:
		day = "Суббота";
		break;
	default:
		day = "";
		break;
	}
	return day;
}
// вывод наименования месяца
string get_month(SYSTEMTIME date)
{
	string mounth;
	switch (date.wMonth)
	{
	case 1:
		mounth = "Январь";
		break;
	case 2:
		mounth = "Февраль";
		break;
	case 3:
		mounth = "Март";
		break;
	case 4:
		mounth = "Апрель";
		break;
	case 5:
		mounth = "Май";
		break;
	case 6:
		mounth = "Июнь";
		break;
	case 7:
		mounth = "Июль";
		break;
	case 8:
		mounth = "Август";
		break;
	case 9:
		mounth = "Сентябрь";
		break;
	case 10:
		mounth = "Октябрь";
		break;
	case 11:
		mounth = "Ноябрь";
		break;
	case 12:
		mounth = "Декабрь";
		break;
	default:
		mounth = "";
		break;
	}
	return mounth;
}
// сортировка массива рабочих дней врача улучшенным пузырьком
void sort_worday(short *mass)
{
	bool changed(true); // флаг состояния изменения массива
	short temp;

	while (changed)
	{
		changed = false;
		for (int i = 1; i < WORKDAY_SIZE - 1; i++)
		{
			if (mass[i] > mass[i + 1])
			{
				temp = mass[i];
				mass[i] = mass[i + 1];
				mass[i + 1] = temp;
				changed = true;
			}
			if (mass[WORKDAY_SIZE - 1 - i] < mass[WORKDAY_SIZE - 2 - i])
			{
				temp = mass[WORKDAY_SIZE - 1 - i];
				mass[WORKDAY_SIZE - 1 - i] = mass[WORKDAY_SIZE - 2 - i];
				mass[WORKDAY_SIZE - 2 - i] = temp;
				changed = true;
			}
		}
	}
}
// проверка на повторяющиеся дни недели
bool check_copyday(short *mass, short val)
{
	for (int i = 0; i < WORKDAY_SIZE; i++)
	{
		if (mass[i] == val)
			return true;
	}
	return false;
}
// функция определяющая по дате день недели
int what_day(SYSTEMTIME date)
{
	/* Формула Зеллера (Zeller's congruence)*/
	int k = date.wYear % 100,
		j = int(date.wYear / 100),
		h = (date.wDay + int(13 * (date.wMonth + 1) / 5) + k + int(k / 4) + int(j / 4) - 2 * j) % 7,
		result = ((h + 5) % 7) + 1;
	return result == 7 ? result - 7: result;
}
// увеличение времени на пол часа
void half_hour_increase(SYSTEMTIME &date)
{
	bool hour_inc(false); // флаг увеличения результата на 1 час
	int summ_minute = date.wMinute + 30; // сумма минут
	if (summ_minute >= 60) // если сумма >= 60, то
	{
		hour_inc = true; // то указываем, что результ нужно увеличить на час
		summ_minute -= 60; // реализуем суммирование времени
	}
	date.wHour += (hour_inc ? 1 : 0);
	date.wMinute = summ_minute;
}
// увеличение даты на inc дней
void many_day_increase(SYSTEMTIME &date, int inc = 1)
{
	bool year_inc(false); // флаг увеличения результата на 1 год
	bool month_inc(false); // флаг увеличения результата на 1 месяц
	int summ_day = date.wDay + inc;
	if (summ_day > 31)
	{
		month_inc = true; // то указываем, что результ нужно увеличить на 1 
		summ_day -= 31;
	}
	date.wDay = summ_day;
	int summ_month = date.wMonth + (month_inc ? 1 : 0);
	if (date.wMonth > 12)
	{
		year_inc = true; // то указываем, что результ нужно увеличить на 1
		summ_month -= 12;
	}
	date.wMonth = summ_month;
	date.wYear += (year_inc ? 1 : 0);
}
// функция реализующая разность времени (minuend - subtrahend = difference)
// HH:MM - HH:MM
SYSTEMTIME diff_time(SYSTEMTIME min, SYSTEMTIME sub, bool *fail = false)
{
	SYSTEMTIME result; // результат разности
	GetSystemTime(&result);
	if (min.wHour - sub.wHour >= 0) // если все удовлетворяет правилу математики(Уменьшаемое>Вычитаемого), то
	{
		bool hour_dec(false); // флаг определяющий уменьшение на 1 час 
		int diff_minute = min.wMinute - sub.wMinute; // разница минут
		if (diff_minute < 0) // если она отрицательно
		{
			hour_dec = true; // то указываем, что результ нужно уменьшить на час 
			diff_minute += 60; // реализуем вычитание времени
		}
		result.wHour = min.wHour - sub.wHour - (hour_dec ? 1 : 0);
		result.wMinute = diff_minute;
	}
	else *fail = true;
	return result;
}
// функция реализующая разность дат (minuend - subtrahend = difference)
/*/ DD.MM - DD.MM
SYSTEMTIME diff_date(SYSTEMTIME min, SYSTEMTIME sub, bool *fail = false)
{
	SYSTEMTIME result; // результат разности
	GetSystemTime(&result);
	if (min.wHour - sub.wHour >= 0) // если все удовлетворяет правилу математики(Уменьшаемое>Вычитаемого), то
	{
		bool hour_dec(false); // флаг определяющий уменьшение на 1 час 
		int diff_minute = min.wMinute - sub.wMinute; // разница минут
		if (diff_minute < 0) // если она отрицательно
		{
			hour_dec = true; // то указываем, что результ нужно уменьшить на час 
			diff_minute += 60; // реализуем вычитание времени
		}
		result.wHour = min.wHour - sub.wHour - (hour_dec ? 1 : 0);
		result.wMinute = diff_minute;
	}
	else *fail = true;
	return result;
}.*/
// функция суммирования времени 
// HH:MM + HH:MM
SYSTEMTIME summ_time(SYSTEMTIME min, SYSTEMTIME sub)
{
	SYSTEMTIME result; // результат суммы
	GetSystemTime(&result);
	bool hour_inc(false); // флаг увеличения результата на 1 час
	int summ_minute = min.wMinute + sub.wMinute; // сумма минут
	if (summ_minute >= 60) // если сумма >= 60, то
	{
		hour_inc = true; // то указываем, что результ нужно увеличить на час 
		summ_minute -= 60; // реализуем суммирование времени
	}
	int summ_hour = min.wHour + sub.wHour + (hour_inc ? 1 : 0);
	if (summ_hour >= 24)
	{
		summ_hour -= 24;
		many_day_increase(result);
	}
	result.wHour = summ_hour;
	result.wMinute = summ_minute;
	return result;
}
// вывод графика работы врача
void show_reception(date d)
{
	//cout << "График работы врача:\n";
	for (int i = 0; i < WORKDAY_SIZE; i++)
	{
		if (d.work_day[i] != NON_WEEK_DAY) cout << get_day_of_week(d.work_day[i]) << ", ";
	}
	//cout << "\nВремя работы: \n";
	for (int i = 0; i < 2; i++)
	{
		d.time[i].wHour < 10 ? cout << "0" << d.time[i].wHour : cout << d.time[i].wHour;
		cout << ":";
		d.time[i].wMinute < 10 ? cout << "0" << d.time[i].wMinute : cout << d.time[i].wMinute;
		i == 0 ? cout << " - " : cout;
	}
	cout << endl;
}
// проверка времени даты приема пациента на попадание в промежуток времени приема врача
bool is_time_visit(SYSTEMTIME *time, SYSTEMTIME visit)
{
	bool fail(false);
	SYSTEMTIME a = diff_time(time[1], time[0], &fail), // разница в промежутка
		b = diff_time(visit, time[0], &fail); // разница мужду временем приема и временем начала работы вреча
	if (fail) return false;

	int delta_1 = a.wHour * 60 + a.wMinute, // перевод в минуты разницы в промежутке
		delta_2 = b.wHour * 60 + b.wMinute; // перевод в минуты 2-й разницы
	// если delta_2 <= delta_1, то время приема принадлежит промежутку времени работы врача
	if (delta_2 <= delta_1) return true;
	else return false; // иначе не принадлежит 
}
// проверка даты приема пациента на попадание в множество рабочих дней врача
bool is_date_visit(short *work_day, SYSTEMTIME visit)
{
	int week = what_day(visit);
	for (int i = 0; work_day[i] != NON_WEEK_DAY; i++)
	{
		if (week == work_day[i]) return true;
	}
	return false;
}
// увеличение даты на 1 день
/*void Йone_day_increase(SYSTEMTIME &date)
{
	bool year_inc(false); // флаг увеличения результата на 1 год
	bool month_inc(false); // флаг увеличения результата на 1 месяц
	int summ_day = date.wDay + 1; 
	if (summ_day > 31) 
	{
		month_inc = true; // то указываем, что результ нужно увеличить на 1 
		summ_day -= 31; 
	}
	date.wDay = summ_day;
	int summ_month = date.wMonth + (month_inc ? 1 : 0);
	if (date.wMonth > 12)
	{
		year_inc = true; // то указываем, что результ нужно увеличить на 1
		summ_month -= 12;
	}
	date.wMonth = summ_month;
	date.wYear += (year_inc ? 1 : 0);
}/**/
// смещение даты по определенному дню недели
void diff_compensate(short *work_day, SYSTEMTIME &date)
{
	while (!is_date_visit(work_day, date))
		many_day_increase(date);
	/*int cur_weekday = what_day(date);
	int earl_weekday = cur_weekday;
	int min_diff = 7;
	int temp(0);

	for (short i = 0; work_day[i] != NON_WEEK_DAY; i++)
	{
		temp = abs(earl_weekday - work_day[i]);
		if (min_diff > temp)//(earl_weekday > work_day[i])
		{
			min_diff = temp;
			//earl_weekday = work_day[i];
		}
	}
	many_day_increase(date, min_diff);
	/*if (earl_weekday == cur_weekday)
	{
		many_day_increase(date, 7);
	}
	else
	{
		many_day_increase(date, (abs(cur_weekday - earl_weekday) - 1));
	}/**/
}/**/
// смещение времени по определенному промежутку
/*void minute_diff_compensate(SYSTEMTIME *time, SYSTEMTIME &date)
{
	while (!is_time_visit(time, date))
		many_day_increase(date);
}/**/
// функция сравнения дат
// 1 - time_1 > time_2
// 2 - time_1 < time_2
// 0 - равны
short date_cmp(SYSTEMTIME time_1, SYSTEMTIME time_2)
{
	if (time_1.wDay == time_2.wDay &&
		time_1.wMonth == time_2.wMonth &&
		time_1.wYear == time_2.wYear)
		return 0;
	else
	{
		if (time_1.wYear == time_2.wYear)
		{
			if (time_1.wMonth == time_2.wMonth)
			{
				if (time_1.wDay > time_2.wDay)
					return 1;
				else
					return 2;
			}
			else
			{
				if (time_1.wMonth > time_2.wMonth)
					return 1;
				else
					return 2;
			}
		}
		else
		{
			if (time_1.wYear > time_2.wYear)
				return 1;
			else
				return 2;
		}
	}
}
// функция сравнение времен
// 1 - time_1 > time_2
// 2 - time_1 < time_2
// 0 - равны
short time_cmp(SYSTEMTIME time_1, SYSTEMTIME time_2)
{
	if (time_1.wHour == time_2.wHour &&
		time_1.wMinute == time_2.wMinute)
		return 0;
	if (time_1.wHour == time_2.wHour)
	{
		if (time_1.wMinute > time_2.wMinute)
			return 1;
		else
			return 2;
	}
	else
	{
		if (time_1.wHour > time_2.wHour)
			return 1;
		else
			return 2;
	}
}
// функция копирования даты
SYSTEMTIME datecpy(SYSTEMTIME cur_time)
{
	SYSTEMTIME temp;
	GetSystemTime(&temp);

	temp.wDay = cur_time.wDay;
	temp.wMonth = cur_time.wMonth;
	temp.wYear = cur_time.wYear;
	return temp;
}
// функция копирования даты
void datecpy(SYSTEMTIME copy, SYSTEMTIME &time)
{
	time.wDay = copy.wDay;
	time.wMonth = copy.wMonth;
	time.wYear = copy.wYear;
}
// функция копирования времени
SYSTEMTIME timecpy(SYSTEMTIME cur_time)
{
	SYSTEMTIME temp;
	GetSystemTime(&temp);
	
	temp.wHour = cur_time.wHour;
	temp.wMinute = cur_time.wMinute;
	return temp;
}
// порядковый номер недели в году
int num_of_week_in_year(SYSTEMTIME time)
{
	int value(0);
	if ((time.wYear % 4) == 0)
	{
		value = int((31 * (time.wMonth - 1) - time.wMonth / 2 + time.wDay) / 7);
	}
	else
	{
		if (time.wMonth>2) time.wDay -= 1;
		value = int((31 * (time.wMonth - 1) - time.wMonth / 2 + time.wDay) / 7);
	}
	return value + 1;
}