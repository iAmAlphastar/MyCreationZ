
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name : ROHAN SATYAMURTHY
//	Program : Nature Demo(Resort)
//	Date : 12 December 2020
//	Modified : 19 February 2021
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
/*GLfloat zaxis = 500.0f;
GLfloat yaxis = 0.0f;
GLfloat xaxis = -57.0f;
*/


GLfloat zaxis ;
GLfloat yaxis ;
GLfloat xaxis ;





//texture
GLuint grass_texture;
GLuint bark_texture;
GLuint snow_texture;
GLuint mountain_texture;
GLuint fence_texture;
GLuint apple_screen;
GLuint grey;
GLuint keyboard;
GLuint tatha;
GLuint lpi;
GLuint lpi_side;
GLuint pages;
GLuint bach;
GLuint bach_side;
GLuint gold;
GLuint marble;
GLuint brick;
GLuint wall;
GLuint path;
GLuint groove_land;
GLuint black_plastic;
GLuint water;
GLuint sidewall;
GLuint winewall;
GLuint bluewall;
GLuint tiles;
GLuint celing;
GLuint greywall;
GLuint bedsheet;
GLuint bedspread;
GLuint newspaper;
GLuint psalms;
GLuint limit;
GLuint family;
GLuint redbull;
GLuint redwhite;
GLuint bluewhite;


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
		TEXT("OpenGL Demo : Resort"),
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
		MessageBox(hwnd, TEXT("\tWelcome to the Fantasy Resort Demo !!!!\nPress 'F' or 'f' for fullscreen\nPress esc or Spacebar or 'X' or 'x' for EXIT.\n\nPress any key to start.\n\nEnjoy the Demo !!\n\n"), TEXT("Help"), MB_OK);
		//PlaySound(TEXT("Tropics"), NULL, SND_ASYNC | SND_RESOURCE);
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
		MessageBox(hwnd, TEXT("Developed By : Rohan Satyamurthy.\n\n**under the guidance of my Guru Dr. Vijay Gokhale Sir.**\n\n\n\nTechnology :\nWin32 SDK.\nOpenGL Fixed Function Pipeline.\nProgramming language used 'C'.\n\n"), TEXT("Info"), MB_OK);
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
	loadGLTexture(&tatha, MAKEINTRESOURCE(TATHA));
	loadGLTexture(&lpi, MAKEINTRESOURCE(LPI));
	loadGLTexture(&lpi_side, MAKEINTRESOURCE(LPI_SIDE));
	loadGLTexture(&pages, MAKEINTRESOURCE(PAGES));
	loadGLTexture(&bach, MAKEINTRESOURCE(BACH));
	loadGLTexture(&bach_side, MAKEINTRESOURCE(BACH_SIDE));
	loadGLTexture(&gold, MAKEINTRESOURCE(GOLD));
	loadGLTexture(&marble, MAKEINTRESOURCE(MARBLE));
	loadGLTexture(&brick, MAKEINTRESOURCE(BRICK));
	loadGLTexture(&wall, MAKEINTRESOURCE(WALL));
	loadGLTexture(&path, MAKEINTRESOURCE(PATH));
	loadGLTexture(&groove_land, MAKEINTRESOURCE(GROOVE_LAND));
	loadGLTexture(&black_plastic, MAKEINTRESOURCE(BLACK_PLASTIC));
	loadGLTexture(&water, MAKEINTRESOURCE(WATER));
	loadGLTexture(&sidewall, MAKEINTRESOURCE(SIDEWALL));
	loadGLTexture(&winewall, MAKEINTRESOURCE(WINEWALL));
	loadGLTexture(&bluewall, MAKEINTRESOURCE(BLUEWALL));
	loadGLTexture(&tiles, MAKEINTRESOURCE(TILES));
	loadGLTexture(&celing, MAKEINTRESOURCE(CELING));
	loadGLTexture(&greywall, MAKEINTRESOURCE(GREYWALL));
	loadGLTexture(&bedsheet, MAKEINTRESOURCE(BEDSHEET));
	loadGLTexture(&bedspread, MAKEINTRESOURCE(BEDSPREAD));
	loadGLTexture(&newspaper, MAKEINTRESOURCE(NEWSPAPER));
	loadGLTexture(&psalms, MAKEINTRESOURCE(PSALMS));
	loadGLTexture(&limit, MAKEINTRESOURCE(LIMIT));
	loadGLTexture(&family, MAKEINTRESOURCE(FAMILY));
	loadGLTexture(&redbull, MAKEINTRESOURCE(REDBULL));
	loadGLTexture(&redwhite, MAKEINTRESOURCE(REDWHITE));
	loadGLTexture(&bluewhite, MAKEINTRESOURCE(BLUEWHITE));

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

	gluPerspective(45.0f, ((GLfloat)width/(GLfloat)height),0.1f,150.0f);
}

void display(void)
{
	void Tree(void);
	void SnowMan(void);
	void Mountains(void);
	void Fence(void);
	void Table(void);
	void Chair(void);
	void Teacup(void);
	void Coconut_Tree(void);
	void Fence_Outer(void);
	void Plastic_Table(void);
	void Wood(void);
	void Shack(void);
	void Pillow(void);
	void WoodenBox(void);
	void Teapoy(void);
	void DrawFont(void);
	void DrawFont1(void);

	//Local variables
	static GLfloat angle = 0.0f;
	static GLfloat yaxis0 = 4.0f;
	static GLfloat yaxis1 = 3.50f;
	static GLfloat yaxis2 = 3.0f;
	static GLfloat yaxis3 = 2.50f;
	static GLfloat yaxis4 = 2.0f;
	static GLfloat yaxis5 = 1.5f;
	static GLfloat yaxis6 = 1.0f;
	static GLfloat yaxis7 = 0.0f;
	static GLfloat yaxis8 = -0.5f;
	static GLfloat yaxis9 = -1.0f;
	static GLfloat yaxis10 = -1.5f;
	static GLfloat yaxis11 = -2.0f;
	static GLfloat yaxis12 = -2.5f;
	static GLfloat yaxis13 = -3.0f;
	static GLfloat yaxis14 = -3.5f;
	static GLfloat yaxis15 = -4.0f;


	//code

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

//**********************************Camera**********************************
/*	gluLookAt(	xaxis, yaxis, zaxis,
				0.0,0.0,0.0,
				0.0f, 1.0f, 0.0f);

	glRotatef(angle, 0.0, 1.0, 0.0);
	//glRotatef(90, 0.0, 1.0, 0.0);
*/

	static GLuint iMagic = 0;	/*This variable is used to render diffrent secne like an key to diffrent secne which
								i would be resetting in diffrent if and else block*/
	static GLuint cnt = 0;

	if (iMagic == 0)
	{

/*		gluLookAt(0.0, 0.0, 900,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);
*/
			DrawFont();

		//glTranslatef(-1.80f, 0.0f, -910.0f);
		if (cnt < 100)
		{
			cnt++;
		}
		else
		{
			//glTranslatef(1.80f, 0.0f, 910.0f);
			PlaySound(TEXT("Tropics"), NULL, SND_ASYNC | SND_RESOURCE);
			zaxis = 500.0f;
			yaxis = 0.0f;
			xaxis = -57.0f;
			iMagic = 1;

		}

	}

	if (iMagic == 1)
	{
		/*The fisrt secne would come from water and go into island on the left side which will hover the camera upwards*/

		//Clearing the color and the modelview matrix for diffrent secne.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);


		//Animation of the secne
		zaxis = zaxis - 0.85;
		yaxis = yaxis + 0.25;

		if (zaxis < 373 && yaxis > 24)
		{
			iMagic = 2;
			zaxis = 375.0f;
			yaxis = 0.0f;
			xaxis = -85.0f;

		}

	}
	else if (iMagic == 2)
	{

		/*This secne would take camera from left side of the island to right then go stright inside*/

		//Clearing the color and the modelview matrix for diffrent secne.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);


		//Animation of the secne	
		xaxis = xaxis + 0.30;

		if (xaxis > -30)
		{
			iMagic = 3;
			zaxis = 375.0f;
			yaxis = 1.0f;
			xaxis = -80.0f;

		}

	}
	else if (iMagic == 3)
	{
		/*This secne would take camera from left side of the island to to inside going diagonal*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		xaxis = xaxis + 0.30;
		yaxis = yaxis + 0.08;
		zaxis = zaxis - 0.50;

		if (zaxis < 309)
		{
			iMagic = 4;
			zaxis = 375.0f;
			yaxis = 0.0f;
			xaxis = -30.0f;
		}

	}
	else if (iMagic == 4)
	{
		/*This secne would take camera from left side of the island to top of the water walk way overview of the pathway sitting palce.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		xaxis = xaxis + 0.20;
		yaxis = yaxis + 0.015;
		zaxis = zaxis - 1.00;

		if (zaxis < 230 && xaxis > -6.0)
		{
			iMagic = 5;
			zaxis = 375.0f;
			yaxis = 1.0f;
			xaxis = 0.0f;
		}

	}
	else if (iMagic == 5)
	{
		/*This secne would take camera from start of pathway to fence.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		zaxis = zaxis - 1.0;

		if (zaxis < 210)
		{
			iMagic = 6;
			zaxis = 210.0f;
			yaxis = -1.50f;
			xaxis = 0.0f;
		}

	}
	else if (iMagic == 6)
	{
		/*This secne would take camera from fence backwards to the pathway.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		zaxis = zaxis + 0.50;

		if (zaxis > 269)
		{
			iMagic = 7;
			zaxis = 269.0f;
			yaxis = 0.0f;
			xaxis = 0.0f;
		}

	}
	else if (iMagic == 7)
	{
		/*This secne would take camera to the house or resto on the path way left side from the turn of the path way .*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne

		if (xaxis < -15)
			xaxis = xaxis - 0.30;
		else
			xaxis = xaxis - 0.50;




		if (xaxis < -33.0)
		{
			iMagic = 8;
			zaxis = 269.0f;
			yaxis = 0.0f;
			xaxis = -33.0f;
		}

	}
	else if (iMagic == 8)
	{
		/*This secne would take camera from the house or resto to the end of pathway.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		xaxis = xaxis + 0.50;

		if (xaxis > 0.1)
		{
			iMagic = 9;
			zaxis = 270.0f;
			yaxis = -3.80f;
			xaxis = 0.0f;
		}

	}
	else if (iMagic == 9)
	{
		/*This secne would take camera from the house or resto to the start of house.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		zaxis = zaxis - 0.50;

		if (zaxis > 200.0)
			yaxis = yaxis + 0.05;

		if (zaxis < 105.0)
		{
			iMagic = 10;
			zaxis = 105.0f;
			yaxis = 0.0f;
			xaxis = 0.0f;
		}

	}
	else if (iMagic == 10)
	{
		/*This secne would take camera up to show the groove.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne	
		if (yaxis < 27.0 && (zaxis <= 105))
		{
			yaxis = yaxis + 0.50;
		}
		else if ((yaxis >= 27.0 || yaxis > 26.50) && (zaxis < 162))
		{
			zaxis = zaxis + 0.50;
			yaxis = yaxis - 0.0001;
		}
		else if ((zaxis > 161) && (yaxis < 27.0))
		{
			xaxis = xaxis - 0.50f;
		}

		if (xaxis < -18.0)
		{
			iMagic = 11;
			zaxis = 162.0f;
			yaxis = 26.90f;
			xaxis = -18.50f;
		}

	}
	else if (iMagic == 11)
	{
		/*This secne would take camera up to show the groove -> from left to right and go down towards the house door.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne	
		if (xaxis < 5.0f)
		{
			xaxis = xaxis + 0.50;
		}
		else if (xaxis >= 4.5f && zaxis > 105)
		{
			zaxis = zaxis - 0.50;
		}
		else if (zaxis < 106.0 && yaxis > 1.0)
		{
			yaxis = yaxis - 0.50f;
			//zaxis = zaxis + 0.00050f;
		}
		else if (yaxis < 1.5 && zaxis > 104)
		{
			xaxis = xaxis - 0.50f;
		}


		if (xaxis >= 0.50 && yaxis < 2.5)
		{
			iMagic = 12;
			zaxis = 105.0f;
			yaxis = 1.0f;
			xaxis = 0.0f;
		}

	}
	else if (iMagic == 12)
	{
		/*This secne would take camera from the house door to inside the house.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		if (zaxis > 68)
		{
			zaxis = zaxis - 0.50;
		}
		else if ((zaxis <= 68 && zaxis >= 42) && (xaxis > -9.0 || yaxis <= 2.5))
		{
			xaxis = xaxis - 0.50;
			yaxis = yaxis + 0.10;
		}
		else if (xaxis <= -8.50 && zaxis > 42.0)
		{
			zaxis = zaxis - 0.30;
		}
		else if (zaxis < 43.0 && yaxis > 2.0 && xaxis < 14.0)
		{
			xaxis = xaxis + 0.25;
		}



		if (xaxis >= 14.0)
		{
			iMagic = 13;
			zaxis = 43.0f;
			yaxis = 3.50f;
			xaxis = 13.0f;
		}

	}
	else if (iMagic == 13)
	{
		/*This secne would take camera from the house door to inside the house camera coming back from iMac to door.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		if (zaxis < 68)
		{
			zaxis = zaxis + 0.20;
		}

		if (zaxis > 68)
		{
			iMagic = 14;
			zaxis = 90.0f;
			yaxis = 0.0f;
			xaxis = 0.0f;
		}

	}
	else if (iMagic == 14)
	{
		/*This secne would take camera from the house door to top room.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		if (yaxis < 17)
		{
			yaxis = yaxis + 0.50;
		}
		else if (yaxis >= 16)
		{
			zaxis = zaxis - 0.50;
		}


		if (zaxis < 45.0)
		{
			iMagic = 15;
			zaxis = 166.0f;
			yaxis = 4.0f;
			xaxis = 33.0f;
		}

	}
	else if (iMagic == 15)
	{
		/*This secne would take camera from the canopys outside on the walking trails.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		if (zaxis < 211)
		{
			zaxis = zaxis + 0.50;
		}
		else if (zaxis >= 210)
		{
			xaxis = xaxis - 0.50f;
		}

		if (xaxis < -20.0)
		{
			iMagic = 16;
			zaxis = 350.0f;
			yaxis = -3.50f;
			xaxis = 0.0f;
		}

	}
	else if (iMagic == 16)
	{
		/*This secne would come walk way to snow worlds.*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//**********************************Camera**********************************
		gluLookAt(xaxis, yaxis, zaxis,
			0.0, 0.0, 0.0,
			0.0f, 1.0f, 0.0f);

		//Animation of the secne
		if (zaxis > 25)
		{
			zaxis = zaxis - 1.0;
			if (yaxis < 0.0)
				yaxis = yaxis + 0.10;
		}

		if (zaxis <= 25)
		{
			iMagic = 17;
			zaxis = 700;
			yaxis = 0;
			xaxis = 100;
		}
	}
	else if (iMagic == 17)
	{
		/*Black Screen*/

		//Clearing the color and the modelview matrix for diffrent secne.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
		DrawFont1();


	}


	fprintf(gpFILE, "zaxis value : %lf\nyaxis value : %lf\nxaxis value : %lf\n", zaxis,yaxis,xaxis);

glPushMatrix();						//Push Matrix

	//**********************************land**********************************

	glPushMatrix();						//Push Matrix
		glTranslatef(0.0f, 0.0f, -1.0f);
		glBindTexture(GL_TEXTURE_2D, groove_land);
		glBegin(GL_QUADS);
		glColor3f(45.0, 38.0, 15.0f);
			glTexCoord2f(4.0f, 4.0f);
			glVertex3f(20.00, 0.05f, -6.0f);
			glTexCoord2f(0.0f, 4.0f);
			glVertex3f(-20.00, 0.05f, -6.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-16.0, -4.0f, 10.0f);
			glTexCoord2f(4.0f, 0.0f);
			glVertex3f(16.0, -4.0f, 10.0f);
		glEnd();
	glPopMatrix();					//Pop Matrix


	/////////////////////////////////////
	//			House
	////////////////////////////////////

	
	// 1st Floor of the house

	glPushMatrix();						//Push Matrix
		glTranslatef(0.0, 0.0, -1.0);
		glBegin(GL_QUADS);
			glVertex3f(2.0, -4.0, 8.0);
			glVertex3f(-2.0, -4.0, 8.0);
			glVertex3f(-2.0, -4.0, 25.0);
			glVertex3f(2.0, -4.0, 25.0);
		glEnd();

		////////////////////////////////////////
		//				Wall
		////////////////////////////////////////

		glBindTexture(GL_TEXTURE_2D, greywall);		
		glBegin(GL_QUADS);
			//left back wall
			//glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0f,  8.0, 25.01);
			//glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-15.0f, 8.0, 25.01);
			//glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-15.0f, -4.0, 25.01);
			//glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, -4.0, 25.01);

			//center top
			//glTexCoord2f(0.0f, 0.0f);
			glVertex3f(2.5f, 8.0, 25.0);
			//glTexCoord2f(0.0f, 0.80f);
			glVertex3f(-2.5f, 8.0, 25.0);
			//glTexCoord2f(0.80f, 0.80f);
			glVertex3f(-2.5f, 1.0, 25.0);
			//glTexCoord2f(0.80f, 0.0f);
			glVertex3f(2.5f, 1.0, 25.0);


/*			//center bottom
			glVertex3f(1.0f, -2.5, 25.0);
			glVertex3f(-1.0f, -2.5, 25.0);
			glVertex3f(-1.0f, -4.0, 25.0);
			glVertex3f(1.0f, -4.0, 25.0);
*/
			//right back wall
			//glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 8.0, 25.01);
			//glTexCoord2f(1.0f, 0.0f);
			glVertex3f(15.0f, 8.0, 25.01);
			//glTexCoord2f(0.0f, 0.0f);
			glVertex3f(15.0f, -4.0, 25.01);
			//glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0f, -4.0, 25.01);
		glEnd();


		//1st Layer of the side wall
		glBindTexture(GL_TEXTURE_2D, bluewall);
		glBegin(GL_QUADS);
			//left side wall
			//glColor3f(1.0, 0.4, 0.5);
			//glTexCoord2f(1.0,1.0);
			glVertex3f(-15.0f, 8.0, 70.0);
			//glTexCoord2f(1.0, 0.0);
			glVertex3f(-15.0f, 8.0, 25.0);
			//glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, -4.0, 25.0);
			//glTexCoord2f(0.0, 1.0);
			glVertex3f(-15.0f, -4.0, 70.0);


			//right side wall
			//glColor3f(0.8, 0.5, 0.0);
			//glTexCoord2f(1.0, 1.0);
			glVertex3f(15.0f, 8.0, 25.0);
			//glTexCoord2f(1.0, 0.0);
			glVertex3f(15.0f, 8.0, 70.0);
			//glTexCoord2f(0.0, 0.0);
			glVertex3f(15.0f, -4.0, 70.0);
			//glTexCoord2f(0.0, 1.0);
			glVertex3f(15.0f, -4.0, 25.0);
		glEnd();


		//2nd layer of the side wall
		glBindTexture(GL_TEXTURE_2D, wall);
		glBegin(GL_QUADS);
			//left side wall
			//glColor3f(1.0, 0.4, 0.5);
			glTexCoord2f(1.0,1.0);
			glVertex3f(-15.01f, 8.0, 70.0);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-15.01f, 8.0, 25.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.01f, -4.0, 25.0);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-15.01f, -4.0, 70.0);


			//right side wall
			//glColor3f(0.8, 0.5, 0.0);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(15.01f, 8.0, 25.0);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(15.01f, 8.0, 70.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(15.01f, -4.0, 70.0);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(15.01f, -4.0, 25.0);
		glEnd();


		glBindTexture(GL_TEXTURE_2D, celing);
		glBegin(GL_QUADS);
			//Roof Top for the ground floor of the house 
			//glColor3f(0.50, 0.50, 0.50);
			//glTexCoord2f(1.0, 0.0);
			glVertex3f(15.0f, 8.0f, 25.0f);
			//glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.0f, 25.0f);
			//glTexCoord2f(0.0, 1.0);
			glVertex3f(-15.0f, 8.0f, 70.0f);
			//glTexCoord2f(1.0, 1.0);
			glVertex3f(15.0f, 8.0f, 70.0f);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, tiles);
		glBegin(GL_QUADS);
			//Bottom floor
			//glColor3f(0.50, 0.50, 0.50);
			glTexCoord2f(4.0, 4.0);
			glVertex3f(15.0f, -4.0f, 25.0f);
			glTexCoord2f(4.0, 0.0);
			glVertex3f(-15.0f, -4.0f, 25.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, -4.0f, 70.0f);
			glTexCoord2f(0.0, 4.0);
			glVertex3f(15.0f, -4.0f, 70.0f);
		glEnd();


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//												front door way 
		//										This is divided in 3 parts left side , center top , right top
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//left front wall
		glBindTexture(GL_TEXTURE_2D, wall);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(-1.0f, 8.0, 70.0);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(-15.0f, 8.0, 70.0);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(-15.0f, -4.0, 70.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-1.0f, -4.0, 70.0);

			//center top
			glTexCoord2f(0.0, 1.0);
			glVertex3f(2.5f, 8.0, 70.01);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-2.5f, 8.0, 70.01);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-2.5f, 1.0, 70.01);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(2.5f, 1.0, 70.01);

			//right front wall
			glTexCoord2f(0.0, 2.0);
			glVertex3f(1.0f, 8.0, 70.0);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(15.0f, 8.0, 70.0);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(15.0f, -4.0, 70.0);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(1.0f, -4.0, 70.0);
		glEnd();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//														2nd Floor of the house
		//
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		glBindTexture(GL_TEXTURE_2D, tiles);
		glBegin(GL_QUADS);
			//2nd layer Roof Top for the top floor's bottom
			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 8.01f, 25.0f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 25.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 70.0f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 70.0f);
		glEnd();

		//Wall around the house
		glBindTexture(GL_TEXTURE_2D, wall);

		//Back Wall
		glBegin(GL_QUADS);
			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 12.0f, 25.0f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 12.0f, 25.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 25.0f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 25.0f);
		glEnd();

		//Side Wall Right
		glBegin(GL_QUADS);
			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 12.0f, 25.0f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(15.0f, 12.0f, 70.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(15.0f, 8.01f, 70.0f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 25.0f);
		glEnd();


		//Side Wall Left
		glBegin(GL_QUADS);
			glTexCoord2f(5.0, 5.0);
			glVertex3f(-15.0f, 12.0f, 70.0f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 12.0f, 25.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 25.0f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-15.0f, 8.01f, 70.0f);
		glEnd();

		//Front Wall Left Part
		glBegin(GL_QUADS);
			glTexCoord2f(5.0, 5.0);
			glVertex3f(-1.50f, 12.0f, 70.0f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 12.0f, 70.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 70.0f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-1.50f, 8.01f, 70.0f);
		glEnd();

		//Front Wall Right Part
		glBegin(GL_QUADS);
			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 12.0f, 70.0f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(1.50f, 12.0f, 70.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(1.50f, 8.01f, 70.0f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 70.0f);
		glEnd();

		//****************************************************four pillars of house of top room****************************************************

		//1st pillar on right side front
		glBegin(GL_QUADS);
			
			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 18.0f, 69.99f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(14.0f, 18.0f, 69.99f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(14.0f, 8.01f, 69.99f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 69.99f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 18.0f, 68.99f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(14.0f, 18.0f, 68.99f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(14.0f, 8.01f, 68.99f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 68.99f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(14.0f, 18.0f, 69.99f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(14.0f, 18.0f, 68.99f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(14.0f, 8.01f, 68.99f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(14.0f, 8.01f, 69.99f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 18.0f, 68.99f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(15.0f, 18.0f, 69.99f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(15.0f, 8.01f, 69.99f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 68.99f);

		glEnd();

		//Right Side back 
		glBegin(GL_QUADS);
			
			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 18.0f, 25.01f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(14.0f, 18.0f, 25.01f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(14.0f, 8.01f, 25.01f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 25.01f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 18.0f, 26.01f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(14.0f, 18.0f, 26.01f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(14.0f, 8.01f, 26.01f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 26.01f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(14.0f, 18.0f, 25.01f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(14.0f, 18.0f, 26.01f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(14.0f, 8.01f, 26.01f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(14.0f, 8.01f, 25.01f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 18.0f, 26.01f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(15.0f, 18.0f, 25.01f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(15.0f, 8.01f, 25.01f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 8.01f, 26.01f);

		glEnd();

		//Left back
		 
		glBegin(GL_QUADS);
			
			glTexCoord2f(5.0, 5.0);
			glVertex3f(-14.0f, 18.0f, 25.01f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 18.0f, 25.01f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 25.01f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-14.0f, 8.01f, 25.01f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(-14.0f, 18.0f, 26.01f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 18.0f, 26.01f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 26.01f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-14.0f, 8.01f, 26.01f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(-14.0f, 18.0f, 25.01f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-14.0f, 18.0f, 26.01f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-14.0f, 8.01f, 26.01f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-14.0f, 8.01f, 25.01f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(-14.0f, 18.0f, 26.01f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-14.0f, 18.0f, 25.01f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-14.0f, 8.01f, 25.01f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-14.0f, 8.01f, 26.01f);

		glEnd();


		//pillar on left side front
		glBegin(GL_QUADS);
			
			glTexCoord2f(5.0, 5.0);
			glVertex3f(-14.0f, 18.0f, 69.99f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 18.0f, 69.99f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 69.99f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-14.0f, 8.01f, 69.99f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(-14.0f, 18.0f, 68.99f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 18.0f, 68.99f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 68.99f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-14.0f, 8.01f, 68.99f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(-15.0f, 18.0f, 69.99f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 18.0f, 68.99f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 8.01f, 68.99f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-15.0f, 8.01f, 69.99f);


			glTexCoord2f(5.0, 5.0);
			glVertex3f(-14.0f, 18.0f, 68.99f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-14.0f, 18.0f, 69.99f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-14.0f, 8.01f, 69.99f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(-14.0f, 8.01f, 68.99f);

		glEnd();



		//roof
		glBindTexture(GL_TEXTURE_2D, celing);
		glBegin(GL_QUADS);
			//2nd layer Roof Top for the top floor's bottom
			glTexCoord2f(5.0, 5.0);
			glVertex3f(15.0f, 18.01f, 25.0f);
			glTexCoord2f(5.0, 0.0);
			glVertex3f(-15.0f, 18.01f, 25.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, 18.01f, 70.0f);
			glTexCoord2f(0.0, 5.0);
			glVertex3f(15.0f, 18.01f, 70.0f);
		glEnd();



		//bed on the 2nd floor
		glBindTexture(GL_TEXTURE_2D, bedsheet);
		glBegin(GL_QUADS);
			//top of bed
			glTexCoord2f(1.0, 1.0);
			glVertex3f(05.0f, 10.01f, 45.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-05.0f, 10.01f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-05.0f, 10.01f, 55.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(05.0f, 10.01f, 55.0f);

			//bottom of bed
			//glTexCoord2f(5.0, 5.0);
			glVertex3f(05.0f, 9.01f, 45.0f);
			//glTexCoord2f(5.0, 0.0);
			glVertex3f(-05.0f, 9.01f, 45.0f);
			//glTexCoord2f(0.0, 0.0);
			glVertex3f(-05.0f, 9.01f, 55.0f);
			//glTexCoord2f(0.0, 5.0);
			glVertex3f(05.0f, 9.01f, 55.0f);

			//front of bed
			//glTexCoord2f(5.0, 5.0);
			glVertex3f(05.0f, 10.01f, 55.0f);
			//glTexCoord2f(5.0, 0.0);
			glVertex3f(-05.0f, 10.01f, 55.0f);
			//glTexCoord2f(0.0, 0.0);
			glVertex3f(-05.0f, 9.01f, 55.0f);
			//glTexCoord2f(0.0, 5.0);
			glVertex3f(05.0f, 9.01f, 55.0f);

			//back of bed
			//glTexCoord2f(5.0, 5.0);
			glVertex3f(05.0f, 10.01f, 45.0f);
			//glTexCoord2f(5.0, 0.0);
			glVertex3f(-05.0f, 10.01f, 45.0f);
			//glTexCoord2f(0.0, 0.0);
			glVertex3f(-05.0f, 9.01f, 45.0f);
			//glTexCoord2f(0.0, 5.0);
			glVertex3f(05.0f, 9.01f, 45.0f);

		glEnd();

		//top of bed bedspread , over the bed to cover while sleeping
		glBindTexture(GL_TEXTURE_2D, bedspread);
		glBegin(GL_QUADS);			
			glTexCoord2f(10.0, 10.0);
			glVertex3f(04.90f, 10.02f, 50.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-04.90f, 10.02f, 50.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-04.90f, 10.02f, 54.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(04.90f, 10.02f, 54.90f);
		glEnd();
		
		//pillow
		glPushMatrix();
			glTranslatef(1.0, 0.0, 0.0);
			Pillow();			
		glPopMatrix();

		glPushMatrix();
			glTranslatef(3.5, 0.0, 0.0);
			Pillow();			
		glPopMatrix();

		glPushMatrix();
			glTranslatef(6.0, 0.0, 0.0);
			Pillow();			
		glPopMatrix();

		//4 Legs of cot
		glPushMatrix();
			glTranslatef(-5.0, 10.0, 45.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, fence_texture);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.2, 0.2, 2.0, 30, 30);
			gluQuadricTexture(quadric, TRUE);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(5.0, 10.0, 45.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, fence_texture);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.2, 0.2, 2.0, 30, 30);
			gluQuadricTexture(quadric, TRUE);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(5.0, 10.0, 55.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, fence_texture);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.2, 0.2, 2.0, 30, 30);
			gluQuadricTexture(quadric, TRUE);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-5.0, 10.0, 55.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, fence_texture);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.2, 0.2, 2.0, 30, 30);
			gluQuadricTexture(quadric, TRUE);
		glPopMatrix();

		//wooden boxes
		glPushMatrix();
			glTranslatef(-0.50, 0.0, 0.0);
			WoodenBox();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(13.0, 0.0, 0.0);
			WoodenBox();
		glPopMatrix();

		//Teapoy on 2nd floor
		glPushMatrix();
			glTranslatef(5.0, 0.0, 15.0);
			Teapoy();
		glPopMatrix();

		
		glPushMatrix();
			glTranslatef(8.0, 2.80, 30.0);
			glScalef(0.65, 0.65, 0.65);
			WoodenBox();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(1.00, 2.80, 30.0);
			glScalef(0.75, 0.65, 0.65);
			WoodenBox();
		glPopMatrix();

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//land in and around the house with all coconut tree( marsh / green moss)
		glBindTexture(GL_TEXTURE_2D, groove_land);
		glBegin(GL_QUADS);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(35.0f, -4.02f, 25.0f);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(-35.0f, -4.02f, 25.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-35.0f, -4.02f, 200.0f);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(35.0f, -4.02f, 200.0f);
		glEnd();

		//Path to walk
		glBindTexture(GL_TEXTURE_2D, path);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 25.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 25.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 40.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 40.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 40.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 40.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 55.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 55.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 55.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 55.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 70.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 70.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 70.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 70.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 85.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 85.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 85.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 85.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 100.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 100.0f);


			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 100.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 100.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 115.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 115.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 115.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 115.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 125.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 125.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 125.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 125.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 140.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 140.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 140.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 140.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 155.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 155.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 155.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 155.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 170.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 170.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 170.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 170.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 185.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 185.0f);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(33.0f, -4.01f, 185.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 185.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 190.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(33.0f, -4.01f, 190.0f);

			//path way in front
			glTexCoord2f(1.0, 0.0);
			glVertex3f(33.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(29.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(33.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(29.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(25.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(25.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(29.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(25.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(20.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(20.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(25.0f, -4.01f, 195.0f);


			glTexCoord2f(1.0, 0.0);
			glVertex3f(20.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(15.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(15.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(20.0f, -4.01f, 195.0f);


			glTexCoord2f(1.0, 0.0);
			glVertex3f(15.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(10.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(10.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(15.0f, -4.01f, 195.0f);


			glTexCoord2f(1.0, 0.0);
			glVertex3f(10.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(5.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(5.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(10.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(5.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(0.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(0.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(5.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(0.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-5.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-5.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(-5.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-10.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-10.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-5.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(-10.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-15.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-15.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-10.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(-15.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-20.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-20.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-15.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(-20.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-25.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-25.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-20.0f, -4.01f, 195.0f);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(-25.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-30.0f, -4.01f, 190.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-30.0f, -4.01f, 195.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-25.0f, -4.01f, 195.0f);

		glEnd();


		//Water around the house / sea
		glBindTexture(GL_TEXTURE_2D, water);
		glBegin(GL_QUADS);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(200.0f, -4.03f, -8.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-200.0f, -4.03f, -8.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-200.0f, -4.03f, 500.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(200.0f, -4.03f, 500.0f);

		glEnd();


		glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();					//Pop Matrix
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//						Fence outside house(function call)
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Fence();

	//********************************************Mountains********************************************	

	glPushMatrix();						//Push Matrix			
			glTranslatef(0.0f, 1.5f, -6.0f);
			Mountains();
			//right side mountain
			glTranslatef(5.5f, 0.0f, 0.0f);
			Mountains();

			glTranslatef(5.5f, 0.50f, 0.0f);
			Mountains();

			glTranslatef(5.5f, 0.0f, 0.0f);
			Mountains();

			glTranslatef(5.5f, 0.50f, 0.0f);
			//Mountains();

			//left side mountain
			glTranslatef(-30.5f, 0.0f, 0.0f);
			Mountains();

			glTranslatef(-5.5f, -0.20f, 0.0f);
			Mountains();

			glTranslatef(-5.5f, 0.0f, 0.0f);
			Mountains();

			glTranslatef(-2.5f, 0.0f, 0.0f);
			//Mountains();
	//*********************************************************Tree******************************************************************************
	
			glTranslatef(20.0f, -1.5f, 13.0);
			//Tree();
	
			//Background forest tree	
	
			//1st row background
			glTranslatef(-3.0f, 0.0f, -7.50);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(14.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(4.0f, 0.0f, 0.0);
			Tree();

			//2nd row background
			glTranslatef(8.0f, 0.0f, -4.0);
			glScalef(0.40f,0.40f,0.40f);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();

			glTranslatef(-4.0f, 0.0f, 0.0);
			Tree();
	//***************************************************************************************************************************
	//Snowman
		glTranslatef(45.0f, -4.0f, 25.0f);
		glScalef(3.0f, 3.0f, 3.0f);
		//SnowMan();

	glPopMatrix();					//Pop Matrix

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//										Inside House Objects
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	
	//											Dining Table
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Table on the left side of the door
	glPushMatrix();
		glTranslatef(-8.0f, 1.0f, 0.0f);
		Table();
	glPopMatrix();

	//************************************chair of the dining table**********************************************

	////Right Side
	//1st chair
	glPushMatrix();
		glTranslatef(-7.0f, -1.0f, 46.50f);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		//glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();

	//2nd chair
	glPushMatrix();
		glTranslatef(-7.0f, -1.0f, 49.0f);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		//glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();

	//3nd chair
	glPushMatrix();
		glTranslatef(-7.0f, -1.0f, 52.0f);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		//glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();



	////Left Side
	//1st chair
	glPushMatrix();
		glTranslatef(-11.0f, -1.0f, 46.0f);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		//glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();

	//2nd chair
	glPushMatrix();
		glTranslatef(-11.0f, -1.0f, 49.0f);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		//glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();


	//3rd chair
	glPushMatrix();
		glTranslatef(-11.0f, -1.0f, 52.0f);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		//glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();


	//front chair
	glPushMatrix();
		glTranslatef(-9.0f, -1.0f, 56.0f);
		glRotatef(180.0, 0.0, 1.0, 0.0);
		//glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();

	//back chair
	glPushMatrix();
		glTranslatef(-9.0f, -1.0f, 44.0f);
		//glRotatef(180.0, 0.0, 1.0, 0.0);
		//glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();

	//*************************************** End of Chair ***************************************

	//*************************************** Utensils on dining table ***************************
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//							Tea Cup Placement on the dining table
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	glPushMatrix();
		glTranslatef(0.0,-0.35,-0.50);
		Teacup();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.50, -0.35, 0.70);
		Teacup();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.50, -0.35, 0.70);
		Teacup();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.50, -0.35, 3.20);
		Teacup();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.50, -0.35, 3.20);
		Teacup();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.50, -0.35, 6.00);
		Teacup();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.50, -0.35, 6.00);
		Teacup();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, -0.35, 7.50);
		Teacup();
	glPopMatrix();
	//**********************************End of Tea Cup Placement on the table**********************************************************

	//plate
	glPushMatrix();
		glTranslatef(-9.0, -0.90, 48.00);
		glBindTexture(GL_TEXTURE_2D, marble);
		quadric = gluNewQuadric();
		glScalef(1.0, 0.10, 1.0);
		gluQuadricTexture(quadric, TRUE);
		gluSphere(quadric, 0.5, 30, 30);	
/*			glPushMatrix();
				glBindTexture(GL_TEXTURE_2D, gold);
				quadric = gluNewQuadric();
				glTranslatef(-9.0, -0.90, 48.00);
				glRotatef(90.0f, 1.0f, 0.0, 0.0f);
	//			gluCylinder(quadric, 0.50, 0.50, 3.80, 30, 30);
				gluQuadricTexture(quadric, TRUE);
			glPopMatrix();
*/		
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-9.0, -1.00, 50.00);
		glBindTexture(GL_TEXTURE_2D, marble);
		quadric = gluNewQuadric();
		glScalef(1.0, 0.10, 1.0);
		gluQuadricTexture(quadric, TRUE);
		gluSphere(quadric, 0.5, 30, 30);		
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-9.0, -1.00, 52.00);
		glBindTexture(GL_TEXTURE_2D, marble);
		quadric = gluNewQuadric();
		glScalef(1.0, 0.10, 1.0);
		gluQuadricTexture(quadric, TRUE);
		gluSphere(quadric, 0.5, 30, 30);
	glPopMatrix();

	//*************************************** End of Utensils on dining table ***************************

	//********************************************* Teapoy Placement in the House ground floor *******************************************

	glPushMatrix();
		glTranslatef(14.0, -12.0, 4.00);
		//glRotatef(90.0, 1.0, 0.0, 0.0);
		Teapoy();
	glPopMatrix();


	//near teapoy 4 box chair to sit
	glColor3f(0.50f, 0.50f, 0.50f);
	glPushMatrix();
		glTranslatef(11.0, -12.0, 4.00);
		WoodenBox();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(20.0, -12.0, 4.00);
		WoodenBox();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.50, -12.0, 1.00);
		WoodenBox();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.50, -12.0, 7.00);
		WoodenBox();
	glPopMatrix();
	//resetting color back
	glColor3f(1.0f, 1.0f, 1.0f);


	//********************************************** Newspaper **********************************************
	glPushMatrix();
		glTranslatef(8.50, 0.0, 48.00);
		//glScalef(0.90, 0.70, 0.0);
		glBindTexture(GL_TEXTURE_2D, newspaper);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0, -1.43, 1.50);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0, -1.43, 1.50);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0, -1.43, 2.50);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0, -1.43, 2.50);
		glEnd();
	glPopMatrix();

	//********************************************** end of newspaper **********************************************


	//**********************************************Photo Frames**********************************************
	//on the right side of the wall next to the teapoy
/*	glPushMatrix();
		glTranslatef(13.50, 3.0, 42.00);
		glBindTexture(GL_TEXTURE_2D, limit);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0, 2.50, 1.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0, 2.50, 2.50);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0, -2.50, 2.50);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0, -2.50, 1.00);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-15.90, 3.0, 35.00);
		glBindTexture(GL_TEXTURE_2D, family);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0, 2.50, 1.0);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0, 2.50, 2.50);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0, -2.50, 2.50);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0, -2.50, 1.00);
		glEnd();
	glPopMatrix();

*/

	//front wall
	glPushMatrix();
		glTranslatef(0.00, 4.50, 24.0);
		glBindTexture(GL_TEXTURE_2D,grey);	
		glBegin(GL_QUADS);
			glColor3f(0.1, 0.1, 0.1);
			glVertex3f(1.10, 2.60, 1.50);
			glVertex3f(-1.10, 2.60, 1.50);
			glVertex3f(-1.10, -1.60, 1.50);
			glVertex3f(1.10, -1.60, 1.50);
		glEnd();


		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, psalms);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(1.0, 2.50, 1.51);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0, 2.50, 1.51);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0, -1.50, 1.51);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(1.0, -1.50, 1.51);
		glEnd();
	glPopMatrix();




	//**********************************************End of Photo Frames **********************************************

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//									Computer Table				
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//******************************Table on the right side of the door.******************************
	glPushMatrix();
		glRotatef(90.0f, 0.0, 0.10, 0.0);
		glTranslatef(-30.0f, 1.0f, -41.0f);
		//glTranslatef(25.0f, 0.0f, -1.0f);
		Table();
	glPopMatrix();
	//******************************end of table on right side******************************


	//******************************Computer******************************
	glPushMatrix();
		glTranslatef(8.0f, 0.50f, 31.50f);
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

	//******************************Photo Frame******************************
	glPushMatrix();
		glTranslatef(11.50f, 0.50f, 31.50f);
		//glBindTexture(GL_TEXTURE_2D, grey);
		//glColor3f(0.0f,0.0f,0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(0.35, -0.50, 0.10);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-0.35, -0.50, -0.20);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-0.35, -1.5, -0.20);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.35, -1.5, 0.10);
		glEnd();

		//behind stand
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(0.05, -1.0, -0.10);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-0.05, -1.0, -0.10);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-0.05, -1.5, -0.50);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.05, -1.5, -0.50);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, tatha);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(0.25, -0.60, 0.10);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-0.25, -0.60, -0.15);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-0.25, -1.40, -0.15);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.25, -1.40, 0.10);
		glEnd();


	glPopMatrix();
	//******************************end of Photo Frame******************************

	//************************************Books*************************************

	//************************************LPI***************************************
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, lpi);
	glTranslatef(13.50f, 0.50f, 31.50f);
	//glRotatef(5.0,0.0,1.0,0.0);
	//top
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.3, -0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.3, -0.60);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.3, 0.90);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.3, 0.90);
	glEnd();

	//front
	glBindTexture(GL_TEXTURE_2D, pages);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.3, 0.85);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.3, 0.85);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.48, 0.85);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, 0.85);
	glEnd();

	//right
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.3, -0.50);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(0.35, -1.3, 0.85);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(0.35, -1.48, 0.85);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, -0.50);
	glEnd();

	//back
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.3, -0.50);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.3, -0.50);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.48, -0.50);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, -0.50);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, lpi_side);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.3, 0.90);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.3, -0.60);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-0.35, -1.48, -0.60);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-0.35, -1.48, 0.90);
	glEnd();


	//bottom
	glBindTexture(GL_TEXTURE_2D, grey);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.48, -0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.48, -0.60);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.48, 0.90);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, 0.90);
	glEnd();

	glPopMatrix();
	//************************************End of LPI***************************************

	//************************************BACH***************************************
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, bach);
	glTranslatef(12.50f, 0.50f, 31.50f);
	//glRotatef(5.0,0.0,1.0,0.0);
	//top
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.38 ,-0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.38, -0.60);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.38, 0.90);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.38, 0.90);
	glEnd();

	//front
	glBindTexture(GL_TEXTURE_2D, pages);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.38, 0.85);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.38, 0.85);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.48, 0.85);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, 0.85);
	glEnd();

	//right
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.38, -0.50);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(0.35, -1.38, 0.85);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(0.35, -1.48, 0.85);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, -0.50);
	glEnd();

	//back
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.38, -0.50);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.38, -0.50);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.48, -0.50);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, -0.50);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, bach_side);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.38, 0.90);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.38, -0.60);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-0.35, -1.48, -0.60);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-0.35, -1.48, 0.90);
	glEnd();


	//bottom
	glBindTexture(GL_TEXTURE_2D, bach);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.48, -0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.48, -0.60);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.48, 0.90);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, 0.90);
	glEnd();

	glPopMatrix();
	//************************************End of BACH***************************************
	
	//************************************End of Books**************************************

	//************************************Coffe Mug****************************************

	glPushMatrix();
		glTranslatef(5.0f, -0.30f, 31.50f);
		glRotatef(90.0f,1.0f,0.0,0.0);
		glBindTexture(GL_TEXTURE_2D, gold);
		quadric = gluNewQuadric();
		gluQuadricTexture(quadric, TRUE);
		gluCylinder(quadric, 0.3, 0.25, 0.70, 30, 30);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(5.25f, -0.60f, 31.50f);
		glRotatef(22.50f, 0.0f, 1.0, 0.0);
		glBindTexture(GL_TEXTURE_2D, gold);
		quadric = gluNewQuadric();
		gluQuadricTexture(quadric, TRUE);
		gluCylinder(quadric, 0.25, 0.25, 0.1, 30, 30);
	glPopMatrix();

	//************************************End Coffe Mug************************************

	//*************************************Light Lamp**************************************
	//if you want to put lamp then put translate & rotate in individual push pop
	
	glPushMatrix();
		glTranslatef(13.0f, 1.0f, 28.0f);
		glRotatef(90.0f, 1.0f, 0.0, 0.0);
		//lamp
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, snow_texture);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 1.0, 1.0, 1.0f, 30, 30);
		glPopMatrix();

		//stand
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, fence_texture);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.10, 0.10, 5.0f, 30, 30);
		glPopMatrix();
	glPopMatrix();


	//Light Lamp on 2nd floor
	//*************************************Light Lamp**************************************
	//if you want to put lamp then put translate & rotate in individual push pop
	
	glPushMatrix();
		glTranslatef(11.0f, 13.0f, 45.0f);
		glRotatef(90.0f, 1.0f, 0.0, 0.0);
		//lamp
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, snow_texture);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 1.0, 1.0, 1.0f, 30, 30);
		glPopMatrix();

		//stand
		glPushMatrix();
			glBindTexture(GL_TEXTURE_2D, fence_texture);
			quadric = gluNewQuadric();
			gluCylinder(quadric, 0.10, 0.10, 5.0f, 30, 30);
		glPopMatrix();
	glPopMatrix();


	//*************************************End Light Lamp**************************************

	//*************************************** Chair *********************************************

	//computer table chair
	glPushMatrix();
		glTranslatef(8.0f, 0.0f, 35.0f);
		glRotatef(180.0,0.0,1.0,0.0);
		glScalef(1.50f, 1.50, 1.50);
		Chair();
	glPopMatrix();

	////////////////////////////////////////////////////////////////// End Of Computer Table //////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//													Top Cabinet
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, fence_texture);
	glTranslatef(5.50f, 3.50f, 25.50f);
	//glRotatef(5.0,0.0,1.0,0.0);
	//top
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(8.00, 2.00, -0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-1.00, 2.00, -0.60);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-1.00, 2.00, 0.90);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(8.0, 2.00, 0.90);
	glEnd();

/*	//front
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(0.35, -1.38, 0.85);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-0.35, -1.38, 0.85);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-0.35, -1.48, 0.85);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0.35, -1.48, 0.85);
	glEnd();
*/
	//right
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(8.0, 2.0, -0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(8.0, 2.0, 0.90);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(8.0, -2.0, 0.90);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(8.0, -2.0, -0.60);
	glEnd();

	//back
/*	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-1.0, 2.0, -0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(8.00, 2.0, -0.60);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(8.00, -2.0, -0.60);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1.0, -2.0, -0.60);
	glEnd();
*/
	//left
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-1.0, 2.0, 0.90);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-1.0, 2.0, -0.60);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-1.0, -2.0, -0.60);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1.0, -2.0, 0.90);
	glEnd();


	//bottom
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(8.0, -2.00, -0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-1.0, -2.00, -0.60);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-1.0, -2.00, 0.90);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(8.0, -2.00, 0.90);
	glEnd();

	//Single loft 
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(8.0, 0.00, -0.60);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-1.0, 0.00, -0.60);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-1.0, 0.00, 0.90);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(8.0, 0.00, 0.90);
	glEnd();


	glPopMatrix();
	
	/////////////////////////////////////////////////////// End of top cabinet //////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//													Fire Place
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	glPushMatrix();
	//glBindTexture(GL_TEXTURE_2D, fence_texture);
		glBindTexture(GL_TEXTURE_2D, brick);
		glTranslatef(-8.50f, -2.0f, 25.50f);
		//glRotatef(5.0,0.0,1.0,0.0);
	
		//Chimney of fire place

		//front
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(2.00, 10.0, 0.75);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(1.00, 10.0, 0.75);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(1.00, 6.0, 0.75);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(2.00, 6.0, 0.75);
		glEnd();

		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(2.00, 6.0, 0.75);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(1.00, 6.0, 0.75);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(1.00, 2.0, 0.75);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(2.00, 2.0, 0.75);
		glEnd();




		//right
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(2.0, 10.0, -0.60);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(2.0, 10.0, 0.75);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(2.0, 2.0, 0.75);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(2.0, 2.0, -0.60);
		glEnd();

		//left
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(1.0, 10.0, 0.75);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(1.0, 10.0, -0.60);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(1.0, 2.0, -0.60);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(1.0, 2.0, 0.75);
		glEnd();

		//**************************************************fire place box structure*****************************************************
		//top
		glBegin(GL_QUADS);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(4.00, 2.00, -0.60);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-1.00, 2.00, -0.60);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(-1.00, 2.00, 0.90);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(4.0, 2.00, 0.90);
		glEnd();

		//front - 1 Half
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(0.50, 2.0, 0.90);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(-1.0, 2.0, 0.90);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(-1.0, -2.0, 0.90);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(0.50, -2.0, 0.90);
		glEnd();

		//front - mid
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(2.50, 2.0, 0.90);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(0.50, 2.0, 0.90);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(0.50, 0.5, 0.90);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(2.50, 0.5, 0.90);
		glEnd();



		//front - 2 Half
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(4.0, 2.0, 0.90);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(2.50, 2.0, 0.90);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(2.50, -2.0, 0.90);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(4.0, -2.0, 0.90);
		glEnd();



		//right
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(4.0, 2.0, -0.60);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(4.0, 2.0, 0.90);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(4.0, -2.0, 0.90);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(4.0, -2.0, -0.60);
		glEnd();

		//left
		glBindTexture(GL_TEXTURE_2D, brick);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 2.0);
			glVertex3f(-1.0, 2.0, 0.90);
			glTexCoord2f(2.0, 2.0);
			glVertex3f(-1.0, 2.0, -0.60);
			glTexCoord2f(2.0, 0.0);
			glVertex3f(-1.0, -2.0, -0.60);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-1.0, -2.0, 0.90);
		glEnd();

		//bottom
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(4.0, -2.00, -0.60);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-1.0, -2.00, -0.60);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-1.0, -2.00, 0.90);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(4.0, -2.00, 0.90);
		glEnd();

		//back
		glBindTexture(GL_TEXTURE_2D, fence_texture);
		glColor3f(0.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-1.0, 2.0, -0.60);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(4.00, 2.0, -0.60);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(4.00, -2.0, -0.60);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-1.0, -2.0, -0.60);
		glEnd();
	
		//resetting the color to white
		glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();

	//wooden chair like to sit near fire place
	glColor3f(0.50f, 0.50f, 0.50f);
	glPushMatrix();
		glTranslatef(-2.50, -12.0, -12.50);
		WoodenBox();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.50, -12.0, -12.50);
		WoodenBox();
	glPopMatrix();


	//resetting the color to white
	glColor3f(1.0f, 1.0f, 1.0f);


	//////////////////////////////////////////////////// End of Fire Place ////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//												Coconut Tree
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Next to the house 
	//Left Side
	//1st Layer 
	glPushMatrix();
		glTranslatef(-25.0, 0.80, -10.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, -10.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//2nd Layer
	glPushMatrix();
		glTranslatef(-25.0, 0.80, -20.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, -20.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//3rd Layer
	glPushMatrix();
		glTranslatef(-25.0, 0.80, -30.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, -30.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	
	//Next to the house 
	//Right Side
	//1st Layer 
	glPushMatrix();
		glTranslatef(25.0, 0.80, -10.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(20.0, 2.0, -10.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//2nd Layer
	glPushMatrix();
		glTranslatef(25.0, 0.80, -20.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(20.0, 2.0, -20.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//3rd Layer
	glPushMatrix();
		glTranslatef(25.0, 0.80, -30.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(20.0, 2.0, -30.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();



	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//											Coconut Grrove on Left side of the House (LEFT)
	//		There are multiple Coconut tree done using fucntion Coconut_Tree() and using translate , scale to set them on their position 
	//

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//1st Layer 
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 0.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 0.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 0.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 0.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 0.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 0.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();


	//2nd Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 10.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 10.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 10.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 10.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 10.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 10.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//3rd Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 20.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 20.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 20.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 20.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 20.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 20.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//4th Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 30.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 30.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 30.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 30.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 30.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 30.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();


	//5th Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 40.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 40.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 40.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 40.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 40.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 40.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();


	//6th Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 50.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 50.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 50.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 50.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 50.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 50.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//7th Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 60.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 60.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 60.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 60.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 60.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 60.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//8th Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 70.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 70.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 70.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 70.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 70.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 70.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//9th Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 80.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 80.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 80.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 80.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 80.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 80.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	//10 Layer
	glPushMatrix();
		glTranslatef(-5.0, 0.80, 90.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-10.0, 2.0, 90.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-15.0, 1.20, 90.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 2.0, 90.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-25.0, 1.20, 90.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, 1.20, 90.0);
		glScalef(1.0, 1.3, 1.0);
		Coconut_Tree();
	glPopMatrix();




	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//											Coconut Grrove on Right side of the House (RIGHT)
	//		There are multiple Coconut tree done using fucntion Coconut_Tree() and using translate , scale to set them on their position 
	//
	//
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//1st Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 0.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 0.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 0.0);
		Coconut_Tree();
	glPopMatrix();


	//2nd Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 10.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 10.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 10.0);
		Coconut_Tree();
	glPopMatrix();

	//3rd Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 20.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 20.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 20.0);
		Coconut_Tree();
	glPopMatrix();


	//4th Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 30.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 30.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 30.0);
		Coconut_Tree();
	glPopMatrix();


	//5th Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 40.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 40.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 40.0);
		Coconut_Tree();
	glPopMatrix();


	//6th Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 50.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 50.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 50.0);
		Coconut_Tree();
	glPopMatrix();


	//7th Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 60.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 60.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 60.0);
		Coconut_Tree();
	glPopMatrix();


	//8th Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 70.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 70.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 70.0);
		Coconut_Tree();
	glPopMatrix();


	//9th Layer
	glPushMatrix();
		glTranslatef(5.0, 0.80, 80.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0,80.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 80.0);
		Coconut_Tree();
	glPopMatrix();


	//10 Layer 
	glPushMatrix();
		glTranslatef(5.0, 0.80, 90.0);
		glScalef(1.0, 1.2, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.0, 2.0, 90.0);
		glScalef(1.0, 1.5, 1.0);
		Coconut_Tree();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.0, 0.0, 90.0);
		Coconut_Tree();
	glPopMatrix();

	//resetting the color to white
	glColor3f(1.0, 1.0, 1.0);



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//										Fence in front of coconut tree
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//fence outer in front of coconut groove
		//left
			glPushMatrix();
				//glTranslatef(0.0,0.0,1.0);
				Fence_Outer();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-12.0,0.0,0.0);
				Fence_Outer();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-19.0,0.0,0.0);
				Fence_Outer();
			glPopMatrix();


			//right
			glPushMatrix();
				glTranslatef(18.0,0.0,0.0);
				Fence_Outer();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(30.0,0.0,0.0);
				Fence_Outer();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(36.0,0.0,0.0);
				Fence_Outer();
			glPopMatrix();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//							Setup Outside the house in the coconut groove for resort look
//							Eg: Table , chairs , Canopy
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//*******************************************Canopy********************************************
			//Canopy
			glPushMatrix();				
				glBindTexture(GL_TEXTURE_2D, redwhite);
				glTranslatef(24.30,3.50,125.0);
				glRotatef(90, 1.0, 0.0f, 0.0f);
				glRotatef(150, 0.0, 0.0f, 1.0f);
				//glScalef(0.90f, 0.90f, 0.70f);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.05, 4, 2, 10, 10);
				

				glBindTexture(GL_TEXTURE_2D, grey);
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.12, 0.12, 8, 30, 30);

			glPopMatrix();

			glPushMatrix();				
				glBindTexture(GL_TEXTURE_2D, bluewhite);
				glTranslatef(24.30,3.50,145.0);
				glRotatef(90, 1.0, 0.0f, 0.0f);
				glRotatef(150, 0.0, 0.0f, 1.0f);
				//glScalef(0.90f, 0.90f, 0.70f);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.05, 4, 2, 10, 10);
				


				glBindTexture(GL_TEXTURE_2D, grey);
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.12, 0.12, 8, 30, 30);
		
			glPopMatrix();


			//Canopy
			glPushMatrix();				
				glBindTexture(GL_TEXTURE_2D, redwhite);
				glTranslatef(24.30,3.50,160.0);
				glRotatef(90, 1.0, 0.0f, 0.0f);
				glRotatef(150, 0.0, 0.0f, 1.0f);
				//glScalef(0.90f, 0.90f, 0.70f);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.05, 4, 2, 10, 10);
				


				glBindTexture(GL_TEXTURE_2D, grey);
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.12, 0.12, 8, 30, 30);
		
			glPopMatrix();


			//Canopy
			glPushMatrix();				
				glBindTexture(GL_TEXTURE_2D, bluewhite);
				glTranslatef(24.30,3.50,175.0);
				glRotatef(90, 1.0, 0.0f, 0.0f);
				glRotatef(150, 0.0, 0.0f, 1.0f);
				//glScalef(0.90f, 0.90f, 0.70f);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.05, 4, 2, 10, 10);
				


				glBindTexture(GL_TEXTURE_2D, grey);
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.12, 0.12, 8, 30, 30);
		
			glPopMatrix();


			//wooden box stool near canopy			
			glColor3f(0.50, 0.50, 0.50);
			glPushMatrix();
				glTranslatef(27.00, -9.60, 93.0);
				glScalef(0.70, 0.70, .70);
				WoodenBox();

				glTranslatef(5.50, 0.0, 0.0);
				WoodenBox();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(27.00, -9.60, 128.0);
				glScalef(0.70, 0.70, .70);
				WoodenBox();

				glTranslatef(5.50, 0.0, 0.0);
				WoodenBox();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(27.00, -9.60, 143.0);
				glScalef(0.70, 0.70, .70);
				WoodenBox();

				glTranslatef(5.50, 0.0, 0.0);
				WoodenBox();
			glPopMatrix();


			glColor3f(1.0, 1.0, 1.0);


			//******************************** Black Plastic Table ********************************
			glPushMatrix();				
				glTranslatef(25.0,0.0,120.0);
				glScalef(0.70f, 0.90f, 0.5f);
				Plastic_Table();
			glPopMatrix();

			//Chair
			glColor3f(0.5, 0.5, 0.5);
			glPushMatrix();				
				glTranslatef(24.20,-1.50,140.0);
				glScalef(0.90f, 0.90f, 0.70f);
				Chair();
			glPopMatrix();

			glPushMatrix();				
				glTranslatef(26.0,-1.50,145.0);
				glRotatef(-90, 0.0, 1.0f, 0.0f);
				glScalef(0.90f, 0.90f, 0.70f);
				Chair();
			glPopMatrix();

			glPushMatrix();				
				glTranslatef(22.50,-1.50,145.0);
				glRotatef(90, 0.0, 1.0f, 0.0f);
				glScalef(0.90f, 0.90f, 0.70f);
				Chair();
			glPopMatrix();

			glPushMatrix();				
				glTranslatef(24.20,-1.50,148.0);
				glRotatef(180, 0.0, -1.0f, 0.0f);
				glScalef(0.90f, 0.90f, 0.70f);
				Chair();
			glPopMatrix();


			glColor3f(1.0, 1.0, 1.0);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			//															On water track
			//
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			
			//1st Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,203.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,203.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			//2nd Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,206.50);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,206.50);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			//3rd Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,210.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,210.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//4th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,215.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,215.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			//5th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,220.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,220.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//6th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,225.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,225.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//7th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,230.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,230.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//8th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,235.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,235.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//9th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,240.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,240.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//10th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,245.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,245.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//11th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,250.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,250.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//12th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,255.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,255.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();



			//13th Track
			glPushMatrix();
				glTranslatef(-1.0,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.30,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();



			//14th Track
			glPushMatrix();
				glTranslatef(-5.0,-3.0,257.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-5.0,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//15th Track
			glPushMatrix();
				glTranslatef(-9.0,-3.0,257.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-9.0,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//16th Track
			glPushMatrix();
				glTranslatef(-13.0,-3.0,257.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-13.0,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//17th Track
			glPushMatrix();
				glTranslatef(-17.0,-3.0,257.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-17.0,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			//18th Track
			glPushMatrix();
				glTranslatef(-21.0,-3.0,257.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-21.0,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();


			//19th Track
			glPushMatrix();
				glTranslatef(-25.0,-3.0,257.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-25.0,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			//20th Track
/*			glPushMatrix();
				glTranslatef(-29.0,-3.0,257.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-29.0,-3.0,260.0);
				glRotatef(90.0, 1.0f, 0.0f, 0.0f);
				gluCylinder(quadric, 0.10, 0.10, 1.0, 30, 30);
			glPopMatrix();
*/

			//Wood on the track
			glPushMatrix();
				glTranslatef(0.0, -3.0, 260.0);
				Wood();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0.0, -3.0, 258.0);
				Wood();
			glPopMatrix();


			glPushMatrix();
				glTranslatef(0.0, -3.0, 256.0);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 254.0);
				Wood();
			glPopMatrix();


			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 252.0);
				Wood();
			glPopMatrix();


			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 250.0);
				Wood();
			glPopMatrix();


			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 248.0);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 246.0);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 244.0);
				Wood();
			glPopMatrix();


			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 242.0);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 240.0);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 238.0);
				Wood();
			glPopMatrix();


			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 236.0);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 234.0);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(0.0, -3.0, 232.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 230.0);
				Wood();
			glPopMatrix();


						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 228.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 226.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 224.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 222.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 220.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 218.0);
				Wood();
			glPopMatrix();


						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 216.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 214.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 212.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 210.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 208.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 206.0);
				Wood();
			glPopMatrix();


						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 204.0);
				Wood();
			glPopMatrix();

						
			glPushMatrix();
				glTranslatef(0.0, -3.0, 202.0);
				Wood();
			glPopMatrix();



			//2nd part of the wood

			glPushMatrix();
				glTranslatef(-2.10, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();


			glPushMatrix();
				glTranslatef(-3.40, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();


			glPushMatrix();
				glTranslatef(-4.70, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-6.0, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();
		
			glPushMatrix();
				glTranslatef(-7.30, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-8.60, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();
			
			glPushMatrix();
				glTranslatef(-9.90, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();
			
			glPushMatrix();
				glTranslatef(-11.20, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();
						
			glPushMatrix();
				glTranslatef(-12.50, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();
						
			glPushMatrix();
				glTranslatef(-13.70, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();
						
			glPushMatrix();
				glTranslatef(-15.0, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();
						
			glPushMatrix();
				glTranslatef(-16.30, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(-17.60, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(-18.90, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();

			
			glPushMatrix();
				glTranslatef(-20.20, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(1.5, 1.0, 0.80);
				Wood();
			glPopMatrix();
			

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			//												House at the end of the walk path way
			//
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			

				Shack();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//								ISLANDS ON THE SEA
	//								1) one island on the left side
	//								2) one island on the right side
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//1st island on the left side
			glPushMatrix();
				glTranslatef(-55.0, -4.0, 326.0f);
				glScalef(5.0, 0.10, 4.0);
				quadric = gluNewQuadric();
				gluSphere(quadric, 5.0, 60, 60);
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-40.0, 0.6, 265.0f);
				glScalef(1.0, 1.5, 1.0);
				Coconut_Tree();

				glTranslatef(-5.0, 0.50, 0.0f);
				Coconut_Tree();

				glTranslatef(-2.0, 0.0, -1.0f);
				Coconut_Tree();

				glTranslatef(-7.0, 0.0, 0.0f);
				Coconut_Tree();

				glTranslatef(-0.0, 0.0, 1.0f);
				Coconut_Tree();

				glTranslatef(-2.0, 0.0, -5.0f);
				Coconut_Tree();

				glTranslatef(-3.0, 0.0, -4.0f);
				Coconut_Tree();

				glTranslatef(-2.0, 0.0, 0.0f);
				Coconut_Tree();

				glTranslatef(-6.0, 0.0, 0.0f);
				Coconut_Tree();

				glTranslatef(-6.0, 0.0, -5.0f);
				Coconut_Tree();

				glTranslatef(6.0, 0.0, -6.0f);
				Coconut_Tree();

				glTranslatef(-8.0, 0.0, -5.0f);
				Coconut_Tree();

				glTranslatef(-2.0, 0.0, 4.0f);
				Coconut_Tree();

				glTranslatef(6.0, 0.0, -6.0f);
				Coconut_Tree();

				glTranslatef(5.0, 0.0, 0.0f);
				Coconut_Tree();

				glTranslatef(5.0, 0.0, 0.0f);
				Coconut_Tree();

				glTranslatef(5.0, 0.0, 0.0f);
				Coconut_Tree();

				glTranslatef(5.0, 0.0, 2.0f);
				Coconut_Tree();

				glTranslatef(5.0, 0.0, 0.0f);
				Coconut_Tree();
				
				
				glTranslatef(5.0, 0.0, 0.0f);
				Coconut_Tree();
				
			glPopMatrix();

			//Resetting white color
			glColor3f(0.50f, 0.50f, 0.50);

			//Shack on the island
			//1st Pillar
			glBindTexture(GL_TEXTURE_2D, fence_texture);
			glPushMatrix();
				glTranslatef(-44.5, 2.75, 331.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.15, 0.15, 8, 30, 30);				
			glPopMatrix();

			//2nd Pillar
			glPushMatrix();
				glTranslatef(-51.0, 2.75, 331.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.15, 0.15, 8, 30, 30);				
			glPopMatrix();

			//3rd Pillar			
			glPushMatrix();
				glTranslatef(-51.0, 2.75, 321.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.15, 0.15, 8, 30, 30);								
			glPopMatrix();

			//4th Pillar			
			glPushMatrix();
				glTranslatef(-44.5, 2.75, 321.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.15, 0.15, 8, 30, 30);				
			glPopMatrix();

			//Top Roof
			glPushMatrix();
				glTranslatef(-49.0, 2.0, 326.50);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(4.0, 1.0, 5.0);
				Wood();
			glPopMatrix();


			//Canopy
			glPushMatrix();				
				glBindTexture(GL_TEXTURE_2D, redbull);
				glTranslatef(-68.0,3.50,338.0);
				glRotatef(90, 1.0, 0.0f, 0.0f);
				glRotatef(150, 0.0, 0.0f, 1.0f);
				glRotatef(10, 0.0, -1.0f, 0.0f);
				//glScalef(0.90f, 0.90f, 0.70f);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.05, 4, 2, 10, 10);
				


				glBindTexture(GL_TEXTURE_2D, grey);
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.12, 0.12, 8, 30, 30);
		
			glPopMatrix();
/////////////////////////////////////////////////////////////////  END OF ISLANDS ON THE SEA ///////////////////////////////////////////////////////////

			glPushMatrix();
				glTranslatef(1.0, -1.0, -70.0);
				Coconut_Tree();

				glTranslatef(5.0, 0.0, 0.0);
				Coconut_Tree();

				glTranslatef(5.0, 0.0, 0.0);
				Coconut_Tree();

				glTranslatef(5.0, 0.0, 0.0);
				Coconut_Tree();

				glTranslatef(-7.0, 0.0, 0.0);
				Coconut_Tree();

				glTranslatef(-5.0, 0.0, 1.50);
				Coconut_Tree();

				glTranslatef(-4.0, 0.0, -1.0);
				Coconut_Tree();

				glTranslatef(-4.0, 0.0, 0.0);
				Coconut_Tree();

				glTranslatef(-5.0, 0.0, 0.0);
				Coconut_Tree();

				glTranslatef(10.0, 0.0, -1.0);
				Coconut_Tree();

			glPopMatrix();


	glColor3f(1.0f, 1.0f, 1.0);
glPopMatrix();					//last Pop Matrix
	


	
	
/*	//snow fall
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glVertex3f(0.0f, yaxis0, 0.0f);
		glVertex3f(0.0f, yaxis1, 0.0f);
		glVertex3f(0.0f, yaxis2, 0.0f);
		glVertex3f(0.0f, yaxis3, 0.0f);
		glVertex3f(0.0f, yaxis4, 0.0f);
		glVertex3f(0.0f, yaxis5, 0.0f);
		glVertex3f(0.0f, yaxis6, 0.0f);
		glVertex3f(0.0f, yaxis7, 0.0f);
		glVertex3f(0.0f, yaxis8, 0.0f);
		glVertex3f(0.0f, yaxis9, 0.0f);
		glVertex3f(0.0f, yaxis10, 0.0f);
		glVertex3f(0.0f, yaxis11, 0.0f);
		glVertex3f(0.0f, yaxis12, 0.0f);
		glVertex3f(0.0f, yaxis13, 0.0f);
		glVertex3f(0.0f, yaxis14, 0.0f);
		glVertex3f(0.0f, yaxis15, 0.0f);

	glEnd();
*/

/*
	//angle = angle + 0.02f;
	yaxis0 = yaxis0 - 0.0005f;
	yaxis1 = yaxis1 - 0.0005f;
	yaxis2 = yaxis2 - 0.0005f;
	yaxis3 = yaxis3 - 0.0005f;
	yaxis4 = yaxis4 - 0.0005f;
	yaxis5 = yaxis5 - 0.0005f;
	yaxis6 = yaxis6 - 0.0005f;
	yaxis7 = yaxis7 - 0.0005f;
	yaxis8 = yaxis8 - 0.0005f;
	yaxis9 = yaxis9 - 0.0005f;
	yaxis10 = yaxis10 - 0.0005f;
	yaxis11 = yaxis11 - 0.0005f;
	yaxis12 = yaxis12 - 0.0005f;
	yaxis13 = yaxis13 - 0.0005f;
	yaxis14 = yaxis14 - 0.0005f;
	yaxis15 = yaxis15 - 0.0005f;


		if (angle >= 360)
		{
			angle = 0.0f;
		}

		if (yaxis0 <= -4.0f)
		{
			yaxis0 = 4.0f;
		}

		if (yaxis1 <= -4.0f)
		{
			yaxis1 = 4.0f;
		}

		if (yaxis2 <= -4.0f)
		{
			yaxis2 = 4.0f;
		}

		if (yaxis3 <= -4.0f)
		{
			yaxis3 = 4.0f;
		}

		if (yaxis4 <= -4.0f)
		{
			yaxis4 = 4.0f;
		}

		if (yaxis5 <= -4.0f)
		{
			yaxis5 = 4.0f;
		}

		if (yaxis6 <= -4.0f)
		{
			yaxis6 = 4.0f;
		}

		if (yaxis7 <= -4.0f)
		{
			yaxis7 = 4.0f;
		}

		if (yaxis8 <= -4.0f)
		{
			yaxis8 = 4.0f;
		}

		if (yaxis9 <= -4.0f)
		{
			yaxis9 = 4.0f;
		}

		if (yaxis10 <= -4.0f)
		{
			yaxis10 = 4.0f;
		}

		if (yaxis11 <= -4.0f)
		{
			yaxis11 = 4.0f;
		}

		if (yaxis12 <= -4.0f)
		{
			yaxis12 = 4.0f;
		}

		if (yaxis13 <= -4.0f)
		{
			yaxis13 = 4.0f;
		}

		if (yaxis14 <= -4.0f)
		{
			yaxis14 = 4.0f;
		}

		if (yaxis15 <= -4.0f)
		{
			yaxis15 = 4.0f;
		}
*/


	//angle = angle + 1.0;

	/*zaxis = zaxis + 0.7;

	if (zaxis < 100)
	{
		xaxis = xaxis - 0.5;
	}
	*/

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

	angle = angle + 1.0f;
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
	glBindTexture(GL_TEXTURE_2D, snow_texture);
	glPushMatrix();	
		glBegin(GL_TRIANGLES);
		glColor3f(0.45, 0.38, 0.15f);
			glTexCoord2f(0.5f,1.0f);
			glVertex3f(0.0f, 2.50f, 0.0f);

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-5.0f, -2.50f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(5.0f, -2.50f, 0.0f);
		glEnd();
		glColor3f(1.0, 1.0, 1.0f);
	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//													Wooden Fence 
//		Description : wooden fence outside house.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Fence in front of coconut tree

void Fence_Outer(void)
{
		glPushMatrix();
		//left side fence
		glBindTexture(GL_TEXTURE_2D, fence_texture);

			//horizontal fences
			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-1.6f, -2.0f, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-16.0f, -2.0f, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-16.0f, -2.4f, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-1.6f, -2.4f, 199.0);


				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-16.0f, -3.2f, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-1.6f, -3.2f, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-1.6f, -3.6f, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-16.0f, -3.6f, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-2.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-2.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-2.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-2.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-3.0, -1.5, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-3.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-3.5, -4.0, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-3.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-4.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-4.5, -1.5, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-4.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-4.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-5.0, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-5.5, -1.5, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-5.5, -4.0, 199.0);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-5.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-6.0, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-6.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-6.5, -4.0, 199.0);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-6.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-7.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-7.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-7.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-7.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-8.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-8.5, -1.5, 199.0);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-8.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-8.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-9.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-9.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-9.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-9.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-10.0, -1.5, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-10.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-10.5, -4.0, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-10.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-11.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-11.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-11.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-11.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-12.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-12.5, -1.5, 199.0);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-12.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-12.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-13.0, -1.5, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-13.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-13.5, -4.0, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-13.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-14.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-14.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-14.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-14.0, -4.0, 199.0);
			glEnd();

			glBegin(GL_QUADS);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(-15.0, -1.5, 199.0);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(-15.5, -1.5, 199.0);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(-15.5, -4.0, 199.0);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(-15.0, -4.0, 199.0);
			glEnd();
		
	glPopMatrix();

}


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


void Plastic_Table()
{
	glPushMatrix();					//Push Matrix		
		glTranslatef(-1.0f, -2.0f, -5.0f);
		glBindTexture(GL_TEXTURE_2D, black_plastic);
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//											Tea Cup 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Teacup()
{
	glPushMatrix();

		glPushMatrix();
			glTranslatef(-9.0f, -0.30f, 46.50f);
			glRotatef(90.0f, 1.0f, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, marble);
			quadric = gluNewQuadric();
			gluQuadricTexture(quadric, TRUE);
			gluCylinder(quadric, 0.13, 0.13, 0.35, 30, 30);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-9.15f, -0.50f, 46.50f);
			glRotatef(22.50f, 0.0f, 1.0, 0.0);
			glBindTexture(GL_TEXTURE_2D, marble);
			quadric = gluNewQuadric();
			gluQuadricTexture(quadric, TRUE);
			gluCylinder(quadric, 0.11, 0.11, 0.05, 30, 30);
		glPopMatrix();

	glPopMatrix();


}
//////////////////////////////////////////////////////// End of Tea Cup ////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//											Coconut Tree 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Coconut_Tree(void)
{
	//Function Declaration 
	void Coconut_Leaf(void);


	//Local variables
	GLUquadric* C_Tree[15];

	static GLfloat tree_angle = 0.0f;

	glBindTexture(GL_TEXTURE_2D, fence_texture);
	glColor3f(0.90, 0.65, 0.45);

	//////////////////////////////////////////////////////long bark of the coconut tree///////////////////////////////////////

	glPushMatrix();
		glTranslatef(0.0, -3.0, 75.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		C_Tree[0] = gluNewQuadric();
		//gluQuadricTexture(C_Tree[0], TRUE);
		gluCylinder(C_Tree[0], 0.4, 0.5, 1.0, 30, 30);

		/*To move the tree giving the wind effect use rotate call*/
		glRotatef(tree_angle, 0.0, 1.0, 0.0);

		//2nd on stack		
		glPushMatrix();
			glTranslatef(0.0, 0.0, -0.80);		//here z is y because it rotated.
			C_Tree[1] = gluNewQuadric();
			gluCylinder(C_Tree[1], 0.35, 0.4, 0.80, 30, 30);
		glPopMatrix();

		//3rd on stack
		glPushMatrix();
			glTranslatef(0.0, 0.0, -1.60);
			C_Tree[2] = gluNewQuadric();
			gluCylinder(C_Tree[2], 0.30, 0.35, 0.80, 30, 30);
		glPopMatrix();

		//4th on stack 
		glPushMatrix();
			glTranslatef(0.0, 0.0, -2.40);
			C_Tree[3] = gluNewQuadric();
			gluCylinder(C_Tree[3], 0.25, 0.30, 0.80, 30, 30);
		glPopMatrix();

		//5th on stack
		glPushMatrix();
			glTranslatef(0.0, 0.0, -3.20);
			C_Tree[4] = gluNewQuadric();
			gluCylinder(C_Tree[4], 0.20, 0.25, 0.80, 30, 30);
		glPopMatrix();

		//6th on stack
		glPushMatrix();
			glTranslatef(0.0, 0.0, -4.0);
			C_Tree[5] = gluNewQuadric();
			gluCylinder(C_Tree[5], 0.15, 0.20, 0.80, 30, 30);
		glPopMatrix();

		//7th on stack
		glPushMatrix();
			glTranslatef(0.0, 0.0, -4.80);
			C_Tree[6] = gluNewQuadric();
			gluCylinder(C_Tree[6], 0.10, 0.15, 0.80, 30, 30);
		glPopMatrix();

		//8th on stack
		glPushMatrix();
			glTranslatef(0.0, 0.0, -5.60);
			C_Tree[7] = gluNewQuadric();
			gluCylinder(C_Tree[7], 0.1, 0.10, 0.80, 30, 30);
		glPopMatrix();

		//9th on stack
		glPushMatrix();
			glTranslatef(0.0, 0.0, -10.60);
			C_Tree[8] = gluNewQuadric();
			gluCylinder(C_Tree[8], 0.10, 0.10, 5.0, 30, 30);
		glPopMatrix();

		//Rotate for leaf
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glPushMatrix();
			Coconut_Leaf();
		glPopMatrix();

		glPushMatrix();
			glRotatef(30.0, 0.0, 1.0, 0.0);
			Coconut_Leaf();
		glPopMatrix();

		glPushMatrix();
			glRotatef(90.0, 0.0, 1.0, 0.0);
			Coconut_Leaf();
		glPopMatrix();


		glPushMatrix();
			glRotatef(150.0, 0.0, 1.0, 0.0);
			Coconut_Leaf();
		glPopMatrix();

		glPushMatrix();
			glRotatef(210.0, 0.0, 1.0, 0.0);
			Coconut_Leaf();
		glPopMatrix();

		glPushMatrix();
			glRotatef(270.0, 0.0, 1.0, 0.0);
			Coconut_Leaf();
		glPopMatrix();

		glPushMatrix();
			glRotatef(320.0, 0.0, 1.0, 0.0);
			Coconut_Leaf();
		glPopMatrix();

		//Coconut on the tree
		glPushMatrix();
			C_Tree[9] = gluNewQuadric();
			glTranslatef(0.0, 10.00, 0.50);
			glColor3f(0.0, 0.6, 0.0);
			glScalef(0.4, 0.60, 0.4);
			gluSphere(C_Tree[9], 0.50, 30, 30);


			C_Tree[10] = gluNewQuadric();
			glTranslatef(1.0, 00.00, -0.50);
			gluSphere(C_Tree[10], 0.50, 30, 30);

			C_Tree[11] = gluNewQuadric();
			glTranslatef(-2.0, 00.00, -0.50);
			gluSphere(C_Tree[11], 0.50, 30, 30);

			C_Tree[12] = gluNewQuadric();
			glTranslatef(1.0, 00.00, -1.0);
			gluSphere(C_Tree[12], 0.50, 30, 30);
		glPopMatrix();

	glPopMatrix();//end of 1st push stack push pop

	////////////////////////////////////////////////end of long bark of the coconut tree///////////////////////////////////////

	//	tree_angle = tree_angle + 0.02;
		if (tree_angle >= 10)
		{
			tree_angle = 10;
		}

}


///////////////////////////////////////////////// Coconut_Leaf //////////////////////////////////////////////////////////////
void Coconut_Leaf()
{
	glTranslatef(0.0, 10.66, 0.0);
	glColor3f(0.0f, 1.0, 0.0);
	glLineWidth(1.8);
	glBegin(GL_LINES);
		//coconut tree leaf top sekeleton
		//1st
		glVertex3f(0.0, -0.06, 0.0);
		glVertex3f(-2.0, -0.50, 0.0);

		//2nd
		glVertex3f(-2.0, -0.50, 0.0);
		glVertex3f(-4.0, -1.20, 0.0);

		//3rd
		glVertex3f(-4.0, -1.20, 0.0);
		glVertex3f(-4.60, -2.00, 0.0);

		//Leaves

		//1st joint leaves

		//Front Leaves

		glVertex3f(-0.20, -0.10, 0.0);
		glVertex3f(-0.20, -1.20, 0.50);

		glVertex3f(-0.30, -0.12, 0.0);
		glVertex3f(-0.30, -1.22, 0.50);

		glVertex3f(-0.40, -0.14, 0.0);
		glVertex3f(-0.40, -1.24, 0.50);

		glVertex3f(-0.50, -0.16, 0.0);
		glVertex3f(-0.50, -1.26, 0.50);

		glVertex3f(-0.60, -0.18, 0.0);
		glVertex3f(-0.60, -1.28, 0.50);

		glVertex3f(-0.70, -0.20, 0.0);
		glVertex3f(-0.70, -1.30, 0.50);

		glVertex3f(-0.80, -0.22, 0.0);
		glVertex3f(-0.80, -1.32, 0.50);

		glVertex3f(-0.90, -0.24, 0.0);
		glVertex3f(-0.90, -1.34, 0.50);

		glVertex3f(-1.00, -0.26, 0.0);
		glVertex3f(-1.00, -1.36, 0.50);

		glVertex3f(-1.10, -0.28, 0.0);
		glVertex3f(-1.10, -1.38, 0.50);

		glVertex3f(-1.20, -0.30, 0.0);
		glVertex3f(-1.20, -1.40, 0.50);

		glVertex3f(-1.30, -0.32, 0.0);
		glVertex3f(-1.30, -1.42, 0.50);

		glVertex3f(-1.40, -0.36, 0.0);
		glVertex3f(-1.40, -1.44, 0.50);

		glVertex3f(-1.50, -0.38, 0.0);
		glVertex3f(-1.50, -1.46, 0.50);

		glVertex3f(-1.60, -0.40, 0.0);
		glVertex3f(-1.60, -1.48, 0.50);

		glVertex3f(-1.70, -0.42, 0.0);
		glVertex3f(-1.70, -1.5, 0.50);

		glVertex3f(-1.80, -0.44, 0.0);
		glVertex3f(-1.80, -1.52, 0.50);

		glVertex3f(-1.90, -0.46, 0.0);
		glVertex3f(-1.90, -1.54, 0.50);

		glVertex3f(-2.0, -0.48, 0.0);
		glVertex3f(-2.0, -1.56, 0.50);

		//back leaves 
		glVertex3f(-0.20, -0.10, 0.0);
		glVertex3f(-0.20, -1.20, -0.50);

		glVertex3f(-0.30, -0.12, 0.0);
		glVertex3f(-0.30, -1.22, -0.50);

		glVertex3f(-0.40, -0.14, 0.0);
		glVertex3f(-0.40, -1.24, -0.50);

		glVertex3f(-0.50, -0.16, 0.0);
		glVertex3f(-0.50, -1.26, -0.50);

		glVertex3f(-0.60, -0.18, 0.0);
		glVertex3f(-0.60, -1.28, -0.50);

		glVertex3f(-0.70, -0.20, 0.0);
		glVertex3f(-0.70, -1.30, -0.50);

		glVertex3f(-0.80, -0.22, 0.0);
		glVertex3f(-0.80, -1.32, -0.50);

		glVertex3f(-0.90, -0.24, 0.0);
		glVertex3f(-0.90, -1.34, -0.50);

		glVertex3f(-1.00, -0.26, 0.0);
		glVertex3f(-1.00, -1.36, -0.50);

		glVertex3f(-1.10, -0.28, 0.0);
		glVertex3f(-1.10, -1.38, -0.50);

		glVertex3f(-1.20, -0.30, 0.0);
		glVertex3f(-1.20, -1.40, -0.50);

		glVertex3f(-1.30, -0.32, 0.0);
		glVertex3f(-1.30, -1.42, -0.50);

		glVertex3f(-1.40, -0.36, 0.0);
		glVertex3f(-1.40, -1.44, -0.50);

		glVertex3f(-1.50, -0.38, 0.0);
		glVertex3f(-1.50, -1.46, -0.50);

		glVertex3f(-1.60, -0.40, 0.0);
		glVertex3f(-1.60, -1.48, -0.50);

		glVertex3f(-1.70, -0.42, 0.0);
		glVertex3f(-1.70, -1.5, -0.50);

		glVertex3f(-1.80, -0.44, 0.0);
		glVertex3f(-1.80, -1.52, -0.50);

		glVertex3f(-1.90, -0.46, 0.0);
		glVertex3f(-1.90, -1.54, -0.50);

		glVertex3f(-2.0, -0.48, 0.0);
		glVertex3f(-2.0, -1.56, -0.50);



		//2nd joint leaves

		glVertex3f(-2.10, -0.53, 0.0);
		glVertex3f(-2.10, -1.58, 0.50);

		glVertex3f(-2.20, -0.55, 0.0);
		glVertex3f(-2.20, -1.60, 0.50);

		glVertex3f(-2.30, -0.57, 0.0);
		glVertex3f(-2.30, -1.62, 0.50);

		glVertex3f(-2.40, -0.64, 0.0);
		glVertex3f(-2.40, -1.64, 0.50);

		glVertex3f(-2.50, -0.68, 0.0);
		glVertex3f(-2.50, -1.66, 0.50);

		glVertex3f(-2.60, -0.70, 0.0);
		glVertex3f(-2.60, -1.68, 0.50);

		glVertex3f(-2.70, -0.73, 0.0);
		glVertex3f(-2.70, -1.70, 0.50);

		glVertex3f(-2.80, -0.77, 0.0);
		glVertex3f(-2.80, -1.72, 0.50);

		glVertex3f(-2.90, -0.80, 0.0);
		glVertex3f(-2.90, -1.74, 0.50);

		glVertex3f(-3.00, -0.84, 0.0);
		glVertex3f(-3.00, -1.76, 0.50);

		glVertex3f(-3.10, -0.88, 0.0);
		glVertex3f(-3.10, -1.78, 0.50);

		glVertex3f(-3.20, -0.91, 0.0);
		glVertex3f(-3.20, -1.80, 0.50);

		glVertex3f(-3.30, -0.94, 0.0);
		glVertex3f(-3.30, -1.82, 0.50);

		glVertex3f(-3.40, -0.98, 0.0);
		glVertex3f(-3.40, -1.84, 0.50);

		glVertex3f(-3.50, -1.02, 0.0);
		glVertex3f(-3.50, -1.86, 0.50);

		glVertex3f(-3.60, -1.05, 0.0);
		glVertex3f(-3.60, -1.88, 0.50);

		glVertex3f(-3.70, -1.08, 0.0);
		glVertex3f(-3.70, -1.9, 0.50);

		glVertex3f(-3.80, -1.11, 0.0);
		glVertex3f(-3.80, -1.92, 0.50);

		glVertex3f(-3.90, -1.15, 0.0);
		glVertex3f(-3.90, -1.94, 0.50);

		glVertex3f(-4.0, -1.18, 0.0);
		glVertex3f(-4.0, -1.96, 0.50);

		glVertex3f(-4.10, -1.36, 0.0);
		glVertex3f(-4.10, -1.98, 0.50);

		glVertex3f(-4.20, -1.45, 0.0);
		glVertex3f(-4.20, -2.0, 0.50);

		glVertex3f(-4.30, -1.60, 0.0);
		glVertex3f(-4.30, -2.0, 0.50);

		glVertex3f(-4.40, -1.78, 0.0);
		glVertex3f(-4.40, -2.0, 0.50);

		glVertex3f(-4.50, -1.88, 0.0);
		glVertex3f(-4.50, -2.0, 0.50);

		glVertex3f(-4.60, -1.98, 0.0);
		glVertex3f(-4.60, -2.06, 0.50);

		//Back Leaves

		glVertex3f(-2.10, -0.53, 0.0);
		glVertex3f(-2.10, -1.58, -0.50);

		glVertex3f(-2.20, -0.55, 0.0);
		glVertex3f(-2.20, -1.60, -0.50);

		glVertex3f(-2.30, -0.57, 0.0);
		glVertex3f(-2.30, -1.62, -0.50);

		glVertex3f(-2.40, -0.64, 0.0);
		glVertex3f(-2.40, -1.64, -0.50);

		glVertex3f(-2.50, -0.68, 0.0);
		glVertex3f(-2.50, -1.66, -0.50);

		glVertex3f(-2.60, -0.70, 0.0);
		glVertex3f(-2.60, -1.68, -0.50);

		glVertex3f(-2.70, -0.73, 0.0);
		glVertex3f(-2.70, -1.70, -0.50);

		glVertex3f(-2.80, -0.77, 0.0);
		glVertex3f(-2.80, -1.72, -0.50);

		glVertex3f(-2.90, -0.80, 0.0);
		glVertex3f(-2.90, -1.74, -0.50);

		glVertex3f(-3.00, -0.84, 0.0);
		glVertex3f(-3.00, -1.76, -0.50);

		glVertex3f(-3.10, -0.88, 0.0);
		glVertex3f(-3.10, -1.78, -0.50);

		glVertex3f(-3.20, -0.91, 0.0);
		glVertex3f(-3.20, -1.80, -0.50);

		glVertex3f(-3.30, -0.94, 0.0);
		glVertex3f(-3.30, -1.82, -0.50);

		glVertex3f(-3.40, -0.98, 0.0);
		glVertex3f(-3.40, -1.84, -0.50);

		glVertex3f(-3.50, -1.02, 0.0);
		glVertex3f(-3.50, -1.86, -0.50);

		glVertex3f(-3.60, -1.05, 0.0);
		glVertex3f(-3.60, -1.88, -0.50);

		glVertex3f(-3.70, -1.08, 0.0);
		glVertex3f(-3.70, -1.9, -0.50);

		glVertex3f(-3.80, -1.11, 0.0);
		glVertex3f(-3.80, -1.92, -0.50);

		glVertex3f(-3.90, -1.15, 0.0);
		glVertex3f(-3.90, -1.94, -0.50);

		glVertex3f(-4.0, -1.18, 0.0);
		glVertex3f(-4.0, -1.96, -0.50);

		glVertex3f(-4.10, -1.36, 0.0);
		glVertex3f(-4.10, -1.98, -0.50);

		glVertex3f(-4.20, -1.45, 0.0);
		glVertex3f(-4.20, -2.0, -0.50);

		glVertex3f(-4.30, -1.60, 0.0);
		glVertex3f(-4.30, -2.0, -0.50);

		glVertex3f(-4.40, -1.78, 0.0);
		glVertex3f(-4.40, -2.0, -0.50);

		glVertex3f(-4.50, -1.88, 0.0);
		glVertex3f(-4.50, -2.0, -0.50);

		glVertex3f(-4.60, -1.98, 0.0);
		glVertex3f(-4.60, -2.06, -0.50);

	glEnd();

}

	/////////////////////////////////////////////////End Coconut_Leaf //////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////End of Coconut Tree ////////////////////////////////////////////////////////

void Wood(void)
{
	
				glBegin(GL_QUADS);
					//Top
					glTexCoord2f(1.0, 1.0);
					glVertex3f(1.5, 0.0, -0.50);
					glTexCoord2f(1.0, 0.0);
					glVertex3f(-1.20, 0.0, -0.50);
					glTexCoord2f(0.0, 0.0);
					glVertex3f(-1.20, 0.0, 1.0);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(1.5, 0.0, 1.0);

					//Front
					glTexCoord2f(1.0, 1.0);
					glVertex3f(1.5, 0.0, 1.0);
					glTexCoord2f(1.0, 0.0);
					glVertex3f(-1.20, 0.0, 1.0);
					glTexCoord2f(0.0, 0.0);
					glVertex3f(-1.20, -0.10, 1.0);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(1.5, -0.10, 1.0);

					//Back
					glTexCoord2f(1.0, 1.0);
					glVertex3f(1.5, 0.0, -0.50);
					glTexCoord2f(1.0, 0.0);
					glVertex3f(-1.20, 0.0, -0.50);
					glTexCoord2f(0.0, 0.0);
					glVertex3f(-1.20, -0.10, -0.50);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(1.5, -0.10, -0.50);

					//Right
					glTexCoord2f(1.0, 1.0);
					glVertex3f(1.5, 0.0, -0.50);
					glTexCoord2f(1.0, 0.0);
					glVertex3f(1.50, 0.0, 1.00);
					glTexCoord2f(0.0, 0.0);
					glVertex3f(1.50, -0.10, 1.0);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(1.5, -0.10, -0.50);

					//Left
					glTexCoord2f(1.0, 1.0);
					glVertex3f(-1.2, 0.0, 1.0);
					glTexCoord2f(1.0, 0.0);
					glVertex3f(-1.20, 0.0, -0.50);
					glTexCoord2f(0.0, 0.0);
					glVertex3f(-1.20, -0.10, -0.50);
					glTexCoord2f(0.0, 1.0);
					glVertex3f(-1.2, -0.10, 1.0);

				glEnd();

}


void Shack(void)
{
	
			//Big Wood on the end bottom 
			glPushMatrix();
				glTranslatef(-25.0, -3.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(3.0, 1.0, 5.0);
				Wood();
			glPopMatrix();

			//Four pillar on the bottom of the house

			//1st Pillar
			glPushMatrix();
				glTranslatef(-20.5, 1.0, 255.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.1, 0.1, 4, 30, 30);				
			glPopMatrix();

			//2nd Pillar
			glPushMatrix();
				glTranslatef(-27.0, 1.0, 255.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.1, 0.1, 4, 30, 30);				
			glPopMatrix();

			//3rd Pillar			
			glPushMatrix();
				glTranslatef(-27.0, 1.0, 262.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.1, 0.1, 4, 30, 30);				
			glPopMatrix();

			//4th Pillar			
			glPushMatrix();
				glTranslatef(-20.5, 1.0, 262.0);
				glRotatef(90.0, 1.0, 0.0, 0.0);
				quadric = gluNewQuadric();
				gluQuadricTexture(quadric, TRUE);
				gluCylinder(quadric, 0.1, 0.1, 4, 30, 30);				
			glPopMatrix();

			//Top Roof
			glPushMatrix();
				glTranslatef(-25.0, 1.0, 259.0);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(3.0, 1.0, 5.0);
				Wood();
			glPopMatrix();

			//Table on the end 
			glPushMatrix();				
				glTranslatef(-23.0,-1.25,242.0);
				glScalef(0.350f, 0.350f, 0.35f);
				Plastic_Table();
			glPopMatrix();

			//chair 

			glPushMatrix();
				glTranslatef(-23.30, -1.5, 257.0);
				glScalef(0.80f, 0.50f, 0.50f);
				Chair();
			glPopMatrix();


			glPushMatrix();
				glTranslatef(-24.0, -1.5, 259.50);
				glRotatef(90.0, 0.0, 1.0, 0.0);
				glScalef(0.80f, 0.50f, 0.50f);
				Chair();
			glPopMatrix();


			glPushMatrix();
				glTranslatef(-22.50, -1.5, 259.50);
				glRotatef(90.0, 0.0, -1.0, 0.0);
				glScalef(0.80f, 0.50f, 0.50f);
				Chair();
			glPopMatrix();


			glPushMatrix();
				glTranslatef(-23.30, -1.5, 261.50);
				glRotatef(180.0, 0.0, 1.0, 0.0);
				glScalef(0.80f, 0.50f, 0.50f);
				Chair();
			glPopMatrix();


}

void Pillow(void)
{
			//pillow
		glBegin(GL_QUADS);			
			
			//bottom
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-2.50f, 10.03f, 45.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-4.80f, 10.03f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-4.80f, 10.03f, 46.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-2.50f, 10.03f, 46.90f);

			//top
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-2.50f, 10.53f, 45.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-4.80f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-4.80f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-2.50f, 10.53f, 46.90f);

			//front
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-2.50f, 10.53f, 46.90f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-4.80f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-4.80f, 10.03f, 46.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-2.50f, 10.03f, 46.90f);

			//back
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-2.50f, 10.53f, 45.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-4.80f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-4.80f, 10.03f, 45.0f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-2.50f, 10.03f, 45.0f);

			//right
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-2.50f, 10.53f, 45.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-2.50f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-2.50f, 10.03f, 46.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-2.50f, 10.03f, 45.0f);

			//left
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-4.80f, 10.53f, 46.90f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-4.80f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-4.80f, 10.03f, 45.0f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-4.80f, 10.03f, 46.90f);

		glEnd();


}

void WoodenBox(void)
{
		glBindTexture(GL_TEXTURE_2D, fence_texture);
		glBegin(GL_QUADS);						
			//bottom
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-5.50f, 8.03f, 45.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-7.50f, 8.03f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 8.03f, 46.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-5.50f, 8.03f, 46.90f);

			//top
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-5.50f, 10.53f, 45.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-5.50f, 10.53f, 46.90f);

			//front
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-5.50f, 10.53f, 46.90f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 8.03f, 46.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-5.50f, 8.03f, 46.90f);

			//back
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-5.50f, 10.53f, 45.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 8.03f, 45.0f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-5.50f, 8.03f, 45.0f);

			//right
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-5.50f, 10.53f, 45.0f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-5.50f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-5.50f, 8.03f, 46.90f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-5.50f, 8.03f, 45.0f);

			//left
			glTexCoord2f(10.0, 10.0);
			glVertex3f(-7.50f, 10.53f, 46.90f);
			glTexCoord2f(10.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 8.03f, 45.0f);
			glTexCoord2f(0.0, 10.0);
			glVertex3f(-7.50f, 8.03f, 46.90f);

		glEnd();


}

void Teapoy(void)
{
		glBindTexture(GL_TEXTURE_2D, sidewall);
		glBegin(GL_QUADS);						
			//top
			glColor3f(0.70, 0.30, 0.30);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-2.50f, 10.53f, 45.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-2.50f, 10.53f, 46.90f);
		glEnd();
		
		//resetting
		glColor3f(1.0, 1.0, 1.0);

		glBindTexture(GL_TEXTURE_2D, fence_texture);
		glBegin(GL_QUADS);
			//bottom
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-2.50f, 10.30f, 45.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-7.50f, 10.30f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 10.30f, 46.90f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-2.50f, 10.30f, 46.90f);

			//front
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-2.50f, 10.53f, 46.90f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 10.30f, 46.90f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-2.50f, 10.30f, 46.90f);

			//back
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-2.50f, 10.53f, 45.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 10.30f, 45.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-2.50f, 10.30f, 45.0f);

			//right
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-2.50f, 10.53f, 45.0f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-2.50f, 10.53f, 46.90f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-2.50f, 10.30f, 46.90f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-2.50f, 10.30f, 45.0f);

			//left
			glTexCoord2f(1.0, 1.0);
			glVertex3f(-7.50f, 10.53f, 46.90f);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(-7.50f, 10.53f, 45.0f);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(-7.50f, 10.30f, 45.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(-7.50f, 10.30f, 46.90f);

		glEnd();

		//FR - Front Right
		glBindTexture(GL_TEXTURE_2D, fence_texture);
		glTranslatef(-3.0, 10.5, 46.50);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.1, 0.1, 2.5, 30, 30);
		gluQuadricTexture(quadric, TRUE);

		//FL - Front Left
		glTranslatef(-4.0, 0.0, 0.0);
		//glRotatef(90.0, 1.0, 0.0, 0.0);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.1, 0.1, 2.5, 30, 30);
		gluQuadricTexture(quadric, TRUE);


		//BL - Back Left
		glTranslatef(0.0, -1.45, 0.0);
		//glRotatef(90.0, 1.0, 0.0, 0.0);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.1, 0.1, 2.5, 30, 30);
		gluQuadricTexture(quadric, TRUE);

		//BR - Back Right
		glTranslatef(4.0, 0.0, 0.0);
		//glRotatef(90.0, 1.0, 0.0, 0.0);
		quadric = gluNewQuadric();
		gluCylinder(quadric, 0.1, 0.1, 2.5, 30, 30);
		gluQuadricTexture(quadric, TRUE);





}

void DrawFont(void)
{
	//code
	glLoadIdentity();

	glTranslatef(-1.80f, 0.0f, -910.0f);
	//	glScalef(2.5f, 2.5f, 2.5f);
	glColor3f(0.50, 0.50, 0.50);

	glRasterPos3f(0.0f, 1.0f, 900.0);
	glListBase(uiFontList);
	glCallLists(15, GL_UNSIGNED_BYTE, "ASTROMEDICOMP'S");

	glRasterPos3f(0.05f, 0.38f, 900.0f);
	glCallLists(14, GL_UNSIGNED_BYTE, "FRAGMENT GROUP");

	glRasterPos3f(0.68f, -0.10f, 900.0f);
	glCallLists(8, GL_UNSIGNED_BYTE, "PRESENTS");

	glColor3f(0.70, 0.50, 0.30);
	glRasterPos3f(0.50f, -1.50f, 900.0);
	glCallLists(14, GL_UNSIGNED_BYTE, "Fantasy Resort");
}


void DrawFont1()
{


	static GLuint iCnt = 0;

	glLoadIdentity();

	glTranslatef(-1.80f, 0.0f, -910.0f);

	if (iCnt > 100 && iCnt < 200)
	{
		glRasterPos3f(0.30f, 2.0f, 900.0);
		glListBase(uiFontList);
		glCallLists(15, GL_UNSIGNED_BYTE, "Technology Used");

		glRasterPos3f(-1.80f, 1.20f, 900.0);
		glCallLists(43, GL_UNSIGNED_BYTE, "Win32 SDK , OpenGL Fixed Function Pipeline.");

		glRasterPos3f(-0.80f, 0.70f, 900.0);
		glCallLists(30, GL_UNSIGNED_BYTE, "Programming Language : C , C++");

		glTranslatef(0.30f, -1.0f, 0.0f);

		glRasterPos3f(0.40f, 0.50f, 900.0);
		glCallLists(8, GL_UNSIGNED_BYTE, "ABSTRACT");

		glRasterPos3f(-2.50f, 0.0f, 900.0);
		glCallLists(50, GL_UNSIGNED_BYTE, "This demo was developed on only basic concepts , ");


		glRasterPos3f(-2.30f, -0.50f, 900.0);
		glCallLists(50, GL_UNSIGNED_BYTE, "taught in class, by using lines , triangle , quads");

		glRasterPos3f(-1.80f, -1.0f, 900.0);
		glCallLists(39, GL_UNSIGNED_BYTE, "and GLU function for Sphere , Cylinder.");

		glRasterPos3f(-0.60f, -1.50f, 900.0);
		glCallLists(23, GL_UNSIGNED_BYTE, "Music : Unigine Tropics");

	}

	if (iCnt > 200 && iCnt < 300)
	{
		glRasterPos3f(1.0f, 1.50f, 900.0);
		glListBase(uiFontList);
		glCallLists(7, GL_UNSIGNED_BYTE, "CREDITS");

		glRasterPos3f(-2.30f, 0.60f, 900.0);
		glCallLists(50, GL_UNSIGNED_BYTE, "Special thanks to My Family , Team AstroMediComp ,");

		glRasterPos3f(-2.20f, 0.0f, 900.0);
		glCallLists(47, GL_UNSIGNED_BYTE, "My Group Leader - Ajay Ambure & Fragment Group.");
	}

	if (iCnt > 300 && iCnt < 400)
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

	if (iCnt > 400)
		iCnt = 399;


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
