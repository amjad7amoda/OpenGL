#ifndef TABLE_H
#define TABLE_H

#include <GL/glut.h>

extern int isLightingEnabled;
extern void glSetColor3f(float r, float g, float b);

class Table {
public:
    float x, y, z;
    float tableTopSize, tableTopThickness, tableHeight, legThickness;

   //Constructer
    Table(float _x, float _y, float _z, float _tableTopSize, float _tableTopThickness, float _tableHeight, float _legThickness);

   //Functions
    void Draw();
};

#endif
