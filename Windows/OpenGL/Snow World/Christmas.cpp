/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name : ROHAN SATYAMURTHY
//	Program : Tree Texture
//	Date : 29 March 2021
//	Modified : 31 March 2021
//	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//header file inclusion
#include<windows.h>				//standard windows file
#include<stdio.h>				//stadard C files
#include<stdlib.h>				//for malloc , or other libraries function
#include<gl/GL.h>				//Graphics Library header file
#include<gl/GLU.h>				//OpenGL utility library
#include"IconHeader.h"			//icon header file(Resource file)
#include "WIN.ini"				//color array

//library include
#pragma comment(lib,"opengl32.lib")			//OpenGL files
#pragma comment(lib,"glu32.lib")			//GLU files
#pragma comment(lib,"Winmm.lib")			//Play Sound


//Macro Definitions
#define WIN_WIDTH	800			//width and height of window
#define WIN_HEIGHT	600

//Global function declaration

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

//Global Variables
FILE* gpFILE = NULL;		//file pointer for Log file
SYSTEMTIME st;

//Global Variables Variable for fullscreen
DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
bool gbFullscreen = false;
HWND ghwnd = NULL;

//Global Variables for initialize & game loop
HDC ghdc = NULL;
HGLRC ghrc = NULL;
bool gbActiveWindow = false;

//Camera
GLfloat zaxis = 200.0f;
GLfloat yaxis = 0.0f;
GLfloat xaxis = 0.0f;

//texture
GLuint grass_texture;
GLuint bark_texture;
GLuint snow_texture;
GLuint mountain_texture;
GLuint fence_texture;
GLuint apple_screen;
GLuint grey;
GLuint keyboard;
GLuint sky;
GLuint blue;
GLuint front;
GLuint right;
GLuint left;
GLuint back;
GLuint top;
GLuint bottom;
GLuint bird;


//Font Rendering
HFONT hVertexFont;
GLuint uiFontList;

//Sphere
GLUquadric* quadric = NULL;

//Main Entry Point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInsatnce, LPSTR lpszCmdLine, int iCmdShow)
{
	//Function declaration
	void initialize(void);
	void display(void);

	//Local Variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("OpenGLTemplate");
	bool bDone = false;

	//Centering of Window
	int X = GetSystemMetrics(SM_CXSCREEN);
	int Y = GetSystemMetrics(SM_CYSCREEN);

	//creating a log file
	if (fopen_s(&gpFILE, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot Create Log File."), TEXT("ERROR"), MB_OK);
		exit(0);
	}
	else
	{
		
		fprintf(gpFILE,"Log Created Successfully.\n");
	}

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	//Register your class

	RegisterClassEx(&wndclass);

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("OpenGL Template Window"),
		WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		((X/2) - (WIN_WIDTH/2)),
		((Y/2) - (WIN_HEIGHT/2)),
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;	//setting handle to window to global varaible for further use in the program as hwnd is local to WinMain.
	
	initialize();

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//UpdateWindow(hwnd);

	//Game Loop

	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}//end of peek message
		else
		{
			if (gbActiveWindow == true)
			{
				display();
			}
		}
	}//end of while

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//Local Function Declaration
	void ToggleFullscreen(void);
	void resize(int, int);
	void uninitialize(void);

	//Local Variable
	GetSystemTime(&st);

	switch (iMsg)
	{
	case WM_CREATE:
		MessageBox(hwnd,TEXT("WM_CREATE come only once !!!"),TEXT("WM_CREATE"),MB_OK);
		break;

	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;

	case WM_ERASEBKGND:
		return(0);

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{

		case VK_ESCAPE:
		case 'x':
		case 'X':
		case ' ':
			DestroyWindow(hwnd);
			break;

		case 0x46:
		case 0x66:
			ToggleFullscreen();
			break;

		case 'W':
		case 'w':
			zaxis = zaxis - 0.50f;
			break;

		case 'S':
		case 's':
			zaxis = zaxis + 0.50f;
			break;

		case 'Y':
			yaxis = yaxis + 0.50;
			break;

		case 'H':
			yaxis = yaxis - 0.50f;
			break;

		case 'A':
			xaxis = xaxis - 0.50f;
			break;

		case 'D':
				xaxis = xaxis + 0.50f;
			break;

		default:
			break;

		}
		break;
	
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PlaySound(NULL, 0, 0);
		fprintf(gpFILE, "Program executed successfully Date : %d-%d-%d Time:%d:%d:%d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
		PostQuitMessage(0);
		break;

	default:
		break;

	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

//Fullscreen code
void ToggleFullscreen(void)
{
	//local variable declaration
	MONITORINFO mi = {sizeof(MONITORINFO)};

	//code

	if (gbFullscreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd,&wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd,MONITORINFOF_PRIMARY),&mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, (dwStyle & ~WS_OVERLAPPEDWINDOW));
				SetWindowPos(ghwnd,HWND_TOP,mi.rcMonitor.left,mi.rcMonitor.top,(mi.rcMonitor.right - mi.rcMonitor.left),(mi.rcMonitor.bottom - mi.rcMonitor.top),SWP_NOZORDER|SWP_FRAMECHANGED);
			}
		}

		ShowCursor(FALSE);
		gbFullscreen = true;
	}
	else
	{
		SetWindowLong(ghwnd,GWL_STYLE,(dwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd,&wpPrev);
		SetWindowPos(ghwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		gbFullscreen = false;
	}

}

void initialize(void)
{
	void resize(int,int);
	//1 change
	bool loadGLTexture(GLuint*, TCHAR []);


	//local variable declaration
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code

	ghdc = GetDC(ghwnd);

	ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL| PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	iPixelFormatIndex = ChoosePixelFormat(ghdc,&pfd);

	if (iPixelFormatIndex == 0)
	{
		fprintf(gpFILE, "ChoosePixelFormat() failed.\n");
		DestroyWindow(ghwnd);
	}
	
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		fprintf(gpFILE, "SetPixelFormat() failed.\n");
		DestroyWindow(ghwnd);
	}

	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL)
	{
		fprintf(gpFILE,"wglCreateContext() failed.\n");
		DestroyWindow(ghwnd);
	}

	if (wglMakeCurrent(ghdc,ghrc) == FALSE)
	{
		fprintf(gpFILE,"wglMakeCurrent() failed.\n");
		DestroyWindow(ghwnd);
	}

	//Depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	//Loading texture(2 change)
	loadGLTexture(&grass_texture,MAKEINTRESOURCE(GRASS));
	loadGLTexture(&bark_texture,MAKEINTRESOURCE(BARK));
	loadGLTexture(&snow_texture, MAKEINTRESOURCE(SNOW));
	loadGLTexture(&mountain_texture, MAKEINTRESOURCE(MOUNTAIN));
	loadGLTexture(&fence_texture, MAKEINTRESOURCE(FENCE));
	loadGLTexture(&apple_screen, MAKEINTRESOURCE(APPLE_SCREEN));
	loadGLTexture(&keyboard, MAKEINTRESOURCE(KEYBOARD));
	loadGLTexture(&sky, MAKEINTRESOURCE(SKY));
	loadGLTexture(&blue, MAKEINTRESOURCE(BLUE));
	loadGLTexture(&front, MAKEINTRESOURCE(FRONT));
	loadGLTexture(&right, MAKEINTRESOURCE(RIGHT));
	loadGLTexture(&left, MAKEINTRESOURCE(LEFT));
	loadGLTexture(&back, MAKEINTRESOURCE(BACK));
	loadGLTexture(&top, MAKEINTRESOURCE(TOP));
	loadGLTexture(&bottom, MAKEINTRESOURCE(BOTTOM));
	loadGLTexture(&bird, MAKEINTRESOURCE(BIRD));

		//Font

	//initialization for font
	LOGFONT logFont;
	logFont.lfHeight = 40;
	logFont.lfWidth = 20;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;
	logFont.lfWeight = FW_DONTCARE;
	logFont.lfItalic = FALSE;
	logFont.lfUnderline = FALSE;
	logFont.lfStrikeOut = FALSE;
	logFont.lfCharSet = ANSI_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont.lfQuality = DEFAULT_QUALITY;
	logFont.lfPitchAndFamily = DEFAULT_PITCH;

	hVertexFont = CreateFontIndirect(&logFont);

	SelectObject(ghdc, hVertexFont);
	uiFontList = glGenLists(128);
	//wglUseFontOutlines(ghdc, 0, 128, guiFontList, 0.0f, 0.5f, WGL_FONT_POLYGONS, agmf);
	wglUseFontBitmaps(ghdc, 0, 128, uiFontList);


	//(3 change)
	glEnable(GL_TEXTURE_2D);

	//set clear color
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

	//warmup call to resize
	resize(WIN_WIDTH, WIN_HEIGHT);

}

//udf user defined function
bool loadGLTexture(GLuint* texture, TCHAR resourceID[])
{
	//variable declaration
	bool bResult = false;
	
	//OS bitmap loading.
	HBITMAP hBitmap = NULL;
	BITMAP bmp;

	//code
	//GetModuleHandle - returns the handle in which its running 
	//LR_CREATEDIBSECTION LR - Load as Resource DIB - Device independent bitmap .

	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), resourceID,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);

	if (hBitmap)
	{
		bResult = true;
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		//OpenGL code
		glPixelStorei(GL_UNPACK_ALIGNMENT,4);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D,*texture);

		//setting  texture parameter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//follwing call will actually push the data into graphic memory
		gluBuild2DMipmaps(GL_TEXTURE_2D,3,bmp.bmWidth,bmp.bmHeight,GL_BGR_EXT,GL_UNSIGNED_BYTE,bmp.bmBits);

		DeleteObject(hBitmap);

	}

	return(bResult);

}

void resize(int width, int height)
{
	//code
	if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLfloat)width, (GLfloat)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, ((GLfloat)width/(GLfloat)height),0.1f,300.0f);
}

void display(void)
{
	void Tree(void);
	void SnowMan(void);
	void Mountains(void);
	void Fence(void);
	void Table(void);
	void Bird(GLuint);
	void Chair(void);
	void Humanoid(void);
	void DrawFont(void);
	void DrawFont1(void);

	//code
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//**********************************Camera**********************************
/*	gluLookAt(	xaxis, yaxis, zaxis,
				0.0,0.0,0.0,
				0.0f, 1.0f, 0.0f);
	*/			

	static GLfloat angle1 = 0.0f;
	static GLfloat angle2 = 0.0f;
	static GLfloat angle3 = 900.0f;
	static GLint iMagic = 0;
	static GLuint iCnt = 0;

	if (iMagic == 0)
	{
		gluLookAt(angle1, angle2, angle3,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);
				
		iCnt++;

		if(iCnt < 200)
			DrawFont();
		else
		{			
			PlaySound(TEXT("Snow"), NULL, SND_ASYNC | SND_RESOURCE);
			angle1 = -7.0f;
			angle2 = 2.0f;
			angle3 = 205.0f;
			iMagic = 1;
		}
	}
	else if (iMagic == 1)
	{		
		gluLookAt(angle1, angle2, angle3,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);


		angle1 = angle1 + 0.008;
		angle2 = angle2 + 0.01;
		angle3 = angle3 - 0.1;

		if (angle3 <= 75)
		{
			iMagic = 2;
			angle1 = 30.0f;
			angle2 = 1.0f;
			angle3 = 200.0f;
		}
	}
	else if (iMagic == 2)
	{
			gluLookAt(angle1, angle2, angle3,
				0.0, 0.0, 0.0,
				0.0f, 1.0f, 0.0f);


			angle1 = angle1 - 0.001;
			angle2 = angle2 + 0.008;
			angle3 = angle3 - 0.1;

			if (angle3 <= 85)
			{
				iMagic = 3;
				angle1 = -30.0f;
				angle2 = 1.0f;
				angle3 = 200.0f;
			}


	}
	else if (iMagic == 3)
	{
		gluLookAt(angle1, angle2, angle3,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);

		angle1 = angle1 + 0.001;
		angle2 = angle2 + 0.008;
		angle3 = angle3 - 0.1;

		if (angle3 <= 95)
		{
			iMagic = 4;
			angle1 = 0.0f;
			angle2 = 5.0f;
			angle3 = 200.0f;
		}


	}
	else if (iMagic == 4)
	{

		gluLookAt(angle1, angle2, angle3,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);

		angle1 = angle1 + 0.001;
		angle2 = angle2 + 0.008;
		angle3 = angle3 - 0.1;

/*			glPushMatrix();

				static GLfloat angle = -15.0f;
				glTranslatef(angle, 2.20f, 135.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				Humanoid();

				angle = angle + 0.01f;
				if (angle >= 30)
					angle = -10.0f;

			glPopMatrix();
*/			
			glPushMatrix();
				glTranslatef(2.0f, 0.500f, 125.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, 0.0f);
				Bird(0);

				glTranslatef(-1.0f, 0.00f, 0.0f);
				Bird(0);

				glTranslatef(2.0f, 0.00f, 1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, 0.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, 1.0f);
				Bird(0);

				glTranslatef(-1.0f, 0.00f, -1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, 1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(0);

				glTranslatef(-1.0f, 0.00f, 0.0f);
				Bird(0);

				glTranslatef(-1.50f, 0.00f, -1.0f);
				Bird(0);


			glPopMatrix();



		if (angle3 <= 95)
		{
			iMagic = 5;
			angle1 = 0.0f;
			angle2 = 7.0f;
			angle3 = 60.0f;
		}


	}
	else if (iMagic == 5)
	{

		gluLookAt(angle1, angle2, angle3,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);

		glPushMatrix();

		static GLfloat angle = 35.0f;
		glTranslatef(2.0f, 2.20f, angle);
//		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		Humanoid();

		angle = angle + 0.01f;
		if (angle >= 55)
		{
			iMagic = 6;
			angle1 = -10.0f;
			angle2 = 0.0f;
			angle3 = 200.0f;

		}

		glPopMatrix();

	}
	else if (iMagic == 6)
	{

		gluLookAt(angle1, angle2, angle3,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);

		angle1 = angle1 + 0.001;
		angle2 = angle2 + 0.008;
		angle3 = angle3 - 0.1;

			glPushMatrix();

				static GLfloat angle = -15.0f;
				glTranslatef(angle, 2.20f, 135.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				Humanoid();				
				

			glPopMatrix();
/*
			glPushMatrix();
				glTranslatef(2.0f, 0.500f, 125.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, 0.0f);
				Bird(0);

				glTranslatef(-1.0f, 0.00f, 0.0f);
				Bird(0);

				glTranslatef(2.0f, 0.00f, 1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, 0.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, 1.0f);
				Bird(0);

				glTranslatef(-1.0f, 0.00f, -1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, 1.0f);
				Bird(0);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(0);

				glTranslatef(-1.0f, 0.00f, 0.0f);
				Bird(0);

				glTranslatef(-1.50f, 0.00f, -1.0f);
				Bird(0);


			glPopMatrix();
*/
		
		angle = angle + 0.01f;

		if (angle3 <= 105)
		{
			iMagic = 7;
			angle1 = 10.0f;
			angle2 = 7.0f;
			angle3 = 200.0f;
		}

	}
	else if (iMagic == 7)
	{

		gluLookAt(angle1, angle2, angle3,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);

		angle1 = angle1 - 0.001;
		angle2 = angle2 + 0.008;
		angle3 = angle3 - 0.1;

			glPushMatrix();

				static GLfloat angle = 15.0f;
				glTranslatef(angle, 2.20f, 135.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				Humanoid();				
				

			glPopMatrix();

			glPushMatrix();
				glTranslatef(2.0f, 0.500f, 125.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, 0.0f);
				Bird(1);

				glTranslatef(-1.0f, 0.00f, 0.0f);
				Bird(1);

				glTranslatef(2.0f, 0.00f, 1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, 0.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, 1.0f);
				Bird(1);

				glTranslatef(-1.0f, 0.00f, -1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, 1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(1);

				glTranslatef(-1.0f, 0.00f, 0.0f);
				Bird(1);

				glTranslatef(-1.50f, 0.00f, -1.0f);
				Bird(1);


			glPopMatrix();

		angle = angle - 0.01f;

		if (angle3 <= 105)
		{
			iMagic = 8;
			angle1 = 0.0f;
			angle2 = 7.0f;
			angle3 = 180.0f;
		}

	}


	
	else if (iMagic == 8)
	{

		gluLookAt(angle1, angle2, angle3,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f);

		/*angle1 = angle1 - 0.001;
		angle2 = angle2 + 0.008;
		angle3 = angle3 - 0.1;
		*/
			glPushMatrix();

				static GLfloat angle = 15.0f;
				glTranslatef(angle, 2.20f, 135.0f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				Humanoid();				
				

			glPopMatrix();

			static GLfloat xBird = 0.50;
			static GLfloat yBird = 0.00;

			glPushMatrix();
				glTranslatef(2.0f, 0.500f, 125.0f);
				glTranslatef(xBird, yBird, 0.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, 0.0f);
				Bird(1);

				glTranslatef(-1.0f, 0.00f, 0.0f);
				Bird(1);

				glTranslatef(2.0f, 0.00f, 1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, 0.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, 1.0f);
				Bird(1);

				glTranslatef(-1.0f, 0.00f, -1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, 1.0f);
				Bird(1);

				glTranslatef(0.50f, 0.00f, -1.0f);
				Bird(1);

				glTranslatef(-1.0f, 0.00f, 0.0f);
				Bird(1);

				glTranslatef(-1.50f, 0.00f, -1.0f);
				Bird(1);


			glPopMatrix();

			xBird = xBird - 0.0090f;
			yBird = yBird + 0.0090f;

		angle = angle - 0.01f;

		if (yBird > 15)
		{
			iMagic = 10;
			angle1 = 100.0f;
			angle2 = 0.0f;
			angle3 = 700.0f;
		}

	}
	else if(iMagic == 10)
	{
		/*Black Screen*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		DrawFont1();

	}


	fprintf(gpFILE, "xaxis value : %lf zaxis value : %lf\n",xaxis, zaxis);



		//resetting color
		glColor3f(1.0f, 1.0f, 1.0f);

glPushMatrix();						//Push Matrix

		//*********************************************Cube map******************************************************************************
		glBindTexture(GL_TEXTURE_2D, right);
		glBegin(GL_QUADS);	
			//Right
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(50.00, 20.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(50.00, 20.0f, 200.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(50.0, 0.0f, 200.0f);
			glTexCoord2f(0.0f, 1.0f);		
			glVertex3f(50.0, 0.0f, 0.0f);
		glEnd();

		//left
		glBindTexture(GL_TEXTURE_2D, left);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-50.00, 20.0f, 200.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-50.00, 20.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-50.0, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-50.0, 0.0f, 200.0f);
		glEnd();

		//back
		glBindTexture(GL_TEXTURE_2D, back);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(50.00, 20.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-50.00, 20.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-50.0, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(50.0, 0.0f, 0.0f);
		glEnd();


		//Top			
		glBindTexture(GL_TEXTURE_2D, top);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(50.00, 20.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-50.00, 20.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-50.0, 20.0f, 200.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(50.0, 20.0f, 200.0f);
		glEnd();

		//Bottom			
		glBindTexture(GL_TEXTURE_2D, bottom);
		glBegin(GL_QUADS);			
			glTexCoord2f(4.0f, 4.0f);
			glVertex3f(50.00, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 4.0f);
			glVertex3f(-50.00, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-50.0, 0.0f, 200.0f);
			glTexCoord2f(4.0f, 0.0f);
			glVertex3f(50.0, 0.0f, 200.0f);
		glEnd();
		
		glColor3f(1.0f, 1.0f, 1.0f);



	/////////////////////////////////////
	//			House
	////////////////////////////////////

	//glBindTexture(GL_TEXTURE_2D, bottom);
	glPushMatrix();						//Push Matrix
	
		////////////////////////////////////////
		//				Wall
		////////////////////////////////////////
		glBegin(GL_QUADS);
			//left back wall
			glVertex3f(-1.0f,  8.0, 25.0);
			glVertex3f(-15.0f, 8.0, 25.0);
			glVertex3f(-15.0f, 0.0f, 25.0);
			glVertex3f(-1.0f, 0.0f, 25.0);

			//center top
			glVertex3f(2.5f, 8.0, 25.0);
			glVertex3f(-2.5f, 8.0, 25.0);
			glVertex3f(-2.5f, 1.0, 25.0);
			glVertex3f(2.5f, 1.0, 25.0);


/*			//center bottom
			glVertex3f(1.0f, -2.5, 25.0);
			glVertex3f(-1.0f, -2.5, 25.0);
			glVertex3f(-1.0f, 0.0f, 25.0);
			glVertex3f(1.0f, 0.0f, 25.0);
*/
			//right back wall
			glVertex3f(1.0f, 8.0, 25.0);
			glVertex3f(15.0f, 8.0, 25.0);
			glVertex3f(15.0f, 0.0f, 25.0);
			glVertex3f(1.0f, 0.0f, 25.0);

			//left side wall
			glColor3f(1.0, 0.4, 0.5);
			glVertex3f(-15.0f, 8.0, 70.0);
			glVertex3f(-15.0f, 8.0, 25.0);
			glVertex3f(-15.0f, 0.0f, 25.0);
			glVertex3f(-15.0f, 0.0f, 70.0);


			//right side wall
			//glColor3f(0.8, 0.5, 0.0);
			glVertex3f(15.0f, 8.0, 25.0);
			glVertex3f(15.0f, 8.0, 70.0);
			glVertex3f(15.0f, 0.0f, 70.0);
			glVertex3f(15.0f, 0.0f, 25.0);

			//Roof Top
			glColor3f(0.50, 0.50, 0.50);
			glVertex3f(15.0f, 8.0f, 25.0f);
			glVertex3f(-15.0f, 8.0f, 25.0f);
			glVertex3f(-15.0f, 8.0f, 70.0f);
			glVertex3f(15.0f, 8.0f, 70.0f);

			//Bottom floor
			glColor3f(0.50, 0.50, 0.50);
			glVertex3f(15.0f, 0.01f, 25.0f);
			glVertex3f(-15.0f, 0.01f, 25.0f);
			glVertex3f(-15.0f, 0.01f, 70.0f);
			glVertex3f(15.0f, 0.01f, 70.0f);


			//front
			glVertex3f(15.0f, 8.0f, 70.0f);
			glVertex3f(-15.0f, 8.01f, 70.0f);
			glVertex3f(-15.0f, 0.01f, 70.0f);
			glVertex3f(15.0f, 0.01f, 70.0f);


		glEnd();
		glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();					//Pop Matrix
	






	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//						Fence outside house(function call)
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glPushMatrix();
		glTranslatef(-4.50f, 4.50f, 0.0f);
		Table();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 4.0f, 150.0f);
		Fence();
	
	glTranslatef(20.0f, 0.0f, 0.0f);
		Fence();
	glPopMatrix();

	//Snowman
	glPushMatrix();
		glTranslatef(0.0f, 7.0f, 75.0f);
		glScalef(3.0f, 3.0f, 3.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-18.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

	//2nd Layer
		glTranslatef(0.20f, 0.0f, 5.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

	//3rd layer
		glTranslatef(0.0f, 0.0f, 5.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

	//4th Layer

		glTranslatef(0.0f, 0.0f, 5.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(6.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

	//5th Layer 

		glTranslatef(0.0f, 0.0f, 5.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(-3.0f, 0.0f, 0.0f);
		Tree();

	//6th Layer

		glTranslatef(0.0f, 0.0f, 5.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

		glTranslatef(3.0f, 0.0f, 0.0f);
		Tree();

	glPopMatrix();
	
	//resetting color
	glColor3f(1.0f, 1.0f, 1.0f);

/*
	glPushMatrix();
		glTranslatef(2.0f, -2.50f, 45.0f);
		Chair();
	glEnd();
*/
	
	//******************************Table on the right side of the door.******************************
	glPushMatrix();
		glRotatef(90.0f, 0.0, 0.10, 0.0);
		glTranslatef(-30.0f, 4.50f, -41.0f);
		//glTranslatef(25.0f, 0.0f, -1.0f);
		Table();
	glPopMatrix();
	//******************************end of table on right side******************************


	//******************************Computer******************************
	glPushMatrix();
		glTranslatef(8.0f, 4.00f, 31.50f);
		//glColor3f(0.50, 0.0, 0.50);
		//iMac background grey
		glBindTexture(GL_TEXTURE_2D,grey);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(2.15, 1.25, 0.0);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-2.15, 1.25, 0.0);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-2.15, -1.3, 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(2.15, -1.3, 0.0);
		glEnd();

		//iMac stand
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(0.1, -0.35, 0.0);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-0.1, -0.35, 0.0);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-0.25, -1.45, 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.25, -1.45, 0.0);
		glEnd();

		//Front side of stand
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.25, -1.45, 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-0.25, -1.45, 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-0.35, -1.45, 0.5);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.35, -1.45, 0.5);

		glEnd();

		//Keyboard
		glBindTexture(GL_TEXTURE_2D,keyboard);
		glBegin(GL_QUADS);
			//top side
			glTexCoord2f(1.0, 1.0);
			glVertex3f(0.9, -1.45, 0.8);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-0.9, -1.45, 0.8);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-0.9, -1.45, 1.5);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(0.9, -1.45, 1.5);
		glEnd();



		//iMac screen
		glBindTexture(GL_TEXTURE_2D,apple_screen);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,1.0);
			glVertex3f(2.13,1.23,0.05);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-2.13, 1.23, 0.05);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-2.13, -0.95, 0.05);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(2.13, -0.95, 0.05);
		glEnd();

	glPopMatrix();

	//******************************end of computer******************************
	//*************************************** Chair *********************************************

		//computer table chair
		glPushMatrix();
			glTranslatef(8.0f, 3.50f, 35.0f);
			glRotatef(180.0, 0.0, 1.0, 0.0);
			glScalef(1.50f, 1.50, 1.50);
			Chair();
		glPopMatrix();

	////////////////////////////////////////////////////////////////// End Of Computer Table //////////////////////////////////////////////////////////////////
		


		//resetting color
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, 2.50f, 150.0f);
		glScalef(2.0f, 2.0f, 2.0f);
		SnowMan();
	
		
	
glPopMatrix();					//Pop Matrix	
	
	SwapBuffers(ghdc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													X'Mas Tree																			
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Tree()
{

glPushMatrix();
	glPushMatrix();
	//Branch
	glBindTexture(GL_TEXTURE_2D, bark_texture);
	//glColor3f(0.70f, 0.45, 0.25);
	glPushMatrix();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.2, 0.2, 2.5f, 20, 20);
	glPopMatrix();


	//TREE

	//glColor3f(0.0f, 1.0, 0.0f);
	glBindTexture(GL_TEXTURE_2D, grass_texture);
	//1st Cone
	glPushMatrix();
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.6, 0.01, 1.0f, 20, 20);
	glPopMatrix();

	//glColor3f(0.0f, 1.0, 1.0f);
	//2nd Cone
	glTranslatef(0.0f, -0.40f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.8, 0.05, 1.0f, 20, 20);
	glPopMatrix();


	//3rd Cone
	//glColor3f(1.0f, 1.0, 0.0f);
	glTranslatef(0.0f, -0.40f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 1.0, 0.05, 1.0f, 20, 20);
	glPopMatrix();

	//4th Cone
	//glColor3f(0.50f, 1.0, 0.50f);
	glTranslatef(0.0f, -0.40f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 1.2, 0.05, 1.0f, 30, 30);
	glPopMatrix();

	//5th Cone
	//glColor3f(0.0f, .50, 0.50f);
	glTranslatef(0.0f, -0.40f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 1.3, 0.05, 1.0f, 30, 30);
	glPopMatrix();

glPopMatrix();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//															Snow Man												
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SnowMan()
{
	static GLfloat angle = 0.0f;

	glRotatef(angle, 0.0f, 1.0f, 0.0f);


	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, snow_texture);
		quadric = gluNewQuadric();
		gluQuadricTexture(quadric, GL_TRUE);
		glRotatef(45, 1.0f, 0.0f, 0.0f);
		gluSphere(quadric, 0.3, 30, 30);
	glPopMatrix();

	glTranslatef(0.0f,-0.7,0.0);
	glPushMatrix();		
		glBindTexture(GL_TEXTURE_2D, snow_texture);
		quadric = gluNewQuadric();
		gluQuadricTexture(quadric, GL_TRUE);
		glRotatef(45, 1.0f, 0.0f, 0.0f);
		gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//hands	
		glTranslatef(0.40f, -0.30f, 0.0f);
		glLineWidth(6.0f);
	glPushMatrix();
		glBegin(GL_LINES);

		glColor3f(0.60f, 0.30f, 0.15f);
			//Right hand stick for snow man
			glVertex3f(0.0f, 0.5, 0.0f);
			glVertex3f(0.5f,0.78f,0.0f);		

			glVertex3f(0.38f, 0.72f, 0.0f);
			glVertex3f(0.45f, 0.90, 0.0f);

			glVertex3f(0.36f, 0.72f, 0.0f);
			glVertex3f(0.45f, 0.65, 0.0f);

			//Left hand stick for snow man
			glVertex3f(-0.80f, 0.5, 0.0f);
			glVertex3f(-1.3f, 0.78f, 0.0f);

			glVertex3f(-1.10f, 0.68f, 0.0f);
			glVertex3f(-1.20f, 0.80, 0.0f);


		glEnd();
	glPopMatrix();
	


	//nose	
	glPushMatrix();
		glTranslatef(-0.4f, 1.0f, 0.20f);
		glColor3f(1.0f,0.50f,0.0f);
		quadric = gluNewQuadric();
		glRotatef(90.0f, 1.25f, -0.5f, -0.30f);
		glRotatef(90.0f, -1.f, 0.0f, 0.0f);
		gluCylinder(quadric, 0.06f, 0.0f, 0.40f, 30, 30);	
	glPopMatrix();

	//hat
	glPushMatrix();
		glTranslatef(-0.4f, 1.65f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(0.50f, 0.50f, 0.50f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.23, 0.23, 0.4, 30, 30);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.4f, 1.30f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(0.50f, 0.50f, 0.50f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.40, 0.40, 0.1, 30, 30);
	glPopMatrix();

	//Red Ribbion
	glPushMatrix();
		glTranslatef(-0.4f, 1.50f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(0.80f, 0.0f, 0.0f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.23, 0.23, 0.1, 30, 30);
	glPopMatrix();

	//muffler
	glPushMatrix();
		glTranslatef(-0.4f, 0.80f, 0.0f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(0.80f, 0.0f, 0.0f);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.25, 0.35, 0.1, 30, 30);
	glPopMatrix();

	//fold of muffler
	glPushMatrix();
		glTranslatef(-0.50f, 0.80f, 0.0f);
		glColor3f(0.80f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glVertex3f(0.10f, 0.00, 0.20f);
			glVertex3f(-0.10f, 0.00, 0.20f);
			glVertex3f(-0.10f, -0.30, 0.55f);
			glVertex3f(0.10f, -0.30, 0.55f);
		glEnd();
	glPopMatrix();

	//eyes left
	glPushMatrix();
		glTranslatef(-0.50f, 1.10f, 0.20f);
		glColor3f(0.0f, 0.0f, 0.0f);
		quadric = gluNewQuadric();
		gluSphere(quadric,0.06,20,20);
	glPopMatrix();

	//eyes right
	glPushMatrix();
	glTranslatef(-0.30f, 1.10f, 0.20f);
	glColor3f(0.0f, 0.0f, 0.0f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.06, 20, 20);
	glPopMatrix();


	glColor3f(1.0f, 1.0f, 1.0f);

	angle = angle + 0.2f;
	if (angle >= 360)
	{
		angle = 0.0f;
	}


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													Mountain																			
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Mountains()
{
	glBindTexture(GL_TEXTURE_2D, bottom);
	glPushMatrix();	
		glBegin(GL_TRIANGLES);
			glTexCoord2f(0.5f,1.0f);
			glVertex3f(0.0f, 2.50f, 0.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-5.0f, -2.50f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(5.0f, -2.50f, 0.0f);
		glEnd();

	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													Wooden Fence 
//		Description : wooden fence outside house.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Fence(void)
{
	//Wooden fence
	

	glPushMatrix();
		//left side fence
		glBindTexture(GL_TEXTURE_2D, fence_texture);

		//horizontal fences
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.6f, -2.0f, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-16.0f, -2.0f, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-16.0f, -2.4f, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.6f, -2.4f, 9.0);


			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-16.0f, -3.2f, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.6f, -3.2f, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.6f, -3.6f, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-16.0f, -3.6f, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-2.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-2.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-2.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-2.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-3.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-3.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-3.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-3.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-4.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-4.5, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-4.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-4.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-5.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-5.5, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-5.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-5.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-6.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-6.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-6.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-6.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-7.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-7.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-7.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-7.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-8.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-8.5, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-8.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-8.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-9.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-9.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-9.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-9.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-10.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-10.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-10.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-10.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-11.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-11.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-11.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-11.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-12.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-12.5, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-12.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-12.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-13.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-13.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-13.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-13.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-14.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-14.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-14.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-14.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-15.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-15.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-15.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-15.0, -4.0, 9.0);
		glEnd();

		//right side fence
		glBindTexture(GL_TEXTURE_2D, fence_texture);
		//horizontal fences
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f,1.0f);
			glVertex3f(16.0f,-2.0f,9.0);
			glTexCoord2f(1.0f,0.0f);
			glVertex3f(1.6f, -2.0f, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.6f, -2.4f, 9.0);
			glTexCoord2f(0.0f,1.0f);
			glVertex3f(16.0f, -2.4f, 9.0);


			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(16.0f, -3.2f, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.6f, -3.2f, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.6f, -3.6f, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(16.0f, -3.6f, 9.0);
		glEnd();

		//standing / vertical fences
		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(2.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(2.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(2.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(2.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(3.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(3.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(3.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(3.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(4.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(4.5, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(4.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(4.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(5.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(5.5, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(5.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(5.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(6.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(6.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(6.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(6.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(7.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(7.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(7.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(7.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(8.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(8.5, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(8.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(8.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(9.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(9.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(9.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(9.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(10.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(10.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(10.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(10.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(11.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(11.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(11.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(11.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(12.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(12.5, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(12.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(12.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(13.0, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(13.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(13.5, -4.0, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(13.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(14.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(14.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(14.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(14.0, -4.0, 9.0);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(15.0, -1.5, 9.0);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(15.5, -1.5, 9.0);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(15.5, -4.0, 9.0);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(15.0, -4.0, 9.0);
		glEnd();

	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//											TABLE
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Table()
{
	glPushMatrix();					//Push Matrix		
	glTranslatef(-1.0f, -2.0f, -5.0f);
	glBindTexture(GL_TEXTURE_2D, fence_texture);
	glColor3f(1.0f, 1.0f, 1.0);
	glBegin(GL_QUADS);
	//front of table
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 0.0f, 60.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 60.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, -0.30f, 60.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(2.0f, -0.30f, 60.0f);

	//right side of table
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 0.0f, 50.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 60.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.0f, -0.30f, 60.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(2.0f, -0.30f, 50.0f);

	//back side of table
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.0f, 0.0f, 50.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 50.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(2.0f, -0.30f, 50.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, -0.30f, 50.0f);

	//left side of table
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-2.0f, 0.0f, 50.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 60.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, -0.30f, 60.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, -0.30f, 50.0f);

	//Top of table
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(2.0f, 0.0f, 50.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-2.0f, 0.0f, 50.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-2.0f, 0.0f, 60.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(2.0f, 0.0f, 60.0f);
	glEnd();

	//Legs of the table
	glPushMatrix();
	//1st Leg
	glTranslatef(1.5, 0.0, 52.0f);
	glRotatef(90, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.2, 0.2, 3, 20, 20);

	//2nd leg
	glTranslatef(-3.0, 0.0, 0.0f);
	//glRotatef(90, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.2, 0.2, 3, 20, 20);

	glPopMatrix();

	glPushMatrix();
	//3rd Leg
	glTranslatef(1.5, 0.0, 58.0f);
	glRotatef(90, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.2, 0.2, 3, 20, 20);

	//4th leg
	glTranslatef(-3.0, 0.0, 0.0f);
	//glRotatef(90, 1.0, 0.0, 0.0);
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric, 0.2, 0.2, 3, 20, 20);

	glPopMatrix();

	glPopMatrix();					//Pop Matrix



}

void Bird(GLuint Flap)
{
	
	//variables
	static GLfloat angle1 = 2.0f;
	static GLfloat angle2 = -0.50f;
	static GLfloat yaxis = 45;

	static GLfloat angleB = 0.0f;
	static GLint flag = 1;

	GLUquadric* quadric[10];

	//glTranslatef(angle1, angle2, -3.0f);

	//

	//glRotatef(yaxis, 0.0, 1.0, 0.0);
	//Sphere

	//head
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, bird);
		quadric[0] = gluNewQuadric();
		gluQuadricTexture(quadric[0], GL_TRUE);
		gluSphere(quadric[0], 0.1, 30, 30);	//normals are calcaulated by glu libraries(gluSphere itself)

		//body
		glPushMatrix();
			glTranslatef(0.20, -0.12, 0.0);
			glRotatef(-30, 0.0, 0.0, 1.0);
			glScalef(0.70, 0.35, 0.5);
			quadric[1] = gluNewQuadric();
			gluQuadricTexture(quadric[1], GL_TRUE);
			gluSphere(quadric[1], 0.3, 30, 30);	//normals are calcaulated by glu libraries(gluSphere itself)
		
			//Tail
			glBegin(GL_QUADS);
				//glColor3f(1.0, 1.0, 0.0);
				glTexCoord2f(1.0f, 0.0);
				glVertex3f(0.50, 0.10, -0.15);
				glTexCoord2f(0.0f, 0.0);
				glVertex3f(0.0, 0.10, -0.10);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(0.0, 0.10, 0.10);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(0.50, 0.10, 0.15);
			glEnd();
			glColor3f(0.70, 0.80, 0.0);
		glPopMatrix();

		glPushMatrix();
			//eyes
			glColor3f(0.0, 0.0, 0.0);
			glTranslatef(-0.08, 0.05, -0.03);
			quadric[2] = gluNewQuadric();
			gluQuadricTexture(quadric[2], GL_TRUE);
			gluSphere(quadric[2], 0.01, 30, 30);	//normals are calcaulated by glu libraries(gluSphere itself)

			glTranslatef(0.01, 0.0, 0.08);
			quadric[3] = gluNewQuadric();
			gluQuadricTexture(quadric[3], GL_TRUE);
			gluSphere(quadric[3], 0.01, 30, 30);	//normals are calcaulated by glu libraries(gluSphere itself)
		glPopMatrix();
	
		//beak
		glPushMatrix();
			glColor3f(1.0, 1.0, 0.0);
			glTranslatef(-0.15, -0.0, 0.01);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glRotatef(90.0, 0.0, 1.0, 0.0);
			quadric[4] = gluNewQuadric();
			gluQuadricTexture(quadric[4], GL_TRUE);
			gluCylinder(quadric[4], 0.001, 0.02, 0.05, 10, 10);
		glPopMatrix();
		glColor3f(0.70, 0.80, 0.0);

		//wings
		//right wing
		glPushMatrix();
		//glColor3f(1.0, 0.0, 1.0);
		if(Flap == 1)
			glRotatef(angleB, 1.0f, 0.0f, 0.0f);
		else
			glRotatef(0, 1.0f, 0.0f, 0.0f);
	
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0);
				glVertex3f(0.35f, -0.15f, 0.0f);
				glTexCoord2f(0.0f, 0.0);
				glVertex3f(0.10f, -0.15f, 0.0f);
				glTexCoord2f(0.0f, 1.0);
				glVertex3f(0.10f, -0.15, 0.35f);
				glTexCoord2f(1.0f, 1.0);
				glVertex3f(0.35f, -0.15, 0.35f);
			glEnd();
			fprintf(gpFILE, "angle of right wing %f\n", angleB);
			if (flag == 1)
			{
				if (angleB >= -31.0)
					angleB = angleB - 0.2;
				else
					flag = 2;
			}
			else if (flag == 2)
			{
				angleB = angleB + 0.2;
				if (angleB >= 23.0)
					flag = 1;

			}
		glPopMatrix();
		
		//left wing
		glPushMatrix();
		//glColor3f(1.0, 0.0, 1.0);
		if (Flap == 1)
			glRotatef(angleB, -1.0f, 0.0f, 0.0f);
		else
			glRotatef(0, 1.0f, 0.0f, 0.0f);

			glBegin(GL_QUADS);
				glVertex3f(0.35f, -0.15f, 0.0f);
				glVertex3f(0.10f, -0.15f, 0.0f);
				glVertex3f(0.10f, -0.15, -0.35f);
				glVertex3f(0.35f, -0.15, -0.35f);
			glEnd();

			fprintf(gpFILE, "angle of left wing %f\n", angleB);
		glPopMatrix();

		glColor3f(0.70, 0.80, 0.0);

	glPopMatrix();
	
	angle1 = angle1 - 0.5f;
	angle2 = angle2 + 0.2f;


}

void Chair()
{
	//function prototype
	void Chair_Stick(void);

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, fence_texture);
		//glTranslatef(2.0f, 0.0f, 38.0f);
		//glRotatef(90.0f, 0.0f, 1.0, 0.0);

		//chair back rest
		glPushMatrix();
		//glTranslatef(0.0f, 0.0f, 0.0f);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(0.40f, 0.0, -0.60);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-0.40f, 0.0, -0.60);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-0.40f, -0.50, -0.60);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(0.40f, -0.50, -0.60);
			glEnd();
		glPopMatrix();


		//seat or base
		glPushMatrix();
			//seat or base
			//top
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0.50, -1.38, -0.60);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-0.50, -1.38, -0.60);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-0.50, -1.38, 0.90);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.50, -1.38, 0.90);
			glEnd();

			//front
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0.50, -1.38, 0.90);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-0.50, -1.38, 0.90);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-0.50, -1.43, 0.90);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.50, -1.43, 0.90);
			glEnd();

			//right
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0.50, -1.38, -0.60);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(0.50, -1.38, 0.90);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(0.50, -1.43, 0.90);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.50, -1.43, -0.60);
			glEnd();

			//back
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0.50, -1.38, -0.60);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-0.50, -1.38, -0.60);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-0.50, -1.43, -0.60);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.50, -1.43, -0.60);
			glEnd();

			//left
			glBegin(GL_QUADS);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-0.50, -1.38, 0.90);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-0.50, -1.38, -0.60);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(-0.50, -1.43, -0.60);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(-0.50, -1.43, 0.90);
			glEnd();


			//bottom
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0.50, -1.43, -0.60);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(-0.50, -1.43, -0.60);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(-0.50, -1.43, 0.90);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0.50, -1.43, 0.90);
			glEnd();

		glPopMatrix();//end of seat or base

		//right stick
		glTranslatef(0.40, 0.0, 0.0);
		Chair_Stick();

		//left stick
		glTranslatef(-0.90, 0.0, 0.0);
		Chair_Stick();

		//left bottom stick
		glTranslatef(0.0, -1.42, 0.0);
		Chair_Stick();
		
		//right bottom stick
		glTranslatef(0.90, 0.0, 0.0);
		Chair_Stick();

		//right front stick
		glTranslatef(0.0, 0.0, 1.40);
		Chair_Stick();

		//left front stick
		glTranslatef(-0.90, 0.0, .0);
		Chair_Stick();


	glPopMatrix();

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//						Chair Stick
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Chair_Stick()
{

	//side stick support
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.10, 0.00, -0.60);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.00, 0.00, -0.60);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.00, 0.00, -0.50);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.10, 0.00, -0.50);
	glEnd();

	//front
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.10, 0.00, -0.50);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.00, 0.00, -0.50);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.00, -1.38, -0.50);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.10, -1.38, -0.50);
	glEnd();

	//right
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.10, 0.00, -0.60);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.00, 0.00, -0.50);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.00, -1.38, -0.50);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.10, -1.38, -0.60);
	glEnd();

	//back
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.10, 0.00, -0.60);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.00, 0.00, -0.60);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.00, -1.38, -0.60);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.10, -1.38, -0.60);
	glEnd();

	//left
	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.10, 0.00, -0.50);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.00, 0.00, -0.60);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.00, -1.38, -0.60);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.10, -1.38, -0.50);
	glEnd();


	//bottom
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.10, -1.38, -0.60);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.00, -1.38, -0.60);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.00, -1.38, -0.50);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.10, -1.38, -0.50);
	glEnd();

	glPopMatrix();



}

void Humanoid(void)
{

	//Local variables
	static GLfloat angle = 0.0f;
	static GLfloat xaxis = -3.50f;
	static GLfloat yaxis = 0.0f;

	static GLfloat leg = 0.0f;
	static GLfloat hand = 0.0f;
	static GLfloat elbow = 0.0f;
	static GLfloat left_knee = 0.0f;
	static GLfloat right_knee = 0.0f;
	static GLfloat right_leg = 0.0f;

	static GLint flag = 1;
	//glTranslatef(0.0f, -0.28f, -7.0f);
	//glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

	//glPushMatrix();
		/////////////////////////////////////////////////////Humanoid//////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////
		//
		//											Head
		//
		///////////////////////////////////////////////////////////////////////////////////////////////

	glTranslatef(0.0f, 1.00f, 0.0f);
	glPushMatrix();
	glScalef(0.75f, 1.0f, 0.75f);
	glColor3f(1.0f, 0.50f, 0.30f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.3, 30, 30);
	glPopMatrix();

	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	//											Neck
	//
	///////////////////////////////////////////////////////////////////////////////////////////////
	glTranslatef(0.0f, -0.28f, 0.0f);
	glPushMatrix();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.50f, 0.30f);
	quadric = gluNewQuadric();
	gluCylinder(quadric, 0.08, 0.08, 0.10, 20, 20);
	glPopMatrix();
	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	//											Torso
	//
	///////////////////////////////////////////////////////////////////////////////////////////////
	glTranslatef(0.0f, -0.62, 0.0f);
	glPushMatrix();
	glBegin(GL_QUADS);
	//front
	glColor3f(0.70f, 0.00f, 0.00f);
	glVertex3f(0.40f, 0.50f, 0.20f);
	glVertex3f(-0.40f, 0.50f, 0.20f);
	glVertex3f(-0.40f, -0.70f, 0.20f);
	glVertex3f(0.40f, -0.70f, 0.20f);

	//Right
	glVertex3f(0.40f, 0.50f, -0.20f);
	glVertex3f(0.40f, 0.50f, 0.20f);
	glVertex3f(0.40f, -0.70f, 0.20f);
	glVertex3f(0.40f, -0.70f, -0.20f);

	//Back
	glVertex3f(-0.40f, 0.50f, -0.20f);
	glVertex3f(0.40f, 0.50f, -0.20f);
	glVertex3f(0.40f, -0.70f, -0.20f);
	glVertex3f(-0.40f, -0.70f, -0.20f);

	//Left
	glVertex3f(-0.40f, 0.50f, 0.20f);
	glVertex3f(-0.40f, 0.50f, -0.20f);
	glVertex3f(-0.40f, -0.70f, -0.20f);
	glVertex3f(-0.40f, -0.70f, 0.20f);

	//Top
	glVertex3f(0.40f, 0.50f, -0.20f);
	glVertex3f(-0.40f, 0.50f, -0.20f);
	glVertex3f(-0.40f, 0.50f, 0.20f);
	glVertex3f(0.40f, 0.50f, 0.20f);

	//Bottom
	glVertex3f(0.40f, -0.50f, -0.20f);
	glVertex3f(-0.40f, -0.50f, -0.20f);
	glVertex3f(-0.40f, -0.70f, 0.20f);
	glVertex3f(0.40f, -0.70f, 0.20f);
	glEnd();
	glPopMatrix();
	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	//										Arms
	//		(Here the matrix is pushed on torso level Translatef and changes are made and 
	//				poped at torso level so the right hand is designed again from torso level).
	//
	///////////////////////////////////////////////////////////////////////////////////////////////

	//Left hand
	//Biceps
	glPushMatrix();
	glRotatef(hand, 1.0f, 0.0f, 0.0f);
	glTranslatef(-0.55f, 0.20f, 0.0f);
	glPushMatrix();
	glColor3f(0.70f, 0.00f, 0.00f);
	glScalef(0.28f, 0.60f, 0.28f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//forearms
	glRotatef((GLfloat)elbow, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.00f, -0.50f, 0.0f);
	glPushMatrix();
	glColor3f(1.0f, 0.50f, 0.30f);;
	glScalef(0.22f, 0.75f, 0.25f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//clothes
	glPushMatrix();
	glTranslatef(0.0f, 0.75, 0.0f);
	glColor3f(0.70f, 0.00f, 0.00f);
	quadric = gluNewQuadric();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, 0.15, 0.15, 0.50, 30, 30);
	glRotatef(90, -1.0, 0.0, 0.0);
	glPopMatrix();

	glPopMatrix();

	//Right Hand
	//Biceps
	glPushMatrix();
	glRotatef(hand, -1.0f, 0.0f, 0.0f);
	glTranslatef(0.55f, 0.20f, 0.0f);
	glPushMatrix();
	glColor3f(0.70f, 0.00f, 0.00f);
	glScalef(0.28f, 0.60f, 0.28f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//forearms
	glRotatef((GLfloat)elbow, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.00f, -0.50f, 0.0f);
	glPushMatrix();
	glColor3f(1.0f, 0.50f, 0.30f);
	glScalef(0.22f, 0.75f, 0.25f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();


	//clothes
	glPushMatrix();
	glTranslatef(0.0f, 0.75, 0.0f);
	glColor3f(0.70f, 0.00f, 0.00f);
	quadric = gluNewQuadric();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, 0.15, 0.15, 0.50, 30, 30);
	glRotatef(90, -1.0, 0.0, 0.0);
	glPopMatrix();

	glPopMatrix();

	///////////////////////////////////////////////////////////////////////////////////////////////
	//
	//								Legs (Lower Part)
	//
	///////////////////////////////////////////////////////////////////////////////////////////////

		//Left Leg
	glPushMatrix();
	glRotatef(leg, -1.0f, 0.0f, 0.0f);
	glTranslatef(-0.25f, -1.08, 0.0f);
	glPushMatrix();
	glScalef(0.35f, 0.80f, 0.35f);
	quadric = gluNewQuadric();
	/*				glRotatef(90, 1.0, 0.0, 0.0);
					gluCylinder(quadric, 0.6, 0.6, 0.70, 30, 30);
					glRotatef(90, -1.0, 0.0, 0.0);*/
	glColor3f(1.0f, 0.50f, 0.30f);
	gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//below Knee
	glRotatef((GLfloat)left_knee, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -0.70, 0.0f);
	glPushMatrix();
	glScalef(0.28f, 0.80f, 0.28f);
	glColor3f(1.0f, 0.50f, 0.30f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//Clothes
	glPushMatrix();
	glTranslatef(0.0f, 1.08, 0.0f);
	glColor3f(1.0f, 0.70f, 0.10f);
	quadric = gluNewQuadric();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, 0.20, 0.21, 0.50, 30, 30);
	glRotatef(90, -1.0, 0.0, 0.0);
	glPopMatrix();

	glPopMatrix();

	//Right Leg
	glPushMatrix();
	glRotatef(leg, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.25f, -1.08, 0.0f);
	glPushMatrix();
	glScalef(0.35f, 0.80f, 0.35f);
	glColor3f(1.0f, 0.50f, 0.30f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//below Knee
	glRotatef((GLfloat)right_knee, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -0.70, 0.0f);
	glPushMatrix();
	glScalef(0.28f, 0.80f, 0.28f);
	glColor3f(1.0f, 0.50f, 0.30f);
	quadric = gluNewQuadric();
	gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//Clothes
	glPushMatrix();
	glTranslatef(0.0f, 1.08, 0.0f);
	glColor3f(1.0f, 0.70f, 0.10f);
	quadric = gluNewQuadric();
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(quadric, 0.20, 0.21, 0.50, 30, 30);
	glRotatef(90, -1.0, 0.0, 0.0);
	glPopMatrix();

	glPopMatrix();

	//glPopMatrix();//end 




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	angle = angle + 0.02f;

	xaxis = xaxis + 0.0001f;

	//animation for leg
	if (flag == 1)
	{
		if (leg >= -13.0)
		{
			leg = leg - 0.3f;
			hand = hand - 0.6f;
		}
		else
		{
			flag = 2;
		}
	}
	else if (flag == 2)
	{
		if (leg < 13.0)
		{
			leg = leg + 0.3f;
			hand = hand + 0.6f;
		}
		else
			flag = 1;
	}



}


void DrawFont(void)
{
	//code
	glLoadIdentity();

	glTranslatef(-2.00f, 0.0f, -910.0f);
	//	glScalef(2.5f, 2.5f, 2.5f);
	glColor3f(0.50, 0.50, 0.50);

	glRasterPos3f(0.0f, 1.0f, 900.0);
	glListBase(uiFontList);
	glCallLists(15, GL_UNSIGNED_BYTE, "ASTROMEDICOMP'S");

	glRasterPos3f(0.05f, 0.38f, 900.0f);
	glCallLists(14, GL_UNSIGNED_BYTE, "FRAGMENT GROUP");

	glRasterPos3f(0.69f, -0.10f, 900.0f);
	glCallLists(8, GL_UNSIGNED_BYTE, "PRESENTS");

	glColor3f(0.70, 0.50, 0.30);
	glRasterPos3f(0.70f, -1.50f, 900.0);
	glCallLists(10, GL_UNSIGNED_BYTE, "Snow World");
}


void DrawFont1()
{


	static GLuint iCnt = 0;

	glLoadIdentity();

	glTranslatef(-3.00f, 0.0f, -910.0f);

	if (iCnt > 100 && iCnt < 250)
	{
		glRasterPos3f(0.65f, 2.0f, 900.0);
		glListBase(uiFontList);
		glCallLists(15, GL_UNSIGNED_BYTE, "Technology Used");

		glRasterPos3f(-1.80f, 1.20f, 900.0);
		glCallLists(43, GL_UNSIGNED_BYTE, "Win32 SDK , OpenGL Fixed Function Pipeline.");

		glRasterPos3f(-0.80f, 0.70f, 900.0);
		glCallLists(30, GL_UNSIGNED_BYTE, "Programming Language : C , C++");

		glTranslatef(0.30f, -1.0f, 0.0f);

		glRasterPos3f(0.75f, 0.50f, 900.0);
		glCallLists(8, GL_UNSIGNED_BYTE, "ABSTRACT");

		glRasterPos3f(-2.50f, 0.0f, 900.0);
		glCallLists(50, GL_UNSIGNED_BYTE, "This demo was developed on only basic concepts , ");


		glRasterPos3f(-2.30f, -0.50f, 900.0);
		glCallLists(50, GL_UNSIGNED_BYTE, "taught in class, by using lines , triangle , quads");

		glRasterPos3f(-1.80f, -1.0f, 900.0);
		glCallLists(39, GL_UNSIGNED_BYTE, "and GLU function for Sphere , Cylinder.");

		glRasterPos3f(-1.0f, -1.50f, 900.0);
		glCallLists(35, GL_UNSIGNED_BYTE, "Music: Hans Zimmer - Lost but Won.");

	}

	if (iCnt > 250 && iCnt < 400)
	{
		glRasterPos3f(1.35f, 1.50f, 900.0);
		glListBase(uiFontList);
		glCallLists(7, GL_UNSIGNED_BYTE, "CREDITS");

		glRasterPos3f(-2.30f, 0.60f, 900.0);
		glCallLists(50, GL_UNSIGNED_BYTE, "Special thanks to My Family , Team AstroMediComp ,");

		glRasterPos3f(-2.20f, 0.0f, 900.0);
		glCallLists(47, GL_UNSIGNED_BYTE, "My Group Leader - Ajay Ambure & Fragment Group.");
	}

	if (iCnt > 400 && iCnt < 550)
	{
		glRasterPos3f(-1.20f, 1.00f, 900.0);
		glListBase(uiFontList);
		glCallLists(33, GL_UNSIGNED_BYTE, "Developed By : Rohan Satyamurthy");

		glRasterPos3f(-2.20f, 0.00f, 900.0);
		glCallLists(47, GL_UNSIGNED_BYTE, "Under Guidance of My Guru Dr Vijay Gokhale Sir");

		glRasterPos3f(-2.40f, -0.60f, 900.0);
		glCallLists(52, GL_UNSIGNED_BYTE, "Without whom this wouldnt have been possible...!!!!");
	}


	iCnt++;

	if (iCnt > 550)
		iCnt = 400;


}


void uninitialize(void)
{
	if (gbFullscreen = true)
	{
		dwStyle = GetWindowLong(ghwnd,GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, (dwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc)
	{
		ReleaseDC(ghwnd,ghdc);
		ghdc = NULL;
	}

	glDeleteTextures(1,&grass_texture);
	glDeleteTextures(1, &bark_texture);

	if (gpFILE)
	{
		fprintf(gpFILE, "Program executed successfully Date : %d-%d-%d Time:%d:%d:%d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
		fclose(gpFILE);
		gpFILE = NULL;
	}

}
