#include "Chair.h"

Chair::Chair(float _x, float _y, float _z, float _seatSize, float _seatHeight, float _backrestHeight, float _legThickness) {
    x = _x;
    y = _y;
    z = _z;
    seatSize = _seatSize;
    seatHeight = _seatHeight;
    backrestHeight = _backrestHeight;
    legThickness = _legThickness;
}


void Chair::Draw() {
    // Draw chair seat
    glPushMatrix();
    glTranslatef(x, y + seatHeight, z);
    glScalef(seatSize, legThickness, seatSize);
    glSetColor3f(0.6f, 0.3f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw chair backrest
    glPushMatrix();
    glTranslatef(x, y + seatHeight + backrestHeight / 2.0f, z - seatSize / 2.0f + legThickness / 2.0f);
    glScalef(seatSize, backrestHeight, legThickness);
    glSetColor3f(0.4f, 0.2f, 0.1f);
    glutSolidCube(1.0);
    glPopMatrix();

    // Draw chair legs
    for (int i = 0; i < 4; i++) {
        float legX = x + (i % 2 == 0 ? -1 : 1) * (seatSize / 2.0f - legThickness / 2.0f);
        float legZ = z + (i / 2 == 0 ? -1 : 1) * (seatSize / 2.0f - legThickness / 2.0f);
        glPushMatrix();
        glTranslatef(legX, y + seatHeight / 2.0f, legZ);
        glScalef(legThickness, seatHeight, legThickness);
        glSetColor3f(0.4f, 0.2f, 0.1f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glColor3f(1, 1, 1);
}
