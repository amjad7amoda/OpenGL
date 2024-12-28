#ifndef RESTURNAT_H
#define RESTURANT_H

#include <GL/glut.h>


class Restaurant {
public:
	int width, height, depth;

	Restaurant(int w, int h, int d);

	void Draw();
	void DrawRestaurantSkyBox();
	void DrawTriangle();
	void DrawColbas();
	void DrawTables();
	void DrawTableWithChairs(float x, float y, float z);
	void DrawPlates();
	void DrawTree(float x, float y, float z);
	void DrawPlate(int w, int h, float frameThickness, int texture);
};

#endif