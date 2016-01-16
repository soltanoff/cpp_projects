#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "Glaux.lib")

#include <TCHAR.h>
#include <windows.h>      //Заголовочный файл для Windows
#include <direct.h>
#include <gl\glut.h>
#include <stdio.h>        //Заголовочный файл стандартного ввода/вывода
#include <gl\glaux.h>     //Заголовочный файл библиотеки GLaux


const int TextureCount = 10;

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

GLfloat ctrlpoints[4][4][3] = {
 {{-1.5, 1.0, -1.5},  {-0.5, 1.0,-1.5 }, {0.5, 1.0, -1.5 },   {1.5, 1.0,-1.5}}, 
 {{-1.5, 1.0, -0.5},  {-0.5, 2.0,-0.5 }, {0.5, 2.0, -0.5 },   {1.5, 1.0,-0.5}}, 
 {{-1.5, 1.0,  0.5},  {-0.5, 2.0, 0.5 }, {0.5, 2.0,  0.5 },   {1.5, 1.0, 0.5}}, 
 {{-1.5, 1.0,  1.5},  {-0.5, 1.0, 1.5 }, {0.5, 1.0,  1.5 },   {1.5, 1.0, 1.5}}
};

float fogColor[4] = {0.5f, 0.5f, 0.5f, 1.0f}; // Цвет тумана 

GLuint  texture[TextureCount];     // Память для шести текстуры

GLuint CUBE_LIST = glGenLists(1);

float zeroPos[3][4] = { // позиции источников света
	 0,  7, 0, 1,
	-7, -5, 0, 1,
	 7, -5, 0, 1
};

float LParam[3][4] = {								// Параметры света
	1.0, 1.0, 1.0, 1.0,
	1.0, 1.0, 1.0, 1.0,
	0.8, 0.4, 0.0, 0.0
};
double rotate_y=0;									// коэфициент смещения по оси X
double rotate_x=0;	

bool Fogi = false;
bool Blend = false;
bool Room = false;

bool Light_0 = false;
bool Light_1 = true;
bool Light_2 = false; 

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Очищаем "рисуемый" буфер и буфер глубины									
	glLoadIdentity();												// ПереИнициализируем параметры (сохраняем изменения)

	if (Fogi)
		glEnable(GL_FOG);
	else
		glDisable(GL_FOG);

	if (Blend)
	{
		glEnable(GL_BLEND); // прозрачность вкл
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}
	else
	{
		glDisable(GL_BLEND);
	}


	glPushMatrix();													// Запоминаем текущую систему координат перед вращением сцены
	glLightfv(GL_LIGHT0, GL_POSITION, zeroPos[0] );
	glLightfv(GL_LIGHT1, GL_POSITION, zeroPos[1] );
	glLightfv(GL_LIGHT2, GL_POSITION, zeroPos[2] );
	glPopMatrix();


	glPushMatrix();
	gluPerspective(45.0f, 1, 50.0f, 0.0f);
	//glTranslatef(0.0, -1.5, 0.0);
	glRotatef( -9100, 1.0, 1.0, 0.0 );
	glRotatef( -14, 1.0, 1.0, 0.0 );
	glRotatef( -126*50, 0.0, 1.0, 1.0 );
	glRotatef( -1050, 0.0, 1.0, 0.0 );
	//printf("rotate_x: %f\n", rotate_x*50);
	glTranslatef(-80.0, -0.5, 0.0);
	glScalef(30.0, 3.0, 3.0);
	for (int j = 8; j <= 130; j++) 
	{
		glBegin(GL_LINE_STRIP);
		for (int i = 10; i <= 130; i++)
			glEvalCoord2f((GLfloat)i/10.0, (GLfloat)j/8.0);
		glEnd();
		glBegin(GL_LINE_STRIP);
		for (int i = 10; i <= 130; i++)
			glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/10.0);
		glEnd();
	}
	glPopMatrix();
	
	//glRotatef( rotate_x, 0.0, 1.0, 0.0 );

	glPushMatrix();
	//gluPerspective(45.0f, 1, 50.0f, 0.0f);
	glTranslatef(0.0, 3.0, -0.0);
	//glScalef (2.0, 0.4, 1.0);
	glRotatef( 90, 1.0, 0.0, 0.0 );
	glRotatef( rotate_x, -1.0, 0.0, 0.0 );			// вращение по оси X
	glRotatef( rotate_y, 0.0, -1.0, 0.0 );			// вращение по оси Z
	glCallList(1);
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(0.0, -2.0, 0.0);
	glScalef (0.25, 0.25, 1.0);
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );			// вращение по оси X
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );			// вращение по оси Z
	glCallList(1);
	glPopMatrix();
	
	//glScalef(1.0, 1.0, -0.5);
	//gluPerspective(45.0f, 1, 50.0f, 0.0f);
	
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	glScalef (0.65, 0.65, 1.0);
	glRotatef( 90, 1.0, 0.0, 0.0 );
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );			// вращение по оси X
	glRotatef( rotate_y, 0.0, -1.0, 0.0 );			// вращение по оси Z
	glCallList(1);
	glPopMatrix();

	glPushMatrix();
	//gluPerspective(45.0f, 1, 50.0f, 0.0f);
	glTranslatef(3.0, 0.0, 0.0);
	glScalef (0.25, 0.65, 1.0);
	glRotatef( 90, 1.0, 0.0, 0.0 );
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );			// вращение по оси X
	glRotatef( rotate_y, 0.0, -1.0, 0.0 );			// вращение по оси Z
	glCallList(2);
	glPopMatrix();
	
	glPushMatrix();
	//gluPerspective(45.0f, 1, 50.0f, 0.0f);
	glTranslatef(-3.0, 0.0, 0.0);
	glScalef (0.4, 0.6, 1.0);
	glRotatef( 90, 1.0, 0.0, 0.0 );
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );			// вращение по оси X
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );			// вращение по оси Z
	glCallList(2);
	glPopMatrix();

	glDisable(GL_BLEND);
	if (Room)
	{
		glPushMatrix();
		gluPerspective(45.0f, 1, 50.0f, 0.0f);
		//glTranslatef(3.0, 0.0, 0.0);
		glScalef (0.65, 0.65, 1.0);
		glRotatef( 90, 1.0, 0.0, 0.0 );
		//glRotatef( rotate_x, 1.0, 0.0, 0.0 );			// вращение по оси X
		//glRotatef( rotate_y, 0.0, -1.0, 0.0 );			// вращение по оси Z
		glCallList(2);
		glPopMatrix();
	}

    glFlush();
    glutSwapBuffers();												// Ждём, пока всё не прорисуется

}

void myKeyboardFunc( unsigned char key, int x, int y )
{
	switch ( key ) {
	case '7':
		Light_0 = !Light_0;
		if (Light_0) glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);
		break;
	case '8':	
		Light_1 = !Light_1;
		if (Light_1) glEnable(GL_LIGHT1);
		else glDisable(GL_LIGHT1);
		break;
	case '9':	
		Light_2 = !Light_2;
		if (Light_2) glEnable(GL_LIGHT2);
		else glDisable(GL_LIGHT2);
		break;
	case '2':												
		Room=!Room;
		break;
	case '3':												
		Blend=!Blend;
		break;
	case '4':
		Fogi=!Fogi;
		break;
	case 27:												// Нажат ESC
		exit(1);											// Плановое завершение программы
	}
	glutPostRedisplay();									// Обновляем дисплей
}

void mySpecialKeys( int key, int x, int y ) 
{
	if (key == GLUT_KEY_RIGHT)
		rotate_y -= 7;
	else if (key == GLUT_KEY_LEFT)
		rotate_y += 7;
	else if (key == GLUT_KEY_UP)
		rotate_x -= 7;
	else if (key == GLUT_KEY_DOWN)
		rotate_x += 7;

	glutPostRedisplay();								// Обновляем дисплей
}

void CreateList()
{
	CUBE_LIST = glGenLists(1);
	glNewList(CUBE_LIST, GL_COMPILE);
	// рисуем куб

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin (GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f (1.0, 1.0, 1.0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (-1.0, 1.0, 1.0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (-1.0, -1.0, 1.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f (1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin (GL_QUADS);
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (1.0, 1.0, -1.0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (1.0, -1.0, -1.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (-1.0, -1.0, -1.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin (GL_QUADS);
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0f, 1.0f);  
		glVertex3f (-1.0, 1.0, 1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f (-1.0, 1.0, -1.0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (-1.0, -1.0, -1.0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (-1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin (GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);   
		glVertex3f (1.0, 1.0, 1.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f (1.0, -1.0, 1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f (1.0, -1.0, -1.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f (1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin (GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);  
		glVertex3f (-1.0, 1.0, -1.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f (-1.0, 1.0, 1.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (1.0, 1.0, 1.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);  
		glVertex3f (-1.0, -1.0, -1.0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (1.0, -1.0, -1.0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (1.0, -1.0, 1.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f (-1.0, -1.0, 1.0);
	glEnd();
	glEndList();


	CUBE_LIST = glGenLists(2);
	glNewList(CUBE_LIST, GL_COMPILE);
	// рисуем куб

	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin (GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f (1.0, 1.0, 1.0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (-1.0, 1.0, 1.0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (-1.0, -1.0, 1.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f (1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin (GL_QUADS);
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (1.0, 1.0, -1.0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (1.0, -1.0, -1.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (-1.0, -1.0, -1.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (-1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin (GL_QUADS);
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0f, 1.0f);  
		glVertex3f (-1.0, 1.0, 1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f (-1.0, 1.0, -1.0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (-1.0, -1.0, -1.0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (-1.0, -1.0, 1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin (GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(1.0f, 1.0f);   
		glVertex3f (1.0, 1.0, 1.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f (1.0, -1.0, 1.0);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f (1.0, -1.0, -1.0);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f (1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin (GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(1.0f, 0.0f);  
		glVertex3f (-1.0, 1.0, -1.0);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f (-1.0, 1.0, 1.0);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f (1.0, 1.0, 1.0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f (1.0, 1.0, -1.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0f, 0.0f);  
		glVertex3f (-1.0, -1.0, -1.0);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f (1.0, -1.0, -1.0);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f (1.0, -1.0, 1.0);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f (-1.0, -1.0, 1.0);
	glEnd();

	//glDisable(GL_BLEND); // прозрачность выкл
	glEndList();
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

void initRendering()
{
    glEnable(GL_DEPTH_TEST);								// Включаем буфер глубины
	glEnable(GL_ALPHA_TEST);								// Разрешаем смешение цветов (для прозрачности)
	glEnable(GL_LIGHTING);	
	
	glEnable(GL_TEXTURE_2D);		// Разрешение наложение текстуры
	
	glEnable(GL_FOG);				// Включает туман (GL_FOG) 
	glFogi(GL_FOG_MODE, GL_LINEAR);// Выбираем тип тумана 
	glFogfv(GL_FOG_COLOR, fogColor); // Устанавливаем цвет тумана 
	glFogi(GL_FOG_MODE, GL_EXP2); // Насколько густым будет туман 
	glHint(GL_FOG_HINT, GL_DONT_CARE); // Вспомогательная установка тумана 
	glFogf(GL_FOG_START, 0.0); // Глубина, с которой начинается туман 
	glFogf(GL_FOG_END, 5.0); // Глубина, где туман заканчивается. 
	
	TCHAR current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir));
	
	for (int i=0; i<TextureCount; i++)
	{
		TCHAR* Path = new TCHAR[strlen(current_work_dir) + strlen("\\Textures\\")+ strlen(TextureName[i]) + 1];
		_tcscpy(Path, current_work_dir);
		_tcscat(Path, "\\Textures\\");
		_tcscat(Path, TextureName[i]);
	
		LoadGLTextures(Path,i);			// Загрузка текстур
		delete [] Path;
	}
	
	CreateList();
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, LParam[0]);			// источник света 0 - настройка света
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LParam[0]);			// источник света 0 - настройка рассеянного света
	glLightfv(GL_LIGHT0, GL_SPECULAR, LParam[0]);			// источник света 0 - настройка отражаемого света
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LParam[1]);			// источник света 1 - настройка света
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LParam[1]);			// источник света 1 - настройка рассеянного света
	glLightfv(GL_LIGHT1, GL_SPECULAR, LParam[1]);				// источник света 1 - настройка отражаемого света
	
	glLightfv(GL_LIGHT2, GL_AMBIENT, LParam[2]);			// источник света 2 - настройка света
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LParam[2]);			// источник света 2 - настройка рассеянного света
	glLightfv(GL_LIGHT2, GL_SPECULAR, LParam[2]);	

	if (Light_0) glEnable(GL_LIGHT0);
	if (Light_1) glEnable(GL_LIGHT1);
	if (Light_2) glEnable(GL_LIGHT2);

   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpoints[0][0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glEnable(GL_AUTO_NORMAL);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
}

void resizeWindow(int width, int height)
{
    /*glViewport(0, 0, width, height);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(-5.0, 5.0, -5.0, 5.0, 2.0, 12.0);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0,0.0);
	glMatrixMode(GL_MODELVIEW);
	/**/
	// Предупредим деление на нуль;
    if (height == 0) height = 1;
    // Сбрасываем текущую область просмотра;
    glViewport(0, 0, width, height);
    // Выбираем матрицу проекций;
    glMatrixMode(GL_PROJECTION);
    // Сбрасываем выбранную матрицу;
    glLoadIdentity();
    // Вычисляем новые геометрические размеры сцены;
	glOrtho(-5.0, 5.0, -5.0, 5.0, 2.0, 12.0);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0,0.0);
    //gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
    // Выбираем матрицу вида модели;
    glMatrixMode(GL_MODELVIEW);
    // Сбрасываем ее;
    glLoadIdentity();
}

/*GLuint create_shader(GLenum shaderType, source)
{
    //# Создаем пустой объект шейдера
    shader = glCreateShader(shader_type)
    //# Привязываем текст шейдера к пустому объекту шейдера
    glShaderSource(shader, source)
    //# Компилируем шейдер
    glCompileShader(shader)
    //# Возвращаем созданный шейдер
    return shader;
}

void draw_shader()
{

}*/

int main( int argc, char** argv )
{
	//glewInit(&argc,argv);
	glutInit(&argc,argv);																// инициализируем glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );							// Инициализируем режим прорисовки

	glutInitWindowSize(800, 600);														// устанавливаем размер окна
	glutInitWindowPosition(200, 200);													// устанавливаем позицию окна
    glutCreateWindow("OpenGL");															// создаём окно

    initRendering();																	// Инициализируем параметры OpenGL

	glutKeyboardFunc(myKeyboardFunc);													// Назначаем функцию отбработки клафиш
	glutSpecialFunc(mySpecialKeys);														// Назначаем функцию отбработки специальных клафиш

    glutReshapeFunc(resizeWindow);														// Назначаем функцию перестроения после изменения размеров окна
    glutDisplayFunc(drawScene);															// Назначаем функцию "отрисовки"

	glutMainLoop();																		// "цикл" рендеренга

    return(0);	// This line is never reached.
}