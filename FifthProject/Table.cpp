#include "Table.h"

Table::Table(float _x, float _y, float _z, float _tableTopSize, float _tableTopThickness, float _tableHeight, float _legThickness) {
    x = _x;
    y = _y;
    z = _z;
    tableTopSize = _tableTopSize;
    tableTopThickness = _tableTopThickness;
    tableHeight = _tableHeight;
    legThickness = _legThickness;
}

void Table::Draw() {
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
        glSetColor3f(0.4f, 0.2f, 0.1f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
}
