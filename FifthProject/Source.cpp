#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#include "texture.h"
#include "camera.h"
#include <fstream>
#include "Model_3DS.h"
#include <ctime>
#include "Chair.h"
#include "Table.h"
#include "Restaurant.h"
#include "Colba.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Cntext
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default
GLfloat LightPos[] = { 0.0f,0.4f,0.0f,1.0f };
GLfloat LightAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiff[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat LightSpec[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat MatAmb[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MatDiff[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat MatSpec[] = { 0.2f,0.2f,0.2f,1.0f };
GLfloat MatShn[] = { 128.0f };


LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}


//Camera Global
Camera MyCamera;

//Texture Global
int SKYFRONT, SKYBACK, SKYLEFT, SKYRIGHT, SKYUP, SKYDOWN;
int McDonaldsLogo, McDonaldsUnderLogo, PizzaHotLogo, PizzaHotUnderLogo;
int SyriaPlate, SalesPlate;
int Fence, tableTexture, tableLegsTexture;
//Model Global
Model_3DS *tank;
Model_3DS* tree;
GLTexture BARK, Leaf;

//float width, height, depth;
float eyeX = 0, eyeY = 1.25, eyeZ = 4, angel = -1.5, pitch = 0;

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.1f, 0.1f, 0.1f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations



	//SkyBlock Texture
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SKYFRONT = LoadTexture("Gallery\\SkyBox\\CyenWall.bmp", 255);
	SKYBACK = LoadTexture("Gallery\\SkyBox\\CyenWall.bmp", 255);
	SKYLEFT = LoadTexture("Gallery\\SkyBox\\CyenWall.bmp", 255);
	SKYRIGHT = LoadTexture("Gallery\\SkyBox\\CyenWall.bmp", 255);
	SKYUP = LoadTexture("Gallery\\SkyBox\\floor.bmp", 255);
	SKYDOWN = LoadTexture("Gallery\\SkyBox\\floor.bmp", 255);
	SyriaPlate = LoadTexture("Gallery\\Plates\\Syria.bmp");
	SalesPlate = LoadTexture("Gallery\\Plates\\Sales.bmp");
	McDonaldsLogo = LoadTexture("Gallery\\Resturants\\McDonald's\\McDonalds-Logo.bmp", 255);
	McDonaldsUnderLogo = LoadTexture("Gallery\\Resturants\\McDonald's\\McDonaldsUnderLogo.bmp", 255);
	PizzaHotLogo = LoadTexture("Gallery\\Resturants\\PizzaHot\\PizzaHot.bmp");
	PizzaHotUnderLogo = LoadTexture("Gallery\\Resturants\\PizzaHot\\PizzaHotUnderLogo.bmp");
	Fence = LoadTexture("Gallery\\Resturants\\Fence.bmp", 255);
	tableTexture = LoadTexture("Gallery\\Table\\Table.bmp");
	tableLegsTexture = LoadTexture("Gallery\\Table\\TableLegs.bmp");
	glDisable(GL_TEXTURE_2D);


	//Lighting
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpec);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiff);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_BACK, GL_AMBIENT, MatAmb);
	glMaterialfv(GL_BACK, GL_DIFFUSE, MatDiff);
	glMaterialfv(GL_BACK, GL_SPECULAR, MatSpec);
	glMaterialfv(GL_BACK, GL_SHININESS, MatShn);
	glEnable(GL_COLOR_MATERIAL);

	//Models Include
	//Tank
	tank = new Model_3DS();
	tank->Load("Gallery\\3D\\Tank\\tank.3DS");
	tank->pos.x = 8;
	tank->pos.y = 0.01;
	tank->pos.z = 5;
	tank->scale = 0.5;

	//Tree
	tree = new Model_3DS();
	tree->Load("Gallery\\3D\\Tree\\Tree1.3DS");
	Leaf.LoadBMP("Gallery\\3D\\Tree\\bat.bmp");
	BARK.LoadBMP("Gallery\\3D\\Tree\\bark_loo.bmp");
	tree->Materials[0].tex = BARK;
	tree->Materials[1].tex = Leaf;
	tree->Materials[2].tex = Leaf;
	tree->Materials[3].tex = Leaf;
	tree->Materials[4].tex = Leaf;
	tree->pos.x = -3;
	tree->pos.y = 0;
	tree->pos.z = -5.5;
	tree->scale = 0.2;
	



	//Camera Definition
	MyCamera = Camera();

	return TRUE;										// Initialization Went OK
}
int isLightingEnabled = 2;
void Key(bool* keys) {
	if (keys['Z']) { 
		isLightingEnabled = 2;
		GLfloat LightAmb[3] = { 1.0f, 1.0f, 1.0f };
		GLfloat LightDiff[3] = { 1.0f, 1.0f, 0.9f }; 
		GLfloat LightSpec[3] = { 1.0f, 1.0f, 0.8f }; 
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiff);
		glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpec);
	}
	if (keys['X']) { 
		isLightingEnabled = 1;
		GLfloat LightAmb[3] = { 0.2f, 0.2f, 0.4f }; 
		GLfloat LightDiff[3] = { 0.4f, 0.4f, 0.6f }; 
		GLfloat LightSpec[3] = { 0.5f, 0.5f, 0.7f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiff);
		glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpec);
	}
	if (keys['C']) {
		isLightingEnabled = 0;
		GLfloat LightAmb[3] = { 0.0f, 0.0f, 0.0f }; 
		GLfloat LightDiff[3] = { 0.0f, 0.0f, 0.0f };
		GLfloat LightSpec[3] = { 0.0f, 0.0f, 0.0f };
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiff);
		glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpec);
	}
}
void glSetColor3f(float r, float g, float b) {
	if (isLightingEnabled == 2) 
		glColor3f(r, g, b);
	else if (isLightingEnabled == 1) 
		glColor3f(r * 0.5f, g * 0.5f, b * 0.5f); 
	else if (isLightingEnabled == 0) 
		glColor3f(r * 0.2f, g * 0.2f, b * 0.2f); 
}

void CameraController(bool* keys, float speed = 0.05) {
	MyCamera.Render();
	float eyeX1 = eyeX;
	float eyeY1 = eyeY;
	float eyeZ1 = eyeZ;

	//Look Down
	if (keys[VK_DOWN])
		MyCamera.RotateX(-1 * 4 * speed);

	//Look Up
	if (keys[VK_UP])
		MyCamera.RotateX(1 * 4 * speed);

	// Rotate left
	if (keys['A'])
		angel -= speed / 3;

	// Rotate right
	if (keys['D'])
		angel += speed / 3;

	// Move forward
	if (keys['W']) {
		eyeX1 += cos(angel) * speed;
		eyeZ1 += sin(angel) * speed;
		//if ((eyeX1 > -boundaryX) && (eyeX1 < boundaryX) && (eyeZ1 > -boundaryZ) && (eyeZ1 < boundaryZ) && (eyeZ1 > -5.7)) {
			eyeX = eyeX1;
			eyeZ = eyeZ1;
		//}
	}

	// Move backward
	if (keys['S']) {
		eyeX1 -= cos(angel) * speed;
		eyeZ1 -= sin(angel) * speed;
		//if ((eyeX1 > -boundaryX) && (eyeX1 < boundaryX) && (eyeZ1 > -boundaryZ) && (eyeZ1 < boundaryZ) && (eyeZ1 > -5.7)) {
			eyeX = eyeX1;
			eyeZ = eyeZ1;
		//}
	}


	// Move upward
	if (keys[VK_SPACE]) {
		eyeY1 += speed;
		//if ((eyeY1 > boundaryYMin) && (eyeY1 < boundaryYMax)) 
			eyeY = eyeY1;
		
	}

	// Move downward
	if (keys[VK_SHIFT]) {
		eyeY1 -= speed;
		//if ((eyeY1 > 1.5) && (eyeY1 < boundaryYMax))
			eyeY = eyeY1;
	}

	gluLookAt(eyeX, eyeY, eyeZ, eyeX + cos(angel), eyeY, eyeZ + sin(angel), 0, 6, 0);
}

int DrawGLScene(GLvoid)									
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Key(keys);
	CameraController(keys);
	Restaurant restaurant(29, 5, 20);
	restaurant.Draw();
	
	return TRUE;
}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "GL template", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
													if (!HIWORD(wParam))					// Check Minimization State
													{
														active = TRUE;						// Program Is Active
													}
													else
													{
														active = FALSE;						// Program Is No Longer Active
													}

													return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
													switch (wParam)							// Check System Calls
													{
													case SC_SCREENSAVE:					// Screensaver Trying To Start?
													case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
														return 0;							// Prevent From Happening
													}
													break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
													PostQuitMessage(0);						// Send A Quit Message
													return 0;								// Jump Back
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
													keys[wParam] = TRUE;					// If So, Mark It As TRUE
													return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
													keys[wParam] = FALSE;					// If So, Mark It As FALSE
													return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
													ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
													return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen = FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("Example", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done = TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1] = FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("OpenGL template", 640, 480, 16, fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

