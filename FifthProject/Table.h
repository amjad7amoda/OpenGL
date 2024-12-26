#ifndef TABLE_H
#define TABLE_H

#include <GL/glut.h>

extern bool isLightingEnabled;
extern void glSetColor3f(float r, float g, float b);

class Table {
public:
    float x, y, z;
    float tableTopSize, tableTopThickness, tableHeight, legThickness;

    // Constructor
    Table(float _x, float _y, float _z, float _tableTopSize, float _tableTopThickness, float _tableHeight, float _legThickness);

    // Method to draw the table
    void Draw();
};

#endif
