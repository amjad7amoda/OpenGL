#ifndef CHAIR_H
#define CHAIR_H

#include <GL/glut.h>

extern int isLightingEnabled;
extern void glSetColor3f(float r, float g, float b);

class Chair {
public:
    float x, y, z;
    float seatSize, seatHeight, backrestHeight, legThickness;

    // Constructor
    Chair(float _x, float _y, float _z, float _seatSize, float _seatHeight, float _backrestHeight, float _legThickness);

    // Method to draw the chair
    void Draw();
    void setLegsTexture(int legsTexture);
    void setChair(int chair);
};

#endif
