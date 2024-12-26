#include "Colba.h"

Colba::Colba(float width, float height, float depth) {
    w = width;
    h = height;
    d = depth;
    offset = 0.001f;  // فرق صغير لتجنب التداخل
}

extern bool isLightingEnabled;
extern void glSetColor3f(float r, float g, float b);

void Colba::draw() {
    // Front Face
    glSetColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(-w / 2 + offset, h, d / 2 + offset);
    glVertex3f(w / 2 - offset, h, d / 2 + offset);
    glVertex3f(w / 2 - offset, h - 2, d / 2 + offset);
    glVertex3f(-w / 2 + offset, h - 2, d / 2 + offset);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(-w / 2 + offset, 0, d / 2 + offset);
    glVertex3f(w / 2 - offset, 0, d / 2 + offset);
    glVertex3f(w / 2 - offset, 0.75, d / 2 + offset);
    glVertex3f(-w / 2 + offset, 0.75, d / 2 + offset);
    glEnd();

    // Left Face
    glSetColor3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex3f(-w / 2 + offset, 0, d / 2 - offset);
    glVertex3f(-w / 2 + offset, 0, -d / 2 - offset);
    glVertex3f(-w / 2 + offset, h + 2, -d / 2 - offset);
    glVertex3f(-w / 2 + offset, h + 2, d / 2 - offset);
    glEnd();

    // Right Face
    glSetColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(w / 2 - offset, 0, d / 2 - offset);
    glVertex3f(w / 2 - offset, 0, -d / 2 - offset);
    glVertex3f(w / 2 - offset, h + 2, -d / 2 - offset);
    glVertex3f(w / 2 - offset, h + 2, d / 2 - offset);
    glEnd();

    // Back Face
    glSetColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(-w / 2 + offset, 0, -d / 2 + offset);
    glVertex3f(w / 2 - offset, 0, -d / 2 + offset);
    glVertex3f(w / 2 - offset, h, -d / 2 + offset);
    glVertex3f(-w / 2 + offset, h, -d / 2 + offset);
    glEnd();

    glColor3f(1, 1, 1); // إعادة تعيين اللون الافتراضي
}

void Colba::setBack(int back) {
    if (back != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, back);
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0); glVertex3f(-w / 2 + offset, 0, -d / 2 + offset);
        glTexCoord2d(1.0, 0.0); glVertex3f(w / 2 - offset, 0, -d / 2 + offset);
        glTexCoord2d(1.0, 1.0); glVertex3f(w / 2 - offset, h, -d / 2 + offset);
        glTexCoord2d(0.0, 1.0); glVertex3f(-w / 2 + offset, h, -d / 2 + offset);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

void Colba::setFront(int front1, int front2) {
    if (front1 != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,front1);
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 1.0); glVertex3f(-w / 2 + offset, h, d / 2 + offset);
        glTexCoord2d(1.0, 1.0); glVertex3f(w / 2 - offset, h, d / 2 + offset);
        glTexCoord2d(1.0, 0.0); glVertex3f(w / 2 - offset, h - 2, d / 2 + offset);
        glTexCoord2d(0.0, 0.0); glVertex3f(-w / 2 + offset, h - 2, d / 2 + offset);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    if (front2 != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, front2);
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0); glVertex3f(-w / 2 + offset, 0, d / 2 + offset);
        glTexCoord2d(1.0, 0.0); glVertex3f(w / 2 - offset, 0, d / 2 + offset);
        glTexCoord2d(1.0, 0.25); glVertex3f(w / 2 - offset, 0.75, d / 2 + offset);
        glTexCoord2d(0.0, 0.25); glVertex3f(-w / 2 + offset, 0.75, d / 2 + offset);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
}

void Colba::setRight(int right) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, right);
        glBegin(GL_QUADS);
        glTexCoord2d(0.0, 0.0); glVertex3f(w / 2 - offset, 0, d / 2 + offset);
        glTexCoord2d(1.0, 0.0); glVertex3f(w / 2 - offset, 0, -d / 2 - offset);
        glTexCoord2d(1.0, 1.0); glVertex3f(w / 2 - offset, h + 2, -d / 2 - offset);
        glTexCoord2d(0.0, 1.0); glVertex3f(w / 2 - offset, h + 2, d / 2 + offset);
        glEnd();
        glDisable(GL_TEXTURE_2D);
}

void Colba::setLeft(int left) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, left);
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex3f(-w / 2 + offset, 0, d / 2 + offset);
    glTexCoord2d(1.0, 0.0); glVertex3f(-w / 2 + offset, 0, -d / 2 - offset);
    glTexCoord2d(1.0, 1.0); glVertex3f(-w / 2 + offset, h + 2, -d / 2 - offset);
    glTexCoord2d(0.0, 1.0); glVertex3f(-w / 2 + offset, h + 2, d / 2 + offset);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

