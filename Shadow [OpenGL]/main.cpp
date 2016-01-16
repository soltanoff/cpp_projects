#include <windows.h>								
#include <tchar.h>
#include <direct.h>
#include <gl\gl.h>		 								
#include <gl\glu.h>										
#include <GL\glut.h>
#include <gl\GLAux.h>
#include "3dobjects.h"								

char* Object_Path;										// Путь к объекту

typedef float GLvector4f[4];							// Тип для процедуры перемножения матриц
typedef float GLmatrix16f[16];							// Тип для процедуры перемножения матриц

HDC			hDC=NULL;									// GDI Контекст устройства
HGLRC		hRC=NULL;									// Постоянный контекст рендеренга
HWND		hWnd=NULL;									// Хранит Хендел окна
HINSTANCE	hInstance=NULL;								// Хранит экземпляр приложения
bool keys[256], keypressed[256];						// Массив бработки нажатых клавишь
bool active=TRUE;										// Окно активно по умолчанию
bool fullscreen=FALSE;									// Полноэкранный режим

extern const int TextureCount;

TCHAR* TextureName[TextureCount] =
{
	"cubeOne.bmp",
	"cubeTwo.bmp",
	"cubeThree.bmp",
	"cubeFour.bmp",
	"cubeFive.bmp",
	"cubeSix.bmp",
	"cubeSeven.bmp",
	"cubeEight.bmp",
	"cubeNine.bmp",
	"cubeTen.bmp"
};

glObject obj;	// Объект

extern GLuint  texture[TextureCount];								// Память для текстуры

float LightPos[] = { 0.0f, 5.0f, 2.0f, 1.0f};			
float LightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f};			
float LightDif[] = { 0.6f, 0.6f, 0.6f, 1.0f};			
float LightSpc[] = {-0.2f, -0.2f, -0.2f, 1.0f};			

float ObjPos[] = {-2.0f,-2.0f,-5.0f};					// Позиция объекта

GLUquadricObj	*q;										// Указатель для пустой структуры (для источника света)

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	

void VMatMult(GLmatrix16f M, GLvector4f v)					// перемножение матриц
{
	GLfloat res[4];										
	res[0]=M[ 0]*v[0]+M[ 4]*v[1]+M[ 8]*v[2]+M[12]*v[3];
	res[1]=M[ 1]*v[0]+M[ 5]*v[1]+M[ 9]*v[2]+M[13]*v[3];
	res[2]=M[ 2]*v[0]+M[ 6]*v[1]+M[10]*v[2]+M[14]*v[3];
	res[3]=M[ 3]*v[0]+M[ 7]*v[1]+M[11]*v[2]+M[15]*v[3];
	v[0]=res[0];										
	v[1]=res[1];
	v[2]=res[2];
	v[3]=res[3];										
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();									


	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.001f,100.0f);

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();							
}

int InitGLObjects(char* path, glObject *obj)			// Инициализация объекта
{
	if (!ReadObject(path, obj))							// Считывание объекта
	{
		return FALSE;									// если чтение сорвалось
	}

	SetConnectivity(obj);								// Установить связи "лицом к лицу"

	for (unsigned int i=0;i<(*obj).nPlanes;i++)			// Идём по всем "точкам поверхности"
		CalcPlane(*obj, &((*obj).planes[i]));				// Вычисляем точки для всех граней

	return TRUE;									
}

GLvoid LoadGLTextures(TCHAR* Path, unsigned int Index)
{
	// Загрузка картинки
	AUX_RGBImageRec *texture1;

	texture1 = auxDIBImageLoad(Path);
		// Создание текстуры
	glGenTextures(1, &texture[Index]);
	glBindTexture(GL_TEXTURE_2D, texture[Index]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Включаем плавную тень
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Цвет закраски
	glClearDepth(1.0f);									// Настройка буфера глубины
	glClearStencil(0);									// Настройка буфера трафарета
	glEnable(GL_DEPTH_TEST);							// Включаем тест глубины
	glDepthFunc(GL_LEQUAL);								// Тип тестирования глубины
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	// настройки света
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);
	glEnable(GL_LIGHT1);								// Включаем источни света
	glEnable(GL_LIGHTING);								// Разрешаем освещение
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	
	q = gluNewQuadric();								// Инициализируем пустую структуру для объекта (сфера [свет])
	gluQuadricNormals(q, GL_SMOOTH);					// Настраиваем нормали
	gluQuadricTexture(q, GL_FALSE);						// Отключаем автоматическое текстурирование
	
	Object_Path = new char[strlen("Data/Object.txt")+1];
	strcpy(Object_Path,"Data/Object.txt");
	if (!InitGLObjects(Object_Path, &obj)) return FALSE;					// Инициализация тенеотбрасывающего объекта
	delete [] Object_Path;

	TCHAR current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir));

	/*TCHAR* Path = new TCHAR[strlen(current_work_dir) + strlen("\\Textures\\")+ strlen("Room.bmp") + 1];
	_tcscpy(Path, current_work_dir);
	_tcscat(Path, "\\Textures\\");
	_tcscat(Path, "Room.bmp");
	
	LoadGLTextures(Path);			// Загрузка текстур
	delete [] Path;*/
	for (int i=0; i<TextureCount; i++)
	{
		TCHAR* Path = new TCHAR[strlen(current_work_dir) + strlen("\\Textures\\")+ strlen(TextureName[i]) + 1];
		_tcscpy(Path, current_work_dir);
		_tcscat(Path, "\\Textures\\");
		_tcscat(Path, TextureName[i]);
	
		LoadGLTextures(Path,i);			// Загрузка текстур
		delete [] Path;
	}

	return TRUE;
}

void DrawGLRoom()										// Отрисовка комнаты
{
	glEnable(GL_TEXTURE_2D);							// Разрешение наложение текстуры
	glBindTexture(GL_TEXTURE_2D, texture[8]);		// Накладываемая текстура
	glBegin(GL_QUADS);									// Begin Drawing Quads
		// Нижняя грань
		glNormal3f(0.0f, 1.0f, 0.0f);											// Нормаль направлена вверх
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f,-10.0f,-20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f,-10.0f, 20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f,-10.0f, 20.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f,-10.0f,-20.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	/*glBegin (GL_QUADS);	
		// Верхняя грань
		glTexCoord2f(0.0f, 1.0f); glNormal3f(0.0f,-1.0f, 0.0f);					// Нормаль направлена вниз
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 10.0f, 20.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 10.0f,-20.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 10.0f,-20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 10.0f, 20.0f);
	glEnd();/**/

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin (GL_QUADS);	
		// фронтальная грань
		glNormal3f(0.0f, 0.0f, 1.0f);											// Нормаль направлена от смотрящего
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 10.0f,-20.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f,-10.0f,-20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f,-10.0f,-20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 10.0f,-20.0f);
	glEnd();/**/

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	/*glBegin (GL_QUADS);
		// Левая грань
		glNormal3f(1.0f, 0.0f, 0.0f);											// Нормаль направлена вправо
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 10.0f, 20.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f,-10.0f, 20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f,-10.0f,-20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 10.0f,-20.0f);
	glEnd();/**/

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	/*glBegin (GL_QUADS);	
		// Правая грань
		glNormal3f(-1.0f, 0.0f, 0.0f);											// Нормаль направлена налево
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 10.0f,-20.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f,-10.0f,-20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f,-10.0f, 20.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 10.0f, 20.0f);
		
	glEnd();/**/
	glDisable(GL_TEXTURE_2D);							// запрещаем наложение текстуры
}

float XPos, YPos, ZPos;
float xrot, yrot;
float SpherePos[] = {0.0f,-4.0f,-6.0f};

int DrawGLScene(GLvoid)
{
	GLmatrix16f Minv;
	GLvector4f wlp, lp;
	
	// Clear Color Buffer, Depth Buffer, Stencil Buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glLoadIdentity();									// сброс матрицы
	glTranslatef(0.0f, 0.0f, -20.0f);					
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// позиция Light1						

	glLoadIdentity();									
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);					
	glRotatef(-xrot, 1.0f, 0.0f, 0.0f);					
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Извлекаем видовую матрицу и помещаем в Minv
	lp[0] = LightPos[0];								
	lp[1] = LightPos[1];								
	lp[2] = LightPos[2];								
	lp[3] = LightPos[3];								// Храним повёрнутый световой вектор в массиве LP
	VMatMult(Minv, lp);									// Мы сохраняем мировые позиции относительно локальных координат системы в массиве "LP"
	glTranslatef(-ObjPos[0], -ObjPos[1], -ObjPos[2]);	// Добавление дает нам позицию света относительно локальной системы координат
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Возвращаем видовую матрицу из Minv
	wlp[0] = 0.0f;										// зануляем мировые кординаты
	wlp[1] = 0.0f;										
	wlp[2] = 0.0f;										
	wlp[3] = 1.0f;
	VMatMult(Minv, wlp);									// Мы сохраняем мировые позиции относительно локальных координат системы в массиве 'WLP "
	
	lp[0] += wlp[0];										// Добавление дает нам позицию света относительно локальной системы координат
	lp[1] += wlp[1];									
	lp[2] += wlp[2];	
	
	//glColor4f(0.7f, 0.4f, 0.0f, 1.0f);					
	glLoadIdentity();									
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(YPos, 1.0f, 0.0f, 0.0f);
	glRotatef(ZPos, 0.0f, 1.0f, 0.0f);
	DrawGLRoom();										// рисуем комнату
	glTranslatef(SpherePos[0], SpherePos[1], SpherePos[2]);	
	gluSphere(q, 1.5f, 20, 16);	
	glTranslatef(-SpherePos[0], -SpherePos[1], -SpherePos[2]);	
	glTranslatef(ObjPos[0], ObjPos[1], ObjPos[2]);		// задаем позицию объекта

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);					// Вращаем объект и тень
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glScalef(3.0f, 2.0f, 1.0f); 
	DrawGLObject(obj);									// Рисуем объект
	CastShadow(&obj, lp);								// рисуем тень

	//glColor4f(0.7f, 0.4f, 0.0f, 1.0f);				
	glDisable(GL_LIGHTING);								// выключаем свет
	glDepthMask(GL_FALSE);								// выключаем маску
	glTranslatef(lp[0], lp[1], lp[2]);					// позиция света

	//gluSphere(q, 0.2f, 16, 8);		
	glEnable(GL_LIGHTING);								// Включить свет
	glDepthMask(GL_TRUE);								// Включить маску глубины

	xrot += XPos;										
	yrot += XPos;										

	glFlush();											
	return TRUE;										
}

void ProcessKeyboard()									
{
	if (keys[VK_LEFT])	LightPos[0] -= 0.05f;				// Источник света влево
	if (keys[VK_RIGHT])	LightPos[0] += 0.05f;				// Источник света вправо
	if (keys[VK_UP])	LightPos[1] += 0.05f;				// Источник света вверх
	if (keys[VK_DOWN])	LightPos[1] -= 0.05f;				// Источник света вниз
	if (keys['Z'])		LightPos[2] += 0.05f;				// Источник света ближе
	if (keys['X'])		LightPos[2] -= 0.05f;				// Источник света дальше
	if (keys['D'])		ObjPos[0] += 0.02f;
	if (keys['A'])		ObjPos[0] -= 0.02f;
	if (keys['W'])		ObjPos[1] += 0.02f;
	if (keys['S'])		ObjPos[1] -= 0.02f;
	if (keys['Q'])		ObjPos[2] += 0.02f;
	if (keys['E'])		ObjPos[2] -= 0.02f;
	if (keys['R'])		XPos -= 0.0004f;
	if (keys['F'])		XPos += 0.0004f;
	if (keys['1'])		YPos -= 0.04f;
	if (keys['2'])		YPos += 0.04f;
	if (keys['3'])		ZPos -= 0.04f;
	if (keys['4'])		ZPos += 0.04f;

	if (keys['L'])		SpherePos[0] += 0.02f;
	if (keys['J'])		SpherePos[0] -= 0.02f;
	if (keys['I'])		SpherePos[1] += 0.02f;
	if (keys['K'])		SpherePos[1] -= 0.02f;
	if (keys['U'])		SpherePos[2] += 0.02f;
	if (keys['O'])		SpherePos[2] -= 0.02f;
}


GLvoid KillGLWindow(GLvoid)								// "Уничтожение" окна
{
	if (hRC)											// Есть ли контекст визуализации
	{
		if (!wglMakeCurrent(NULL,NULL))					// Если можно освободить контексты
		{
			MessageBox(NULL,"Не удалось остановить постоянный контекст рендеренга.","ОШИБКА ЗАВЕРШЕНИЯ",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Можно ли удалить контекст рендеренга
		{
			MessageBox(NULL,"Не удалось корректно удалить контекст рендеренга.","ОШИБКА ЗАВЕРШЕНИЯ",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Устанавливаем в NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Можем ли мы остановить контекст устройства
	{
		MessageBox(NULL,"Не удалось остановить контекст устройства.","ОШИБКА ЗАВЕРШЕНИЯ",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Устанавливаем в NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Можно ли уничтожить окно?
	{
		MessageBox(NULL,"Не удалось уничтожить окно.","ОШИБКА ЗАВЕРШЕНИЯ",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (fullscreen)										
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Можно ли отменить регистрацию класса?
	{
		MessageBox(NULL,"Не удалось отменить регистрацию классов.","ОШИБКА ЗАВЕРШЕНИЯ",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Устанавливаем в NULL
	}
}
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) // создание окна
{
	GLuint		PixelFormat;							// получит результат
	WNDCLASS	wc;										// структура Windows Class
	DWORD		dwExStyle;								// расширеный стиль окна
	DWORD		dwStyle;								// стиль окна

	fullscreen=fullscreenflag;							// флаг полноэкранного режима

	hInstance			= GetModuleHandle(NULL);		// создаём экземпляр окна
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// перерисовка при смене размера
	wc.lpfnWndProc		= (WNDPROC) WndProc;			// обработка события WndProc
	wc.cbClsExtra		= 0;							// нет экстраданных
	wc.cbWndExtra		= 0;							// нет экстраданных
	wc.hInstance		= hInstance;					// установка требований
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);	// загружаем дефолтную иконку
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);	// Загрузка курсора
	wc.hbrBackground	= NULL;							// отключаем фон
	wc.lpszMenuName		= NULL;							// отключаем меню
	wc.lpszClassName	= "OpenGL";						// Название

	if (!RegisterClass(&wc))							// Попытка регистрации класса окна
	{
		MessageBox(NULL,"Не удалось зарегистрировать Window Class.","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}
	
	if (fullscreen)										// разрешон полноэкранный режим?
	{
		DEVMODE dmScreenSettings;						// режим устройства
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// задаём размер памяти
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);	// размер структуры
		dmScreenSettings.dmPelsWidth	= width;		// устанавливаем ширину окна
		dmScreenSettings.dmPelsHeight	= height;		// устанавливаем высоту окна
		dmScreenSettings.dmBitsPerPel	= bits;			// бит на пиксель
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// попытка установить выбраный режим
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// если режим не удался, предлагаем окооный режим или выйти
			if (MessageBox(NULL,"Запрашиваемая Полноэкранный режим не поддерживается вашей видеокартой.\nИспользуйте оконный режим вместо этого?","Что то пошло не так",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;						// переключаемся в оконный режим
			}
			else										// в противном случаи
			{
				MessageBox(NULL,"Программа будет закрыта.","ОШИБКА",MB_OK|MB_ICONSTOP);
				return FALSE;							
			}
		}
	}

	if (fullscreen)										// если выбран полноэкранный режим
	{
		dwExStyle=WS_EX_APPWINDOW;						// расширеный стиль окна
		dwStyle=WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// стиль окна
		ShowCursor(FALSE);								// скрыть указатель мыши
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// расширеный стиль окна
		dwStyle=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	//стиль окна
	}

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,				// расширеный стиль для окна
								"OpenGL",				// имя
								title,					// заголовок окна
								dwStyle,				// стиль окна
								200, 200,				// позиция окна
								width, height,			// ширина\высота
								NULL,					// нет предка
								NULL,					// нет меню
								hInstance,				
								NULL)))					
	{
		KillGLWindow();									// сброс дисплея
		MessageBox(NULL,"Ошибка создания окна.","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=					// сообщаем, каким должно быть окно
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// размер пикселя
		1,												// номер версии
		PFD_DRAW_TO_WINDOW |							// формат должен поддерживать окно
		PFD_SUPPORT_OPENGL |							// формат должен поддерживать OpenGL
		PFD_DOUBLEBUFFER,								// должна поддерживаться двойная буферизация
		PFD_TYPE_RGBA,									// запрос формата RGBA
		bits,											// Выбираем глубину цвета
		0, 0, 0, 0, 0, 0,								// цвет битов игнорируется
		0,												// прозрачность оключена
		0,												// игнорируем сдвиг бита
		0,												// аккамулирующий буфер отключён
		0, 0, 0, 0,										// аккамулирующие биты игнорируются
		16,												// 16-ти битный буфер глубины  
		1,												// 
		0,												// нет вспомогательного буфера
		PFD_MAIN_PLANE,									// 
		0,												// 
		0, 0, 0											// 
	};
	
	if (!(hDC=GetDC(hWnd)))								// контекст устройства получен?
	{
		KillGLWindow();									// сброс дисплея
		MessageBox(NULL,"Не удается создать контекст GL устройства.","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))		// соответствующий формат найден?
	{
		KillGLWindow();									// сброс дисплея
		MessageBox(NULL,"Не удалось найти подходящйи формат пикселя.","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))			// возможно ли установить формат пикселя?
	{
		KillGLWindow();									// сброс дисплея
		MessageBox(NULL,"Не возможно установить формат пикселя.","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))					// Можем ли мы получить контекст визуализации?
	{
		KillGLWindow();									// сброс дисплея
		MessageBox(NULL,"Не удаётся создать контекст GL рендеринга.","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))						// Можем ли мы установить контекст визуализации?
	{
		KillGLWindow();									// сброс дисплея
		MessageBox(NULL,"Не можете активировать GL контекст визуализации.","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);							// выводим окно
	SetForegroundWindow(hWnd);							// ставим приоритет
	SetFocus(hWnd);										// Устанавливаем отлавливание клавиатуры в окне
	ReSizeGLScene(width, height);						// Точка зарения на экрана

	if (!InitGL())										// Инициализация
	{
		KillGLWindow();									// Уничтожаем окно
		MessageBox(NULL,"Не удалось инициализировать OpenGL.","ОШИБКА",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	return TRUE;										
}


LRESULT CALLBACK WndProc(	HWND	hWnd,				// Хендел окна
							UINT	uMsg,				// Сообщение
							WPARAM	wParam,				// Дополнительноое сообщение
							LPARAM	lParam)				// Дополнительная информация
{
	switch (uMsg)										// Устанавливаем сообщение окна
	{
		case WM_ACTIVATE:								// если активно
		{
			if (!HIWORD(wParam))						// Свернуть
			{
				active=TRUE;							// программа активна
			}
			else										
			{
				active=FALSE;							// программа не активна
			}

			return 0;									
		}

		case WM_SYSCOMMAND:								// Перехвад системных команд
		{
			switch (wParam)								// проверка системного вывода
			{
				case SC_SCREENSAVE:						
				case SC_MONITORPOWER:					
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:									// Сообщение о закрытии
		{
			PostQuitMessage(0);							// шлёт сообщение
			return 0;									
		}

		case WM_KEYDOWN:								
		{
			keys[wParam] = TRUE;				
			return 0;									
		}

		case WM_KEYUP:									
		{
			keys[wParam] = FALSE;					
			return 0;									
		}

		case WM_SIZE:									// вывод окна
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // установка ширины и высоты
			return 0;									
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,				// экземпляр
					HINSTANCE	hPrevInstance,			// предыдущий экземпляр
					LPSTR		lpCmdLine,				// параметры командной строки
					int			nCmdShow)				// Состояние окна
{
	MSG		msg;										// Структура сообщения окна
	BOOL	done=FALSE;									// переменная выхода из цикла

	if (!CreateGLWindow("OpenGL",800,600,32,fullscreen))
	{
		return 0;										// Выход если окно не создалось
	}

	while(!done)										// цикл
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))		// ожидаеное сообщение?
		{
			if (msg.message==WM_QUIT)					// получено сообщение о выходе
			{
				done=TRUE;								// разрешаем завершить цикл
			}
			else										
			{
				TranslateMessage(&msg);					// перевод сообщений
				DispatchMessage(&msg);					// отправляем сообщение
			}
		}
		else											
		{
			// Рисуем сцену. Проверяем нажатия кнопки ESC и сообщения о выходе от DrawGLScene ()
			if (active && keys[VK_ESCAPE])				// активный
			{
				done=TRUE;								// ESC или DrawGLScene поступил Сигнал
			}
			else										
			{
				DrawGLScene();							// рисуем сцену
				SwapBuffers(hDC);						// свап буфер \ двойная буферизация
				ProcessKeyboard();						// нажатие клавиши
			}
		}
	}

	
	KillGLWindow();										// уничтожаем окно
	return (msg.wParam);								// выходим из программы
}
