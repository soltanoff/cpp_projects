#include <windows.h>								
#include <tchar.h>
#include <direct.h>
#include <gl\gl.h>		 								
#include <gl\glu.h>										
#include <GL\glut.h>
#include <gl\GLAux.h>
#include "3dobjects.h"								

char* Object_Path;										// ���� � �������

typedef float GLvector4f[4];							// ��� ��� ��������� ������������ ������
typedef float GLmatrix16f[16];							// ��� ��� ��������� ������������ ������

HDC			hDC=NULL;									// GDI �������� ����������
HGLRC		hRC=NULL;									// ���������� �������� ����������
HWND		hWnd=NULL;									// ������ ������ ����
HINSTANCE	hInstance=NULL;								// ������ ��������� ����������
bool keys[256], keypressed[256];						// ������ �������� ������� �������
bool active=TRUE;										// ���� ������� �� ���������
bool fullscreen=FALSE;									// ������������� �����

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

glObject obj;	// ������

extern GLuint  texture[TextureCount];								// ������ ��� ��������

float LightPos[] = { 0.0f, 5.0f, 2.0f, 1.0f};			
float LightAmb[] = { 0.2f, 0.2f, 0.2f, 1.0f};			
float LightDif[] = { 0.6f, 0.6f, 0.6f, 1.0f};			
float LightSpc[] = {-0.2f, -0.2f, -0.2f, 1.0f};			

float ObjPos[] = {-2.0f,-2.0f,-5.0f};					// ������� �������

GLUquadricObj	*q;										// ��������� ��� ������ ��������� (��� ��������� �����)

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	

void VMatMult(GLmatrix16f M, GLvector4f v)					// ������������ ������
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

int InitGLObjects(char* path, glObject *obj)			// ������������� �������
{
	if (!ReadObject(path, obj))							// ���������� �������
	{
		return FALSE;									// ���� ������ ���������
	}

	SetConnectivity(obj);								// ���������� ����� "����� � ����"

	for (unsigned int i=0;i<(*obj).nPlanes;i++)			// ��� �� ���� "������ �����������"
		CalcPlane(*obj, &((*obj).planes[i]));				// ��������� ����� ��� ���� ������

	return TRUE;									
}

GLvoid LoadGLTextures(TCHAR* Path, unsigned int Index)
{
	// �������� ��������
	AUX_RGBImageRec *texture1;

	texture1 = auxDIBImageLoad(Path);
		// �������� ��������
	glGenTextures(1, &texture[Index]);
	glBindTexture(GL_TEXTURE_2D, texture[Index]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// �������� ������� ����
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// ���� ��������
	glClearDepth(1.0f);									// ��������� ������ �������
	glClearStencil(0);									// ��������� ������ ���������
	glEnable(GL_DEPTH_TEST);							// �������� ���� �������
	glDepthFunc(GL_LEQUAL);								// ��� ������������ �������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	// ��������� �����
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpc);
	glEnable(GL_LIGHT1);								// �������� ������� �����
	glEnable(GL_LIGHTING);								// ��������� ���������
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	
	q = gluNewQuadric();								// �������������� ������ ��������� ��� ������� (����� [����])
	gluQuadricNormals(q, GL_SMOOTH);					// ����������� �������
	gluQuadricTexture(q, GL_FALSE);						// ��������� �������������� ���������������
	
	Object_Path = new char[strlen("Data/Object.txt")+1];
	strcpy(Object_Path,"Data/Object.txt");
	if (!InitGLObjects(Object_Path, &obj)) return FALSE;					// ������������� ������������������ �������
	delete [] Object_Path;

	TCHAR current_work_dir[FILENAME_MAX];
	_getcwd(current_work_dir, sizeof(current_work_dir));

	/*TCHAR* Path = new TCHAR[strlen(current_work_dir) + strlen("\\Textures\\")+ strlen("Room.bmp") + 1];
	_tcscpy(Path, current_work_dir);
	_tcscat(Path, "\\Textures\\");
	_tcscat(Path, "Room.bmp");
	
	LoadGLTextures(Path);			// �������� �������
	delete [] Path;*/
	for (int i=0; i<TextureCount; i++)
	{
		TCHAR* Path = new TCHAR[strlen(current_work_dir) + strlen("\\Textures\\")+ strlen(TextureName[i]) + 1];
		_tcscpy(Path, current_work_dir);
		_tcscat(Path, "\\Textures\\");
		_tcscat(Path, TextureName[i]);
	
		LoadGLTextures(Path,i);			// �������� �������
		delete [] Path;
	}

	return TRUE;
}

void DrawGLRoom()										// ��������� �������
{
	glEnable(GL_TEXTURE_2D);							// ���������� ��������� ��������
	glBindTexture(GL_TEXTURE_2D, texture[8]);		// ������������� ��������
	glBegin(GL_QUADS);									// Begin Drawing Quads
		// ������ �����
		glNormal3f(0.0f, 1.0f, 0.0f);											// ������� ���������� �����
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f,-10.0f,-20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f,-10.0f, 20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f,-10.0f, 20.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f,-10.0f,-20.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture[1]);
	/*glBegin (GL_QUADS);	
		// ������� �����
		glTexCoord2f(0.0f, 1.0f); glNormal3f(0.0f,-1.0f, 0.0f);					// ������� ���������� ����
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 10.0f, 20.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 10.0f,-20.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 10.0f,-20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f, 10.0f, 20.0f);
	glEnd();/**/

	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin (GL_QUADS);	
		// ����������� �����
		glNormal3f(0.0f, 0.0f, 1.0f);											// ������� ���������� �� ����������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 10.0f,-20.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f,-10.0f,-20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f,-10.0f,-20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f, 10.0f,-20.0f);
	glEnd();/**/

	glBindTexture(GL_TEXTURE_2D, texture[3]);
	/*glBegin (GL_QUADS);
		// ����� �����
		glNormal3f(1.0f, 0.0f, 0.0f);											// ������� ���������� ������
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 10.0f, 20.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-10.0f,-10.0f, 20.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-10.0f,-10.0f,-20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 10.0f,-20.0f);
	glEnd();/**/

	glBindTexture(GL_TEXTURE_2D, texture[4]);
	/*glBegin (GL_QUADS);	
		// ������ �����
		glNormal3f(-1.0f, 0.0f, 0.0f);											// ������� ���������� ������
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f, 10.0f,-20.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f,-10.0f,-20.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 10.0f,-10.0f, 20.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 10.0f, 10.0f, 20.0f);
		
	glEnd();/**/
	glDisable(GL_TEXTURE_2D);							// ��������� ��������� ��������
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
	
	glLoadIdentity();									// ����� �������
	glTranslatef(0.0f, 0.0f, -20.0f);					
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);		// ������� Light1						

	glLoadIdentity();									
	glRotatef(-yrot, 0.0f, 1.0f, 0.0f);					
	glRotatef(-xrot, 1.0f, 0.0f, 0.0f);					
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// ��������� ������� ������� � �������� � Minv
	lp[0] = LightPos[0];								
	lp[1] = LightPos[1];								
	lp[2] = LightPos[2];								
	lp[3] = LightPos[3];								// ������ ��������� �������� ������ � ������� LP
	VMatMult(Minv, lp);									// �� ��������� ������� ������� ������������ ��������� ��������� ������� � ������� "LP"
	glTranslatef(-ObjPos[0], -ObjPos[1], -ObjPos[2]);	// ���������� ���� ��� ������� ����� ������������ ��������� ������� ���������
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// ���������� ������� ������� �� Minv
	wlp[0] = 0.0f;										// �������� ������� ���������
	wlp[1] = 0.0f;										
	wlp[2] = 0.0f;										
	wlp[3] = 1.0f;
	VMatMult(Minv, wlp);									// �� ��������� ������� ������� ������������ ��������� ��������� ������� � ������� 'WLP "
	
	lp[0] += wlp[0];										// ���������� ���� ��� ������� ����� ������������ ��������� ������� ���������
	lp[1] += wlp[1];									
	lp[2] += wlp[2];	
	
	//glColor4f(0.7f, 0.4f, 0.0f, 1.0f);					
	glLoadIdentity();									
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(YPos, 1.0f, 0.0f, 0.0f);
	glRotatef(ZPos, 0.0f, 1.0f, 0.0f);
	DrawGLRoom();										// ������ �������
	glTranslatef(SpherePos[0], SpherePos[1], SpherePos[2]);	
	gluSphere(q, 1.5f, 20, 16);	
	glTranslatef(-SpherePos[0], -SpherePos[1], -SpherePos[2]);	
	glTranslatef(ObjPos[0], ObjPos[1], ObjPos[2]);		// ������ ������� �������

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);					// ������� ������ � ����
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);
	glScalef(3.0f, 2.0f, 1.0f); 
	DrawGLObject(obj);									// ������ ������
	CastShadow(&obj, lp);								// ������ ����

	//glColor4f(0.7f, 0.4f, 0.0f, 1.0f);				
	glDisable(GL_LIGHTING);								// ��������� ����
	glDepthMask(GL_FALSE);								// ��������� �����
	glTranslatef(lp[0], lp[1], lp[2]);					// ������� �����

	//gluSphere(q, 0.2f, 16, 8);		
	glEnable(GL_LIGHTING);								// �������� ����
	glDepthMask(GL_TRUE);								// �������� ����� �������

	xrot += XPos;										
	yrot += XPos;										

	glFlush();											
	return TRUE;										
}

void ProcessKeyboard()									
{
	if (keys[VK_LEFT])	LightPos[0] -= 0.05f;				// �������� ����� �����
	if (keys[VK_RIGHT])	LightPos[0] += 0.05f;				// �������� ����� ������
	if (keys[VK_UP])	LightPos[1] += 0.05f;				// �������� ����� �����
	if (keys[VK_DOWN])	LightPos[1] -= 0.05f;				// �������� ����� ����
	if (keys['Z'])		LightPos[2] += 0.05f;				// �������� ����� �����
	if (keys['X'])		LightPos[2] -= 0.05f;				// �������� ����� ������
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


GLvoid KillGLWindow(GLvoid)								// "�����������" ����
{
	if (hRC)											// ���� �� �������� ������������
	{
		if (!wglMakeCurrent(NULL,NULL))					// ���� ����� ���������� ���������
		{
			MessageBox(NULL,"�� ������� ���������� ���������� �������� ����������.","������ ����������",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// ����� �� ������� �������� ����������
		{
			MessageBox(NULL,"�� ������� ��������� ������� �������� ����������.","������ ����������",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// ������������� � NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// ����� �� �� ���������� �������� ����������
	{
		MessageBox(NULL,"�� ������� ���������� �������� ����������.","������ ����������",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// ������������� � NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// ����� �� ���������� ����?
	{
		MessageBox(NULL,"�� ������� ���������� ����.","������ ����������",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (fullscreen)										
	{
		ChangeDisplaySettings(NULL,0);					
		ShowCursor(TRUE);
	}

	if (!UnregisterClass("OpenGL",hInstance))			// ����� �� �������� ����������� ������?
	{
		MessageBox(NULL,"�� ������� �������� ����������� �������.","������ ����������",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// ������������� � NULL
	}
}
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag) // �������� ����
{
	GLuint		PixelFormat;							// ������� ���������
	WNDCLASS	wc;										// ��������� Windows Class
	DWORD		dwExStyle;								// ���������� ����� ����
	DWORD		dwStyle;								// ����� ����

	fullscreen=fullscreenflag;							// ���� �������������� ������

	hInstance			= GetModuleHandle(NULL);		// ������ ��������� ����
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// ����������� ��� ����� �������
	wc.lpfnWndProc		= (WNDPROC) WndProc;			// ��������� ������� WndProc
	wc.cbClsExtra		= 0;							// ��� ������������
	wc.cbWndExtra		= 0;							// ��� ������������
	wc.hInstance		= hInstance;					// ��������� ����������
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);	// ��������� ��������� ������
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);	// �������� �������
	wc.hbrBackground	= NULL;							// ��������� ���
	wc.lpszMenuName		= NULL;							// ��������� ����
	wc.lpszClassName	= "OpenGL";						// ��������

	if (!RegisterClass(&wc))							// ������� ����������� ������ ����
	{
		MessageBox(NULL,"�� ������� ���������������� Window Class.","������",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}
	
	if (fullscreen)										// �������� ������������� �����?
	{
		DEVMODE dmScreenSettings;						// ����� ����������
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// ����� ������ ������
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);	// ������ ���������
		dmScreenSettings.dmPelsWidth	= width;		// ������������� ������ ����
		dmScreenSettings.dmPelsHeight	= height;		// ������������� ������ ����
		dmScreenSettings.dmBitsPerPel	= bits;			// ��� �� �������
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// ������� ���������� �������� �����
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// ���� ����� �� ������, ���������� ������� ����� ��� �����
			if (MessageBox(NULL,"������������� ������������� ����� �� �������������� ����� �����������.\n����������� ������� ����� ������ �����?","��� �� ����� �� ���",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;						// ������������� � ������� �����
			}
			else										// � ��������� ������
			{
				MessageBox(NULL,"��������� ����� �������.","������",MB_OK|MB_ICONSTOP);
				return FALSE;							
			}
		}
	}

	if (fullscreen)										// ���� ������ ������������� �����
	{
		dwExStyle=WS_EX_APPWINDOW;						// ���������� ����� ����
		dwStyle=WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// ����� ����
		ShowCursor(FALSE);								// ������ ��������� ����
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// ���������� ����� ����
		dwStyle=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	//����� ����
	}

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,				// ���������� ����� ��� ����
								"OpenGL",				// ���
								title,					// ��������� ����
								dwStyle,				// ����� ����
								200, 200,				// ������� ����
								width, height,			// ������\������
								NULL,					// ��� ������
								NULL,					// ��� ����
								hInstance,				
								NULL)))					
	{
		KillGLWindow();									// ����� �������
		MessageBox(NULL,"������ �������� ����.","������",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=					// ��������, ����� ������ ���� ����
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// ������ �������
		1,												// ����� ������
		PFD_DRAW_TO_WINDOW |							// ������ ������ ������������ ����
		PFD_SUPPORT_OPENGL |							// ������ ������ ������������ OpenGL
		PFD_DOUBLEBUFFER,								// ������ �������������� ������� �����������
		PFD_TYPE_RGBA,									// ������ ������� RGBA
		bits,											// �������� ������� �����
		0, 0, 0, 0, 0, 0,								// ���� ����� ������������
		0,												// ������������ ��������
		0,												// ���������� ����� ����
		0,												// �������������� ����� ��������
		0, 0, 0, 0,										// �������������� ���� ������������
		16,												// 16-�� ������ ����� �������  
		1,												// 
		0,												// ��� ���������������� ������
		PFD_MAIN_PLANE,									// 
		0,												// 
		0, 0, 0											// 
	};
	
	if (!(hDC=GetDC(hWnd)))								// �������� ���������� �������?
	{
		KillGLWindow();									// ����� �������
		MessageBox(NULL,"�� ������� ������� �������� GL ����������.","������",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))		// ��������������� ������ ������?
	{
		KillGLWindow();									// ����� �������
		MessageBox(NULL,"�� ������� ����� ���������� ������ �������.","������",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))			// �������� �� ���������� ������ �������?
	{
		KillGLWindow();									// ����� �������
		MessageBox(NULL,"�� �������� ���������� ������ �������.","������",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))					// ����� �� �� �������� �������� ������������?
	{
		KillGLWindow();									// ����� �������
		MessageBox(NULL,"�� ������ ������� �������� GL ����������.","������",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))						// ����� �� �� ���������� �������� ������������?
	{
		KillGLWindow();									// ����� �������
		MessageBox(NULL,"�� ������ ������������ GL �������� ������������.","������",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);							// ������� ����
	SetForegroundWindow(hWnd);							// ������ ���������
	SetFocus(hWnd);										// ������������� ������������ ���������� � ����
	ReSizeGLScene(width, height);						// ����� ������� �� ������

	if (!InitGL())										// �������������
	{
		KillGLWindow();									// ���������� ����
		MessageBox(NULL,"�� ������� ���������������� OpenGL.","������",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;									// Return FALSE
	}

	return TRUE;										
}


LRESULT CALLBACK WndProc(	HWND	hWnd,				// ������ ����
							UINT	uMsg,				// ���������
							WPARAM	wParam,				// ��������������� ���������
							LPARAM	lParam)				// �������������� ����������
{
	switch (uMsg)										// ������������� ��������� ����
	{
		case WM_ACTIVATE:								// ���� �������
		{
			if (!HIWORD(wParam))						// ��������
			{
				active=TRUE;							// ��������� �������
			}
			else										
			{
				active=FALSE;							// ��������� �� �������
			}

			return 0;									
		}

		case WM_SYSCOMMAND:								// �������� ��������� ������
		{
			switch (wParam)								// �������� ���������� ������
			{
				case SC_SCREENSAVE:						
				case SC_MONITORPOWER:					
				return 0;							
			}
			break;									
		}

		case WM_CLOSE:									// ��������� � ��������
		{
			PostQuitMessage(0);							// ��� ���������
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

		case WM_SIZE:									// ����� ����
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // ��������� ������ � ������
			return 0;									
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,				// ���������
					HINSTANCE	hPrevInstance,			// ���������� ���������
					LPSTR		lpCmdLine,				// ��������� ��������� ������
					int			nCmdShow)				// ��������� ����
{
	MSG		msg;										// ��������� ��������� ����
	BOOL	done=FALSE;									// ���������� ������ �� �����

	if (!CreateGLWindow("OpenGL",800,600,32,fullscreen))
	{
		return 0;										// ����� ���� ���� �� ���������
	}

	while(!done)										// ����
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))		// ��������� ���������?
		{
			if (msg.message==WM_QUIT)					// �������� ��������� � ������
			{
				done=TRUE;								// ��������� ��������� ����
			}
			else										
			{
				TranslateMessage(&msg);					// ������� ���������
				DispatchMessage(&msg);					// ���������� ���������
			}
		}
		else											
		{
			// ������ �����. ��������� ������� ������ ESC � ��������� � ������ �� DrawGLScene ()
			if (active && keys[VK_ESCAPE])				// ��������
			{
				done=TRUE;								// ESC ��� DrawGLScene �������� ������
			}
			else										
			{
				DrawGLScene();							// ������ �����
				SwapBuffers(hDC);						// ���� ����� \ ������� �����������
				ProcessKeyboard();						// ������� �������
			}
		}
	}

	
	KillGLWindow();										// ���������� ����
	return (msg.wParam);								// ������� �� ���������
}
