#ifndef COLBA_H
#define COLBA_H

#include <GL/glut.h>

class Colba {
private:
    float w, h, d; 
    float offset;  
public:
    //Constructer
    Colba(float width, float height, float depth);

    //Functions
    void draw();
    void setBack(int back);
    void setFront(int front1 = 0, int front2 = 0);
    void setRight(int right);
    void setLeft(int left);
};

#endif
