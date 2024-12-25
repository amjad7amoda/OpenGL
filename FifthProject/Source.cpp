﻿#define _CRT_SECURE_NO_WARNINGS
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
GLfloat LightPos[] = { 0.0f,0.0f,0.0f,1.0f };
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
int SKYFRONT, SKYBACK, SKYLEFT, SKYRIGHT, SKYUP, SKYDOWN, Plate;

//Model Global
Model_3DS *tank;
Model_3DS* tree;
GLTexture BARK, Leaf;

void Draw_Skybox(float x, float y, float z, float width, float height, float length)
{
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;
	glEnable(GL_TEXTURE_2D);

	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, SKYFRONT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, SKYBACK);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, SKYLEFT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, SKYRIGHT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, SKYUP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, SKYDOWN);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

	glColor3f(1, 1, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

float width, height, depth, x, y;
float eyeX = 0, eyeY = 1, eyeZ = 4, angel = -1.5, pitch = 0;

;

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
	Plate = LoadTexture("Gallery\\Syria.bmp");
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
	tree->pos.x = 0;
	tree->pos.y = 0;
	tree->pos.z = 0;
	tree->scale = 0.05;

	//Camera Definition
	MyCamera = Camera();

	return TRUE;										// Initialization Went OK
}

bool isLightingEnabled = true;
void Key(bool* keys) {
	if (keys['L']) {
		isLightingEnabled = true;
		LightAmb[0] = 1; LightAmb[1] = 1; LightAmb[2] = 1;
		LightDiff[0] = 1; LightDiff[1] = 1; LightDiff[2] = 1;
		LightSpec[0] = 1; LightSpec[1] = 1; LightSpec[2] = 1;
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
		glLightfv(GL_LIGHT1, GL_SPECULAR, LightAmb);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightAmb);
	}
	if (keys['O']) {
		isLightingEnabled = false;
		LightAmb[0] = 0; LightAmb[1] = 0; LightAmb[2] = 0;
		LightDiff[0] = 0; LightDiff[1] = 0; LightDiff[2] = 0;
		LightSpec[0] = 0; LightSpec[1] = 0; LightSpec[2] = 0;
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
		glLightfv(GL_LIGHT1, GL_SPECULAR, LightAmb);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, LightAmb);
	}
}



void glSetColor3f(float r, float g, float b) {
	if (isLightingEnabled) 
		glColor3f(r, g, b);
	else 
		glColor3f(r * 0.2f, g * 0.2f, b * 0.2f);
}
void camera(bool* keys, float speed = 0.05) {
	MyCamera.Render();
	float boundaryX = width * 0.975f / 2.0f;
	float boundaryYMin = height * 0.025f;
	float boundaryYMax = height * 0.975f;
	float boundaryZ = depth * 0.975f / 2.0f;

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
		angel -= speed / 2;

	// Rotate right
	if (keys['D'])
		angel += speed / 2;

	// Move forward
	if (keys['W']) {
		eyeX1 += cos(angel) * speed;
		eyeZ1 += sin(angel) * speed;
		if ((eyeX1 > -boundaryX) && (eyeX1 < boundaryX) && (eyeZ1 > -boundaryZ) && (eyeZ1 < boundaryZ)) {
			eyeX = eyeX1;
			eyeZ = eyeZ1;
		}
	}

	// Move backward
	if (keys['S']) {
		eyeX1 -= cos(angel) * speed;
		eyeZ1 -= sin(angel) * speed;
		if ((eyeX1 > -boundaryX) && (eyeX1 < boundaryX) && (eyeZ1 > -boundaryZ) && (eyeZ1 < boundaryZ)) {
			eyeX = eyeX1;
			eyeZ = eyeZ1;
		}
	}


	// Move upward
	if (keys[VK_SPACE]) {
		eyeY1 += speed;
		if ((eyeY1 > boundaryYMin) && (eyeY1 < boundaryYMax)) {
			eyeY = eyeY1;
		}
	}

	// Move downward
	if (keys[VK_SHIFT]) {
		eyeY1 -= speed;
		if ((eyeY1 > boundaryYMin) && (eyeY1 < boundaryYMax))
			eyeY = eyeY1;
	}
}

void move_tank(float speed){

	if (keys['Y'])
		tank->pos.z -=speed;
	if (keys['H'])
		tank->pos.z +=speed;
	if (keys['J'])
		tank->pos.x +=speed;
	if (keys['G'])
		tank->pos.x -=speed;

}

void DrawTexturedPlate(int x, int y , int z ,int plate) {
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, plate); 
	glBegin(GL_QUADS); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2, -height / 2, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 2, -height / 2, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 2, height / 2, depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2, height / 2, depth);
	glEnd();
}
void DrawTable(float x, float y, float z) {
	glColor3f(1, 1, 1);

	// Table dimensions
	float tableTopSize = 1.5f;
	float tableTopThickness = 0.1f;
	float tableHeight = 0.7f;
	float legThickness = 0.1f;


	// Draw table top
	glPushMatrix();
	glTranslatef(x, y + tableHeight - tableTopThickness / 2.0f, z);
	glScalef(tableTopSize, tableTopThickness, tableTopSize);
	glSetColor3f(0.6f, 0.3f, 0.1f);
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw table legs
	for (int i = 0; i < 4; i++) {
		float legX = x + (i % 2 == 0 ? -1 : 1) * (tableTopSize / 2.0f - legThickness / 2.0f);
		float legZ = z + (i / 2 == 0 ? -1 : 1) * (tableTopSize / 2.0f - legThickness / 2.0f);

		glPushMatrix();
		glTranslatef(legX, y + (tableHeight - tableTopThickness) / 2.0f, legZ);
		glScalef(legThickness, tableHeight - tableTopThickness, legThickness);
		glSetColor3f(0.4f, 0.2f, 0.1f); // Darker brown for legs
		glutSolidCube(1.0);
		glPopMatrix();
	}
}
void DrawChair(float x, float y, float z, float chairSeatSize, float chairSeatHeight, float chairBackrestHeight, float chairLegThickness) {
	// Draw chair seat
	glColor3f(1, 1, 1);

	glPushMatrix();
	glTranslatef(x, y + chairSeatHeight, z);
	glScalef(chairSeatSize, chairLegThickness, chairSeatSize);
	glSetColor3f(0.6f, 0.3f, 0.1f);
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw chair backrest
	glPushMatrix();
	glTranslatef(x, y + chairSeatHeight + chairBackrestHeight / 2.0f, z - chairSeatSize / 2.0f + chairLegThickness / 2.0f);
	glScalef(chairSeatSize, chairBackrestHeight, chairLegThickness);
	glSetColor3f(0.4f, 0.2f, 0.1f);
	glutSolidCube(1.0);
	glPopMatrix();

	// Draw chair legs
	for (int i = 0; i < 4; i++) {
		float legX = x + (i % 2 == 0 ? -1 : 1) * (chairSeatSize / 2.0f - chairLegThickness / 2.0f);
		float legZ = z + (i / 2 == 0 ? -1 : 1) * (chairSeatSize / 2.0f - chairLegThickness / 2.0f);
		glPushMatrix();
		glTranslatef(legX, y + chairSeatHeight / 2.0f, legZ);
		glScalef(chairLegThickness, chairSeatHeight, chairLegThickness);
		glSetColor3f(0.4f, 0.2f, 0.1f);
		glutSolidCube(1.0);
		glPopMatrix();
	}
	glColor3f(1, 1, 1);
}
void DrawTableWithChairs(float x, float y, float z) {
	glColor3f(1, 1, 1);
	//glClear(GL_COLOR_BUFFER_BIT);
	// Table dimensions
	float tableTopSize = 1.5f;
	float tableTopThickness = 0.1f;
	float tableHeight = 0.7f;
	float legThickness = 0.1f;

	// Chair dimensions
	float chairSeatSize = 0.5f;
	float chairSeatHeight = 0.4f;
	float chairLegThickness = 0.05f;
	float chairBackrestHeight = 0.5f;

	// Draw table
	DrawTable(x, y, z);

	// Draw chairs around the table
	float chairDistance = tableTopSize / 2.0f + chairSeatSize / 2.0f + 0.1f;

	// Left chair
	glPushMatrix();
	glTranslatef(x - chairDistance, y, z);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	DrawChair(0.0f, 0.0f, 0.0f, chairSeatSize, chairSeatHeight, chairBackrestHeight, chairLegThickness);
	glPopMatrix();

	// Right chair
	glPushMatrix();
	glTranslatef(x + chairDistance, y, z);
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); // Rotate to face the table
	DrawChair(0.0f, 0.0f, 0.0f, chairSeatSize, chairSeatHeight, chairBackrestHeight, chairLegThickness);
	glPopMatrix();

	// Front chair
	glPushMatrix();
	glTranslatef(x, y, z - chairDistance);
	DrawChair(0.0f, 0.0f, 0.0f, chairSeatSize, chairSeatHeight, chairBackrestHeight, chairLegThickness);
	glPopMatrix();

	// Back chair
	glPushMatrix();
	glTranslatef(x, y, z + chairDistance);

	glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
	DrawChair(0.0f, 0.0f, 0.0f, chairSeatSize, chairSeatHeight, chairBackrestHeight, chairLegThickness);
	glPopMatrix();
}

void DrawSkyBox() {
	glClear(GL_COLOR_BUFFER_BIT);
	float halfWidth = width / 2.0f;
	float halfDepth = depth / 2.0f;
	glEnable(GL_TEXTURE_2D);
	// Draw the Back side
	glBindTexture(GL_TEXTURE_2D, SKYBACK);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-halfWidth, 0, halfDepth);
	glTexCoord2f(1, 0);
	glVertex3f(halfWidth, 0, halfDepth);
	glTexCoord2f(1, 1);
	glVertex3f(halfWidth, height, halfDepth);
	glTexCoord2f(0, 1);
	glVertex3f(-halfWidth, height, halfDepth);
	glEnd();

	// Draw the Left side
	glBindTexture(GL_TEXTURE_2D, SKYLEFT);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-halfWidth, 0, halfDepth);
	glTexCoord2f(1, 0);
	glVertex3f(-halfWidth, 0, -halfDepth);
	glTexCoord2f(1, 1);
	glVertex3f(-halfWidth, height, -halfDepth);
	glTexCoord2f(0, 1);
	glVertex3f(-halfWidth, height, halfDepth);
	glEnd();



	// Draw the Front side
	glBindTexture(GL_TEXTURE_2D, SKYFRONT);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-halfWidth, 0, -halfDepth);
	glTexCoord2f(1, 0);
	glVertex3f(halfWidth, 0, -halfDepth);
	glTexCoord2f(1, 1);
	glVertex3f(halfWidth, height, -halfDepth);
	glTexCoord2f(0, 1);
	glVertex3f(-halfWidth, height, -halfDepth);
	glEnd();

	// Draw the Top
	glBindTexture(GL_TEXTURE_2D, SKYUP);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-halfWidth, height, halfDepth);
	glTexCoord2f(width, 0);
	glVertex3f(halfWidth, height, halfDepth);
	glTexCoord2f(width, depth);
	glVertex3f(halfWidth, height, -halfDepth);
	glTexCoord2f(0, depth);
	glVertex3f(-halfWidth, height, -halfDepth);
	glEnd();


	// Draw the Right side
	glBindTexture(GL_TEXTURE_2D, SKYRIGHT);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(halfWidth, 0, halfDepth);
	glTexCoord2f(1, 0);
	glVertex3f(halfWidth, 0, -halfDepth);
	glTexCoord2f(1, 1);
	glVertex3f(halfWidth, height, -halfDepth);
	glTexCoord2f(0, 1);
	glVertex3f(halfWidth, height, halfDepth);
	glEnd();


	// Draw the Bottom
	glBindTexture(GL_TEXTURE_2D, SKYDOWN);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3f(-halfWidth, 0, halfDepth);
	glTexCoord2d(width, 0);
	glVertex3f(halfWidth, 0, halfDepth);
	glTexCoord2d(width, depth);
	glVertex3f(halfWidth, 0, -halfDepth);
	glTexCoord2d(0, depth);
	glVertex3f(-halfWidth, 0, -halfDepth);
	glEnd();
}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	
	Key(keys);
	camera(keys);
	gluLookAt(eyeX, eyeY, eyeZ, eyeX + cos(angel), eyeY, eyeZ + sin(angel), 0, 6, 0);
	move_tank(0.1);
	
	//LightAmb[0] = x;
	//LightAmb[1] = x;
	//LightAmb[2] = x;
	//glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);

	width = 20; height = 4; depth = 15;
	glEnable(GL_TEXTURE_2D);
	DrawSkyBox();
	//Plate
	glPushMatrix();
	glTranslatef((width / 2) - 0.05f, (height / 2.0f) + 0.25, (depth / 2) - 1.5);
	glRotatef(90, 0.0f, 1.0f, 0.0f);
	DrawTexturedPlate(2,1,0,Plate);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	//tree->Draw();
	tank->Draw();

	glDisable(GL_LIGHTING);
	glPushMatrix();
	
	DrawTableWithChairs(8.0f, 0.0f, 5.0f);
	DrawTableWithChairs(8.0f, 0.0f, 1.0f);
	DrawTableWithChairs(4.0f, 0.0f, 5.0f);
	DrawTableWithChairs(4.0f, 0.0f, 1.0f);
	DrawTableWithChairs(-8.0f, 0.0f, 5.0f);
	DrawTableWithChairs(-8.0f, 0.0f, 1.0f);
	DrawTableWithChairs(-4.0f, 0.0f, 5.0f);
	DrawTableWithChairs(-4.0f, 0.0f, 1.0f);
	
	glPopMatrix();
	glEnable(GL_LIGHTING);
	//glDisable(GL_LIGHT1);
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
