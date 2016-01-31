// Memory leaks

#define _CRT_SECURE_NO_WARNINGS

#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW
#endif
#endif

#include <time.h>
#include <iostream>
#include "hash_tab.h"

using namespace std;


const char *TXT_FILENAME = "Result.txt";

void is_memory_leaks()
{
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
	_CrtDumpMemoryLeaks();

	system("pause");
}

void test()
{
	srand(time(NULL));

	hash_tab table;

	table.get_collisions_test(TXT_FILENAME);
}

int main()
{
	test();
	is_memory_leaks();
	return 0;
}