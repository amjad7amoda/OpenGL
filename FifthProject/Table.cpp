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
    // رسم سطح الطاولة
    glPushMatrix();
    glTranslatef(x, y + tableHeight - tableTopThickness / 2.0f, z);
    glSetColor3f(0.6f, 0.3f, 0.1f);

    // رسم مكعب سطح الطاولة يدوياً
    float halfTopSize = tableTopSize / 2.0f;
    float halfThickness = tableTopThickness / 2.0f;
    glBegin(GL_QUADS);

    // وجه علوي
    glVertex3f(-halfTopSize, halfThickness, -halfTopSize);
    glVertex3f(halfTopSize, halfThickness, -halfTopSize);
    glVertex3f(halfTopSize, halfThickness, halfTopSize);
    glVertex3f(-halfTopSize, halfThickness, halfTopSize);

    // وجه سفلي
    glVertex3f(-halfTopSize, -halfThickness, -halfTopSize);
    glVertex3f(halfTopSize, -halfThickness, -halfTopSize);
    glVertex3f(halfTopSize, -halfThickness, halfTopSize);
    glVertex3f(-halfTopSize, -halfThickness, halfTopSize);

    // أوجه جانبية
    glVertex3f(-halfTopSize, -halfThickness, -halfTopSize);
    glVertex3f(-halfTopSize, halfThickness, -halfTopSize);
    glVertex3f(-halfTopSize, halfThickness, halfTopSize);
    glVertex3f(-halfTopSize, -halfThickness, halfTopSize);

    glVertex3f(halfTopSize, -halfThickness, -halfTopSize);
    glVertex3f(halfTopSize, halfThickness, -halfTopSize);
    glVertex3f(halfTopSize, halfThickness, halfTopSize);
    glVertex3f(halfTopSize, -halfThickness, halfTopSize);

    glVertex3f(-halfTopSize, -halfThickness, -halfTopSize);
    glVertex3f(halfTopSize, -halfThickness, -halfTopSize);
    glVertex3f(halfTopSize, halfThickness, -halfTopSize);
    glVertex3f(-halfTopSize, halfThickness, -halfTopSize);

    glVertex3f(-halfTopSize, -halfThickness, halfTopSize);
    glVertex3f(halfTopSize, -halfThickness, halfTopSize);
    glVertex3f(halfTopSize, halfThickness, halfTopSize);
    glVertex3f(-halfTopSize, halfThickness, halfTopSize);

    glEnd();
    glPopMatrix();

    //Table Legs
    for (int i = 0; i < 4; i++) {
        float legX = x + (i % 2 == 0 ? -1 : 1) * (tableTopSize / 2.0f - legThickness / 2.0f - 0.01f);
        float legZ = z + (i / 2 == 0 ? -1 : 1) * (tableTopSize / 2.0f - legThickness / 2.0f - 0.01f);
        float legHeight = tableHeight - tableTopThickness;

        glPushMatrix();
        glTranslatef(legX, y + legHeight / 2.0f, legZ);
        glSetColor3f(0.4f, 0.2f, 0.1f);

        float halfLegThickness = legThickness / 2.0f;
        float halfLegHeight = legHeight / 2.0f;
        glBegin(GL_QUADS);

        //Top Face
        glVertex3f(-halfLegThickness, halfLegHeight, -halfLegThickness);
        glVertex3f(halfLegThickness, halfLegHeight, -halfLegThickness);
        glVertex3f(halfLegThickness, halfLegHeight, halfLegThickness);
        glVertex3f(-halfLegThickness, halfLegHeight, halfLegThickness);

        //Bottom Face
        glVertex3f(-halfLegThickness, -halfLegHeight, -halfLegThickness);
        glVertex3f(halfLegThickness, -halfLegHeight, -halfLegThickness);
        glVertex3f(halfLegThickness, -halfLegHeight, halfLegThickness);
        glVertex3f(-halfLegThickness, -halfLegHeight, halfLegThickness);

        //Side Faces
        glVertex3f(-halfLegThickness, -halfLegHeight, -halfLegThickness);
        glVertex3f(-halfLegThickness, halfLegHeight, -halfLegThickness);
        glVertex3f(-halfLegThickness, halfLegHeight, halfLegThickness);
        glVertex3f(-halfLegThickness, -halfLegHeight, halfLegThickness);

        glVertex3f(halfLegThickness, -halfLegHeight, -halfLegThickness);
        glVertex3f(halfLegThickness, halfLegHeight, -halfLegThickness);
        glVertex3f(halfLegThickness, halfLegHeight, halfLegThickness);
        glVertex3f(halfLegThickness, -halfLegHeight, halfLegThickness);

        glVertex3f(-halfLegThickness, -halfLegHeight, -halfLegThickness);
        glVertex3f(halfLegThickness, -halfLegHeight, -halfLegThickness);
        glVertex3f(halfLegThickness, halfLegHeight, -halfLegThickness);
        glVertex3f(-halfLegThickness, halfLegHeight, -halfLegThickness);

        glVertex3f(-halfLegThickness, -halfLegHeight, halfLegThickness);
        glVertex3f(halfLegThickness, -halfLegHeight, halfLegThickness);
        glVertex3f(halfLegThickness, halfLegHeight, halfLegThickness);
        glVertex3f(-halfLegThickness, halfLegHeight, halfLegThickness);

        glEnd();
        glPopMatrix();
    }
}

void Table::setTableTexture(int Table) {
    glSetColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, Table);

    glPushMatrix();
    glTranslatef(x, y + tableHeight - tableTopThickness / 2.0f, z);

    float halfTopSize = tableTopSize / 2.0f;
    float halfThickness = tableTopThickness / 2.0f;
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfTopSize, halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfTopSize, halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfTopSize, halfThickness, halfTopSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfTopSize, halfThickness, halfTopSize);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfTopSize, -halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfTopSize, -halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfTopSize, -halfThickness, halfTopSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfTopSize, -halfThickness, halfTopSize);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfTopSize, -halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfTopSize, halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfTopSize, halfThickness, halfTopSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfTopSize, -halfThickness, halfTopSize);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfTopSize, -halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfTopSize, halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfTopSize, halfThickness, halfTopSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(halfTopSize, -halfThickness, halfTopSize);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfTopSize, -halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfTopSize, -halfThickness, -halfTopSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfTopSize, halfThickness, -halfTopSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfTopSize, halfThickness, -halfTopSize);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfTopSize, -halfThickness, halfTopSize);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfTopSize, -halfThickness, halfTopSize);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfTopSize, halfThickness, halfTopSize);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfTopSize, halfThickness, halfTopSize);

    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glSetColor3f(1, 1, 1);
}

void Table::setLegsTexture(int tableLegs) {
    glSetColor3f(1, 1, 1);

    glEnable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, tableLegs); 

    for (int i = 0; i < 4; i++) {
        float legX = x + (i % 2 == 0 ? -1 : 1) * (tableTopSize / 2.0f - legThickness / 2.0f - 0.01f);
        float legZ = z + (i / 2 == 0 ? -1 : 1) * (tableTopSize / 2.0f - legThickness / 2.0f - 0.01f);
        float legHeight = tableHeight - tableTopThickness;

        glPushMatrix();
        glTranslatef(legX, y + legHeight / 2.0f, legZ);

        float halfLegThickness = legThickness / 2.0f;
        float halfLegHeight = legHeight / 2.0f;

        glBegin(GL_QUADS);

        //Front Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLegThickness, halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLegThickness, halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLegThickness, halfLegHeight, halfLegThickness);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLegThickness, halfLegHeight, halfLegThickness);

        //Bottom Face
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLegThickness, -halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLegThickness, -halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLegThickness, -halfLegHeight, halfLegThickness);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLegThickness, -halfLegHeight, halfLegThickness);

        //Side Faces
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLegThickness, -halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfLegThickness, halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfLegThickness, halfLegHeight, halfLegThickness);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLegThickness, -halfLegHeight, halfLegThickness);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(halfLegThickness, -halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLegThickness, halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLegThickness, halfLegHeight, halfLegThickness);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(halfLegThickness, -halfLegHeight, halfLegThickness);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLegThickness, -halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLegThickness, -halfLegHeight, -halfLegThickness);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLegThickness, halfLegHeight, -halfLegThickness);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLegThickness, halfLegHeight, -halfLegThickness);

        glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfLegThickness, -halfLegHeight, halfLegThickness);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(halfLegThickness, -halfLegHeight, halfLegThickness);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(halfLegThickness, halfLegHeight, halfLegThickness);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfLegThickness, halfLegHeight, halfLegThickness);

        glEnd();
        glPopMatrix();
    }
    
    glDisable(GL_TEXTURE_2D);
    glSetColor3f(1, 1, 1);
}


