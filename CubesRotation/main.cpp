﻿#include <stdlib.h>
#include <vector>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <random>
#include <set>
#include <unordered_set>

struct cube {
    glm::vec3 b1;
    glm::vec3 b2;
    glm::vec3 b3;
    glm::vec3 b4;
    glm::vec3 t1;
    glm::vec3 t2;
    glm::vec3 t3;
    glm::vec3 t4;
};

std::vector<cube>* cubes = new std::vector<cube>();
float step = 0.01;

void DrawTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    glBegin(GL_POLYGON);
    glColor3f(1, 0, 0); glVertex3f(p1.x, p1.y, p1.z);
    glColor3f(0, 1, 0); glVertex3f(p2.x, p2.y, p2.z);
    glColor3f(0, 0, 1); glVertex3f(p3.x, p3.y, p3.z);
    glEnd();
}

void DrawCube(cube cub) {
    //Нижняя грань:
    DrawTriangle(cub.b1, cub.b2, cub.b3);
    DrawTriangle(cub.b2, cub.b3, cub.b4);
    //Верхняя грань:
    DrawTriangle(cub.t1, cub.t2, cub.t3);
    DrawTriangle(cub.t2, cub.t3, cub.t4);
    //Боковая грань(дальняя правая):
    DrawTriangle(cub.b1, cub.b2, cub.t1);
    DrawTriangle(cub.b2, cub.t1, cub.t2);
    //Боковая грань(ближняя правая):
    DrawTriangle(cub.b2, cub.b4, cub.t2);
    DrawTriangle(cub.b4, cub.t2, cub.t4);
    //Боковая грань(дальняя левая):
    DrawTriangle(cub.b1, cub.b3, cub.t1);
    DrawTriangle(cub.b3, cub.t1, cub.t3);
    //Боковая грань(ближняя левая):
    DrawTriangle(cub.b3, cub.b4, cub.t3);
    DrawTriangle(cub.b4, cub.t3, cub.t4);
}

void MoveCube(cube& cub) {
    if (cub.b1.x > 1) {
        step = -0.1;
    }
    else if(cub.b1.x < -1) {
        step = 0.1;
    }
    cub.b1.x += step;
    cub.b1.y += step;
    cub.b2.x += step;
    cub.b2.y += step;
    cub.b3.x += step;
    cub.b3.y += step;
    cub.b4.x += step;
    cub.b4.y += step;
    cub.t1.x += step;
    cub.t1.y += step;
    cub.t2.x += step;
    cub.t2.y += step;
    cub.t3.x += step;
    cub.t3.y += step;
    cub.t4.x += step;
    cub.t4.y += step;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 0, 0.5, 0.5, 0.5, 0, 1.0, 0);
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    for (unsigned int i = 0; i < cubes->size(); i++) {
        DrawCube(cubes[i][0]);
        MoveCube(cubes[i][0]);
    }
    glFlush();
}

void Loop(int i)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, Loop, 0);
}

int main(int argc, char** argv) {
    cube cub = *new cube();
    cub.b1 = glm::vec3(0, 0, 0);
    cub.b2 = glm::vec3(0, 0, 1);
    cub.b3 = glm::vec3(1, 0, 0);
    cub.b4 = glm::vec3(1, 0, 1);
    cub.t1 = glm::vec3(0, 1, 0);
    cub.t2 = glm::vec3(0, 1, 1);
    cub.t3 = glm::vec3(1, 1, 0);
    cub.t4 = glm::vec3(1, 1, 1);
    cubes->insert(cubes->begin(), cub);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Cubes rotation");
    glutDisplayFunc(display);
    Loop(1);
    glutMainLoop();
}
