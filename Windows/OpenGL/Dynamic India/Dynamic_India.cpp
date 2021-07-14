/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Name : ROHAN SATYAMURTHY
//	Program : Dynamic India
//	Date : 24 Sept 2020
//	Modified Date : 30 Sept 2020	
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//header file inclusion
#include<windows.h>				//standard windows file
#include<stdio.h>				//stadard C files
#include<stdlib.h>				//for malloc , or other libraries function
#include<gl/GL.h>				//Graphics Library header file
#include<gl/GLU.h>				//OpenGL utility library
#include"IconHeader.h"			//ICON header
#include"Library.h"				//My header file

//library include
#pragma comment(lib,"opengl32.lib")		//OpenGL files
#pragma comment(lib,"glu32.lib")		//GLU files
#pragma comment(lib,"Winmm.lib")		//Play Sound

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

//For Animation Toggling.
int Toggle;

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
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

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
		MessageBox(hwnd,TEXT("Press 'F' or 'f' for fullscreen\nPress esc or Spacebar or 'X' or 'x' for EXIT.\n\nPress any key to start.\n\nEnjoy the Demo !!\n\n"),TEXT("Help"),MB_OK);
		PlaySound(TEXT("Dynamic"), NULL, SND_ASYNC | SND_RESOURCE);
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

		case 'R':
		case 'r':
			Toggle = 1;
			break;

		case 'A':
		case 'a':
			Toggle = 2;
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
		MessageBox(hwnd, TEXT("Developed By : Rohan Satyamurthy.\n\n**under the guidance of my Guru Dr. Vijay Gokhale Sir.**\n\n\n\nTechnology :\nWin32 SDK.\nOpenGL Fixed Function Pipeline.\nProgramming language used 'C'.\n\n"),TEXT("Info"),MB_OK);
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

	//set clear color
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);

	//warmup call to resize
	resize(WIN_WIDTH, WIN_HEIGHT);

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

	gluPerspective(45.0f, ((GLfloat)width/(GLfloat)height),0.1f,100.0f);
}
/*
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-5.0f);
	Jet();
	BehindPlaneFlag();

		SwapBuffers(ghdc);
}
*/


void display(void)
{
	//Function prototype
	void Letter_I(void);		//Function for Letter I included in header file , library.h
	void Letter_N(void);		//Function for Letter N included in header file , library.h
	void Letter_D(void);		//Function for Letter D included in header file , library.h
	void Letter_A(void);		//Function for Letter A included in header file , library.h
	void Jet(void);				//Function for Jet Fighter included in header file , library.h

	//Local variable
	static GLfloat ICnt  = 2.5f;	//Variable I for INDIA		
	static GLfloat NCnt  = 1.5f;	//Variable N for INDIA		
	static GLfloat DCnt  = 20.0f;	//Variable D for INDIA		
	static GLfloat IICnt = 1.5f;	//Variable I for INDIA		
	static GLfloat ACnt  = 2.14f;	//Variable A for INDIA		
	static GLfloat Fangle = -6.0f;	//Fighter Plane variable
	static GLfloat Fyaxis = 3.0f;	//Fighter Plane variable
	static GLfloat FyaxisN = 0.0f;	//Fighter Plane variable



	static GLbyte IFlag = FALSE;	//Flag Variable for INDIA		
	static GLbyte NFlag = FALSE;	//Flag Variable for INDIA		
	static GLbyte DFlag = FALSE;	//Flag Variable for INDIA		
	static GLbyte IIFlag = FALSE;	//Flag Variable for INDIA		
	static GLbyte AFlag = FALSE;	//Flag Variable for INDIA		
	static GLbyte Plane = FALSE;	//Flag Fighter Plane variable
	static GLbyte RunWayFlying = FALSE;
	
	static int iMagic = 0;		//The Variable used for transition from one transition to the other.
	GLint iZaxis = -5.0f;			//Depth setting can be done from here.

		
	//Runway Flying.
	if (RunWayFlying == FALSE && iMagic == 0)
	{

		//code for runway setup.
		//local variable
		//static GLfloat angle = 0.02f;
		
		static GLfloat T_x = -10.50f;
		static GLfloat T_y = 6.0f;
		static GLfloat T_z = -20.0f;

		static GLfloat RunWayLights_X = 0.0f;
		static GLfloat RunWayLights_Y = 0.0f;

		static GLint iCnt = 0;

		//code
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


		glTranslatef(0, 0, -20.0);

		glColor3f(1.0f, 1.0f, 1.0f);
		//*******************************************Run Way Lines*******************************************************
		glBegin(GL_LINES);

		glVertex3f(-10.0f, 8.0f, 0.0f);
		glVertex3f(15.0f, -12.0f, 0.0f);

		glVertex3f(-13.0f, 5.35f, 0.0f);
		glVertex3f(15.0f, -17.0f, 0.0f);

		glVertex3f(-10.0f, 8.0f, 0.0f);
		glVertex3f(-13.0f, 5.35f, 0.0f);



		//side line top
		glVertex3f(4.50f, -3.20f, 0.0f);
		glVertex3f(8.50f, 0.80f, 0.0f);

		//Side Line bottom
		glVertex3f(1.00f, -6.4f, 0.0f);
		glVertex3f(-.50f, -8.0f, 0.0f);
		glEnd();

		//********************************************Run way lights*******************************************************
		glColor3f(RunWayLights_X, RunWayLights_Y, 0.0f);
		glPointSize(8);
		glBegin(GL_POINTS);
		//Top Lights
		glVertex3f(-9.50f, 8.00f, 0.0f);
		glVertex3f(-9.00f, 7.60f, 0.0f);
		glVertex3f(-8.50f, 7.20f, 0.0f);
		glVertex3f(-8.00f, 6.80f, 0.0f);
		glVertex3f(-7.50f, 6.40f, 0.0f);
		glVertex3f(-7.00f, 6.00f, 0.0f);
		glVertex3f(-6.50f, 5.60f, 0.0f);
		glVertex3f(-6.00f, 5.20f, 0.0f);
		glVertex3f(-5.50f, 4.80f, 0.0f);
		glVertex3f(-5.00f, 4.40f, 0.0f);
		glVertex3f(-4.50f, 4.00f, 0.0f);
		glVertex3f(-4.00f, 3.60f, 0.0f);
		glVertex3f(-3.50f, 3.20f, 0.0f);
		glVertex3f(-3.00f, 2.80f, 0.0f);
		glVertex3f(-2.50f, 2.40f, 0.0f);
		glVertex3f(-2.00f, 2.00f, 0.0f);
		glVertex3f(-1.50f, 1.60f, 0.0f);
		glVertex3f(-1.00f, 1.20f, 0.0f);
		glVertex3f(-0.50f, 0.80f, 0.0f);
		glVertex3f(-0.00f, 0.40f, 0.0f);
		glVertex3f(0.50f, 0.00f, 0.0f);
		glVertex3f(1.00f, -0.40f, 0.0f);
		glVertex3f(1.50f, -0.80f, 0.0f);
		glVertex3f(2.00f, -1.20f, 0.0f);
		glVertex3f(2.50f, -1.60f, 0.0f);
		glVertex3f(3.00f, -2.00f, 0.0f);
		glVertex3f(3.50f, -2.40f, 0.0f);
		glVertex3f(4.00f, -2.80f, 0.0f);

		glEnd();

		//Bottom Lights
		glBegin(GL_POINTS);
		glVertex3f(-12.50f, 4.4f, 0.0f);
		glVertex3f(-12.00f, 4.0f, 0.0f);
		glVertex3f(-11.50f, 3.6f, 0.0f);
		glVertex3f(-11.00f, 3.2f, 0.0f);
		glVertex3f(-10.50f, 2.8f, 0.0f);
		glVertex3f(-10.00f, 2.4f, 0.0f);
		glVertex3f(-9.50f, 2.0f, 0.0f);
		glVertex3f(-9.00f, 1.6f, 0.0f);
		glVertex3f(-8.50f, 1.2f, 0.0f);
		glVertex3f(-8.00f, 0.8f, 0.0f);
		glVertex3f(-7.50f, 0.4f, 0.0f);
		glVertex3f(-7.0f, 0.0f, 0.0f);
		glVertex3f(-6.50f, -0.4f, 0.0f);
		glVertex3f(-6.0f, -0.8f, 0.0f);
		glVertex3f(-5.50f, -1.2f, 0.0f);
		glVertex3f(-5.0f, -1.6f, 0.0f);
		glVertex3f(-4.50f, -2.0f, 0.0f);
		glVertex3f(-4.0f, -2.4f, 0.0f);
		glVertex3f(-3.50f, -2.8f, 0.0f);
		glVertex3f(-3.0f, -3.2f, 0.0f);
		glVertex3f(-2.50f, -3.6f, 0.0f);
		glVertex3f(-2.0f, -4.0f, 0.0f);
		glVertex3f(-1.50f, -4.4f, 0.0f);

		glVertex3f(-1.00f, -4.8f, 0.0f);
		glVertex3f(-0.50f, -5.2f, 0.0f);
		glVertex3f(0.00f, -5.6f, 0.0f);
		glVertex3f(0.50f, -6.0f, 0.0f);
		glEnd();

		//Green Signal
		glBegin(GL_POINTS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(4.50f, -3.20f, 0.0f);
		glVertex3f(8.50f, 0.80f, 0.0f);

		glVertex3f(1.00f, -6.4f, 0.0f);
		glVertex3f(-.50f, -8.0f, 0.0f);
		glEnd();

		//**********************************************End of Lights**************************************************************************

		//**********************************************Off runway planes.**********************************************************************
		//Right side
		
		//Plane 1
		if (iCnt < 2)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, 5.0f, -15.0f);
			glRotatef(130, 0.0f, 0.0f, -1.0f);
			Jet();
		}

		//Plane 2
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(1.4f, 3.50f, -14.0f);
		glRotatef(130, 0.0f, 0.0f, -1.0f);
		Jet();

		//Plane 3
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(2.8f, 2.20f, -14.0f);
		glRotatef(130, 0.0f, 0.0f, -1.0f);
		Jet();
		
		//Plane 4
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(4.4f, 1.0f, -14.0f);
		glRotatef(130, 0.0f, 0.0f, -1.0f);
		Jet();

		//***************************************Left Side***************************************
		if (iCnt < 1)
		{
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(-6.30f, -1.70f, -14.0f);
			glRotatef(-45.0f, 0.0f, 0.0f, -1.0f);
			Jet();
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-5.0f, -3.0f, -14.0f);
		glRotatef(-45.0f, 0.0f, 0.0f, -1.0f);
		Jet();
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-3.50f, -4.25f, -14.0f);
		glRotatef(-45.0f, 0.0f, 0.0f, -1.0f);
		Jet();
		
		//***************************************End Of Runway Planes********************************


		//***************************************************
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(T_x, T_y, T_z);

		glRotatef(45, 0.0f, 0.0f, -1.0f);

		glColor3f(0.50f, .50f, .50f);

		Jet();

		T_x = T_x + 0.000750f;
		T_y = T_y - 0.00040f;
		T_z = T_z + 0.00100f;

		if (iCnt < 3)
		{
			if (T_z > -3.0f)
			{
				iCnt++;
				T_x = -10.50f;
				T_y = 6.0f;
				T_z = -20.0f;

			}
		}
		else
		{
			iMagic = 1;
			RunWayFlying = TRUE;

		}

		//runway lights
		RunWayLights_X = RunWayLights_X + 0.050;
		RunWayLights_Y = RunWayLights_Y + 0.050;

		if (RunWayLights_X >= 1.0f)
		{
			RunWayLights_X = 0.0f;
			RunWayLights_Y = 0.0f;
		}


	}
	else
	{
		//code
		glClear(GL_COLOR_BUFFER_BIT);

		//Graph();					//Graph paper.

		//****************************************Start of Animation of INDIA****************************************************

		//******************Letter - I********************
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (IFlag == FALSE && iMagic==1)
		{
			glTranslatef(-ICnt, 0.0f, iZaxis);
		}
		else
		{
			glTranslatef(-1.5f, 0.0f, iZaxis);
			iMagic = 2;
		}
		Letter_I();

		//****************Letter A*****************************
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (AFlag == FALSE && iMagic == 2)
		{
			glTranslatef(ACnt, 0.0f, iZaxis);
		}
		else if (AFlag == TRUE)
		{
			glTranslatef(1.14f, 0.0f, iZaxis);
			iMagic = 3;
		}
		Letter_A();
		if (Fangle > 3.40f)
		{
			glBegin(GL_QUADS);

				glColor3f(1.0f, 0.50f, 0.0f);
				glVertex3f(0.07f, 0.01f, 0.0f);

				glColor3f(1.0f, .500f, 0.0f);
				glVertex3f(-0.07f, 0.01f, 0.0f);

				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(-0.085f, -0.025f, 0.0f);

				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(0.085f, -0.025f, 0.0f);

			glEnd();

			glBegin(GL_QUADS);

				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(0.085f, -0.025f, 0.0f);

				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(-0.085f, -0.025f, 0.0f);

				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(-0.10f, -0.055f, 0.0f);

				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.10f, -0.055f, 0.0f);

			glEnd();


		}


		//*****************Letter - N*********************
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (NFlag == FALSE && iMagic == 3)
		{
			glTranslatef(-0.89f, NCnt, iZaxis);
		}
		else if (NFlag == TRUE)
		{
			glTranslatef(-0.89f, 0.0f, iZaxis);
			iMagic = 4;
		}
		Letter_N();

		//****************Letter I*****************************	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (IIFlag == FALSE && iMagic == 4)
		{
			glTranslatef(0.54f, -IICnt, iZaxis);
		}
		else if (IIFlag == TRUE)
		{
			glTranslatef(0.54f, 0.0f, iZaxis);
			iMagic = 5;
		}
		Letter_I();

		//*****************Letter - D*********************
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (DFlag == FALSE && iMagic == 5)
		{
			glTranslatef(0.03, 0.0f, -DCnt);
		}
		else if (DFlag == TRUE)
		{
			glTranslatef(0.03, 0.0f, iZaxis);
			iMagic = 6;
		}
		Letter_D();

		//***********************************Transition of letters*******************************************
		//For I

		if (iMagic == 1 && IFlag == FALSE)
		{
			ICnt = ICnt - 0.0001f;

			if (ICnt < 1.5 && IFlag == FALSE)
			{
				IFlag = TRUE;
			}
		}

		//For N
		if (iMagic == 3 && NFlag == FALSE)
		{
			//fprintf(gpFILE, "In N iMagic 2 %d\n", iMagic);

			NCnt = NCnt - 0.0001f;
			if (NCnt < 0.00)
			{
				NFlag = TRUE;
			}

		}

		//For D
		if (iMagic == 5 && DFlag == FALSE)
		{
			DCnt = DCnt - 0.0008f;
			if (DCnt < -iZaxis)
			{
				DFlag = TRUE;
			}

		}

		//For I
		if (iMagic == 4 && IIFlag == FALSE)
		{
			IICnt = IICnt - 0.0001f;
			if (IICnt < 0)
			{
				IIFlag = TRUE;
			}

		}

		//For A
		if (iMagic == 2 && AFlag == FALSE)
		{
			ACnt = ACnt - 0.0001f;
			if (ACnt < 1.14)
			{
				AFlag = TRUE;
			}

		}

		//****************************************End of Transition of INDIA****************************************************

		//*********************************************Jets flying*********************************************

		//**********************************************First part********************************************* 
		//Plane 1 Top left
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (Plane == FALSE && iMagic == 6)
		{
			glTranslatef(Fangle, Fyaxis, -10.0f);
			Jet();
			BehindPlaneFlag();
		}
		else if (Plane == TRUE)
		{
			glTranslatef(Fangle, -0.0f, -10.0f);
			Jet();
//			BehindPlaneFlag();
		}

		//Plane 2 middle
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (iMagic == 6)
		{
			glTranslatef(Fangle, -0.00f, -10.0f);
			Jet();
			BehindPlaneFlag();
		}

		//Plane 3 bottom left
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (Plane == FALSE && iMagic == 6)
		{
			glTranslatef(Fangle, -Fyaxis, -10.0f);
			Jet();
			BehindPlaneFlag();
		}
		else if (Plane == TRUE)
		{
			glTranslatef(Fangle, -0.0f, -10.0f);
			Jet();
	//		BehindPlaneFlag();
		}

		// Transition for Plane(Merging).
		if (iMagic == 6)
		{
			//fprintf(gpFILE,"In iMagic 5 %d\n",iMagic);
			Fangle = Fangle + 0.0002;
			if (Fangle > 2.8000f)		//end point where the plane will fly again.
			{
				iMagic = 7;
				Fyaxis = 0.0f;
			}

			Fyaxis = Fyaxis - 0.0002f;
			if (Fyaxis < 0.0002)
			{
				Plane = TRUE;
			}
		}


		//****************************************************Part 2************************************************************
		//Again Flying	/ Again Flying seprating.
		//Top Plane
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (iMagic == 7)
		{
			glTranslatef(Fangle, FyaxisN, -10.0f);
			Jet();
			BehindPlaneFlag();
		}

		//Middle Plane
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(Fangle, -0.00f, -10.0f);


		//Bottom Plane
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if (iMagic == 7)
		{
			glTranslatef(Fangle, -FyaxisN, -10.0f);
			Jet();
			BehindPlaneFlag();
		}

		//Again Flying seprating Transition
		if (iMagic == 7)
		{
			//fprintf(gpFILE, "In iMagic 6 %d\n", iMagic);
			FyaxisN = FyaxisN + 0.0002f;
		}

		//*****************************************************************************************************
	}

	SwapBuffers(ghdc);
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

	if (gpFILE)
	{
		fprintf(gpFILE, "Program executed successfully Date : %d-%d-%d Time:%d:%d:%d\n", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
		fclose(gpFILE);
		gpFILE = NULL;
	}

}
