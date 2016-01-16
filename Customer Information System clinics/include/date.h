/* ============ ������ � ����� ============ */
#pragma once
#include "anytools.h"
// ��������� ������� ������ �����
struct date
{
	short work_day[WORKDAY_SIZE]; // ������ ������� ���� �����
	SYSTEMTIME time[2]; // ����� ������ �����
};
// ����� ��� ������
string get_day_of_week(SYSTEMTIME date)
{
	string day;
	switch (date.wDayOfWeek)
	{
	case 0:
		day = "�����������";
		break;
	case 1:
		day = "�����������";
		break;
	case 2:
		day = "�������";
		break;
	case 3:
		day = "�����";
		break;
	case 4:
		day = "�������";
		break;
	case 5:
		day = "�������";
		break;
	case 6:
		day = "�������";
		break;
	default:
		day = "";
		break;
	}
	return day;
}
// ����� ��� ������
string get_day_of_week(short date)
{
	string day;
	switch (date)
	{
	case 0:
		day = "�����������";
		break;
	case 1:
		day = "�����������";
		break;
	case 2:
		day = "�������";
		break;
	case 3:
		day = "�����";
		break;
	case 4:
		day = "�������";
		break;
	case 5:
		day = "�������";
		break;
	case 6:
		day = "�������";
		break;
	default:
		day = "";
		break;
	}
	return day;
}
// ����� ������������ ������
string get_month(SYSTEMTIME date)
{
	string mounth;
	switch (date.wMonth)
	{
	case 1:
		mounth = "������";
		break;
	case 2:
		mounth = "�������";
		break;
	case 3:
		mounth = "����";
		break;
	case 4:
		mounth = "������";
		break;
	case 5:
		mounth = "���";
		break;
	case 6:
		mounth = "����";
		break;
	case 7:
		mounth = "����";
		break;
	case 8:
		mounth = "������";
		break;
	case 9:
		mounth = "��������";
		break;
	case 10:
		mounth = "�������";
		break;
	case 11:
		mounth = "������";
		break;
	case 12:
		mounth = "�������";
		break;
	default:
		mounth = "";
		break;
	}
	return mounth;
}
// ���������� ������� ������� ���� ����� ���������� ���������
void sort_worday(short *mass)
{
	bool changed(true); // ���� ��������� ��������� �������
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
// �������� �� ������������� ��� ������
bool check_copyday(short *mass, short val)
{
	for (int i = 0; i < WORKDAY_SIZE; i++)
	{
		if (mass[i] == val)
			return true;
	}
	return false;
}
// ������� ������������ �� ���� ���� ������
int what_day(SYSTEMTIME date)
{
	/* ������� ������� (Zeller's congruence)*/
	int k = date.wYear % 100,
		j = int(date.wYear / 100),
		h = (date.wDay + int(13 * (date.wMonth + 1) / 5) + k + int(k / 4) + int(j / 4) - 2 * j) % 7,
		result = ((h + 5) % 7) + 1;
	return result == 7 ? result - 7: result;
}
// ���������� ������� �� ��� ����
void half_hour_increase(SYSTEMTIME &date)
{
	bool hour_inc(false); // ���� ���������� ���������� �� 1 ���
	int summ_minute = date.wMinute + 30; // ����� �����
	if (summ_minute >= 60) // ���� ����� >= 60, ��
	{
		hour_inc = true; // �� ���������, ��� ������� ����� ��������� �� ���
		summ_minute -= 60; // ��������� ������������ �������
	}
	date.wHour += (hour_inc ? 1 : 0);
	date.wMinute = summ_minute;
}
// ���������� ���� �� inc ����
void many_day_increase(SYSTEMTIME &date, int inc = 1)
{
	bool year_inc(false); // ���� ���������� ���������� �� 1 ���
	bool month_inc(false); // ���� ���������� ���������� �� 1 �����
	int summ_day = date.wDay + inc;
	if (summ_day > 31)
	{
		month_inc = true; // �� ���������, ��� ������� ����� ��������� �� 1 
		summ_day -= 31;
	}
	date.wDay = summ_day;
	int summ_month = date.wMonth + (month_inc ? 1 : 0);
	if (date.wMonth > 12)
	{
		year_inc = true; // �� ���������, ��� ������� ����� ��������� �� 1
		summ_month -= 12;
	}
	date.wMonth = summ_month;
	date.wYear += (year_inc ? 1 : 0);
}
// ������� ����������� �������� ������� (minuend - subtrahend = difference)
// HH:MM - HH:MM
SYSTEMTIME diff_time(SYSTEMTIME min, SYSTEMTIME sub, bool *fail = false)
{
	SYSTEMTIME result; // ��������� ��������
	GetSystemTime(&result);
	if (min.wHour - sub.wHour >= 0) // ���� ��� ������������� ������� ����������(�����������>�����������), ��
	{
		bool hour_dec(false); // ���� ������������ ���������� �� 1 ��� 
		int diff_minute = min.wMinute - sub.wMinute; // ������� �����
		if (diff_minute < 0) // ���� ��� ������������
		{
			hour_dec = true; // �� ���������, ��� ������� ����� ��������� �� ��� 
			diff_minute += 60; // ��������� ��������� �������
		}
		result.wHour = min.wHour - sub.wHour - (hour_dec ? 1 : 0);
		result.wMinute = diff_minute;
	}
	else *fail = true;
	return result;
}
// ������� ����������� �������� ��� (minuend - subtrahend = difference)
/*/ DD.MM - DD.MM
SYSTEMTIME diff_date(SYSTEMTIME min, SYSTEMTIME sub, bool *fail = false)
{
	SYSTEMTIME result; // ��������� ��������
	GetSystemTime(&result);
	if (min.wHour - sub.wHour >= 0) // ���� ��� ������������� ������� ����������(�����������>�����������), ��
	{
		bool hour_dec(false); // ���� ������������ ���������� �� 1 ��� 
		int diff_minute = min.wMinute - sub.wMinute; // ������� �����
		if (diff_minute < 0) // ���� ��� ������������
		{
			hour_dec = true; // �� ���������, ��� ������� ����� ��������� �� ��� 
			diff_minute += 60; // ��������� ��������� �������
		}
		result.wHour = min.wHour - sub.wHour - (hour_dec ? 1 : 0);
		result.wMinute = diff_minute;
	}
	else *fail = true;
	return result;
}.*/
// ������� ������������ ������� 
// HH:MM + HH:MM
SYSTEMTIME summ_time(SYSTEMTIME min, SYSTEMTIME sub)
{
	SYSTEMTIME result; // ��������� �����
	GetSystemTime(&result);
	bool hour_inc(false); // ���� ���������� ���������� �� 1 ���
	int summ_minute = min.wMinute + sub.wMinute; // ����� �����
	if (summ_minute >= 60) // ���� ����� >= 60, ��
	{
		hour_inc = true; // �� ���������, ��� ������� ����� ��������� �� ��� 
		summ_minute -= 60; // ��������� ������������ �������
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
// ����� ������� ������ �����
void show_reception(date d)
{
	//cout << "������ ������ �����:\n";
	for (int i = 0; i < WORKDAY_SIZE; i++)
	{
		if (d.work_day[i] != NON_WEEK_DAY) cout << get_day_of_week(d.work_day[i]) << ", ";
	}
	//cout << "\n����� ������: \n";
	for (int i = 0; i < 2; i++)
	{
		d.time[i].wHour < 10 ? cout << "0" << d.time[i].wHour : cout << d.time[i].wHour;
		cout << ":";
		d.time[i].wMinute < 10 ? cout << "0" << d.time[i].wMinute : cout << d.time[i].wMinute;
		i == 0 ? cout << " - " : cout;
	}
	cout << endl;
}
// �������� ������� ���� ������ �������� �� ��������� � ���������� ������� ������ �����
bool is_time_visit(SYSTEMTIME *time, SYSTEMTIME visit)
{
	bool fail(false);
	SYSTEMTIME a = diff_time(time[1], time[0], &fail), // ������� � ����������
		b = diff_time(visit, time[0], &fail); // ������� ����� �������� ������ � �������� ������ ������ �����
	if (fail) return false;

	int delta_1 = a.wHour * 60 + a.wMinute, // ������� � ������ ������� � ����������
		delta_2 = b.wHour * 60 + b.wMinute; // ������� � ������ 2-� �������
	// ���� delta_2 <= delta_1, �� ����� ������ ����������� ���������� ������� ������ �����
	if (delta_2 <= delta_1) return true;
	else return false; // ����� �� ����������� 
}
// �������� ���� ������ �������� �� ��������� � ��������� ������� ���� �����
bool is_date_visit(short *work_day, SYSTEMTIME visit)
{
	int week = what_day(visit);
	for (int i = 0; work_day[i] != NON_WEEK_DAY; i++)
	{
		if (week == work_day[i]) return true;
	}
	return false;
}
// ���������� ���� �� 1 ����
/*void �one_day_increase(SYSTEMTIME &date)
{
	bool year_inc(false); // ���� ���������� ���������� �� 1 ���
	bool month_inc(false); // ���� ���������� ���������� �� 1 �����
	int summ_day = date.wDay + 1; 
	if (summ_day > 31) 
	{
		month_inc = true; // �� ���������, ��� ������� ����� ��������� �� 1 
		summ_day -= 31; 
	}
	date.wDay = summ_day;
	int summ_month = date.wMonth + (month_inc ? 1 : 0);
	if (date.wMonth > 12)
	{
		year_inc = true; // �� ���������, ��� ������� ����� ��������� �� 1
		summ_month -= 12;
	}
	date.wMonth = summ_month;
	date.wYear += (year_inc ? 1 : 0);
}/**/
// �������� ���� �� ������������� ��� ������
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
// �������� ������� �� ������������� ����������
/*void minute_diff_compensate(SYSTEMTIME *time, SYSTEMTIME &date)
{
	while (!is_time_visit(time, date))
		many_day_increase(date);
}/**/
// ������� ��������� ���
// 1 - time_1 > time_2
// 2 - time_1 < time_2
// 0 - �����
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
// ������� ��������� ������
// 1 - time_1 > time_2
// 2 - time_1 < time_2
// 0 - �����
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
// ������� ����������� ����
SYSTEMTIME datecpy(SYSTEMTIME cur_time)
{
	SYSTEMTIME temp;
	GetSystemTime(&temp);

	temp.wDay = cur_time.wDay;
	temp.wMonth = cur_time.wMonth;
	temp.wYear = cur_time.wYear;
	return temp;
}
// ������� ����������� ����
void datecpy(SYSTEMTIME copy, SYSTEMTIME &time)
{
	time.wDay = copy.wDay;
	time.wMonth = copy.wMonth;
	time.wYear = copy.wYear;
}
// ������� ����������� �������
SYSTEMTIME timecpy(SYSTEMTIME cur_time)
{
	SYSTEMTIME temp;
	GetSystemTime(&temp);
	
	temp.wHour = cur_time.wHour;
	temp.wMinute = cur_time.wMinute;
	return temp;
}
// ���������� ����� ������ � ����
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