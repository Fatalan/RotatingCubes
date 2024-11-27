#include <stdlib.h>
#include <vector>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <random>
#include <set>
#include <unordered_set>

#define HI 30
#define LO -30

static void DrawTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    glBegin(GL_POLYGON);
    //glColor3f(0, 0.6, 0.5); glVertex3f(p1.x, p1.y, p1.z);
    //glColor3f(0, 0.4, 0.6); glVertex3f(p2.x, p2.y, p2.z);
    //glColor3f(0, 0.5, 0.5); glVertex3f(p3.x, p3.y, p3.z);
    glColor3f(1, 0, 0); glVertex3f(p1.x, p1.y, p1.z);
    glColor3f(0, 1, 0); glVertex3f(p2.x, p2.y, p2.z);
    glColor3f(0, 0, 1); glVertex3f(p3.x, p3.y, p3.z);
    glEnd();
}

class Figure {
protected:
    float step = 0.1;
    std::vector<glm::vec3> points;
public:
    Figure() {

    }
    virtual void Draw() = 0;
    void Move() {
        if (points[0].x > 30) {
            step = -0.1;
        }
        else if (points[0].x < -30) {
            step = 0.1;
        }
        for (unsigned int i = 0; i < points.size(); i++) {
            points[i].x += step;
            points[i].y += step;
        }
    }
    virtual void Rotate(float angle) = 0;
};

class Cube : Figure {
public:
    Cube(glm::vec3 b1, glm::vec3 b2, glm::vec3 b3, glm::vec3 b4, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, glm::vec3 t4) {
        points.insert(points.end(), b1);
        points.insert(points.end(), b2);
        points.insert(points.end(), b3);
        points.insert(points.end(), b4);
        points.insert(points.end(), t1);
        points.insert(points.end(), t2);
        points.insert(points.end(), t3);
        points.insert(points.end(), t4);
    }
    Cube(glm::vec3 center, float length) {
        points.insert(points.end(), glm::vec3(center.x - length / 2, center.y - length / 2, center.z - length / 2));
        points.insert(points.end(), glm::vec3(center.x - length / 2, center.y - length / 2, center.z + length / 2));
        points.insert(points.end(), glm::vec3(center.x + length / 2, center.y - length / 2, center.z - length / 2));
        points.insert(points.end(), glm::vec3(center.x + length / 2, center.y - length / 2, center.z + length / 2));
        points.insert(points.end(), glm::vec3(center.x - length / 2, center.y + length / 2, center.z - length / 2));
        points.insert(points.end(), glm::vec3(center.x - length / 2, center.y + length / 2, center.z + length / 2));
        points.insert(points.end(), glm::vec3(center.x + length / 2, center.y + length / 2, center.z - length / 2));
        points.insert(points.end(), glm::vec3(center.x + length / 2, center.y + length / 2, center.z + length / 2));
    }
    void Draw() {
        //Нижняя грань:
        DrawTriangle(points[1], points[0], points[2]);
        DrawTriangle(points[1], points[3], points[2]);
        //Верхняя грань:
        DrawTriangle(points[5], points[4], points[6]);
        DrawTriangle(points[5], points[7], points[6]);
        //Боковая грань(дальняя правая):
        DrawTriangle(points[1], points[0], points[4]);
        DrawTriangle(points[1], points[5], points[4]);
        //Боковая грань(ближняя правая):
        DrawTriangle(points[3], points[1], points[5]);
        DrawTriangle(points[3], points[7], points[5]);
        //Боковая грань(дальняя левая):
        DrawTriangle(points[2], points[0], points[4]);
        DrawTriangle(points[2], points[6], points[4]);
        //Боковая грань(ближняя левая):
        DrawTriangle(points[3], points[2], points[6]);
        DrawTriangle(points[3], points[7], points[6]);
    }
    void Rotate(float angle) {
        glm::mat4 rotationMatrix(1);
        rotationMatrix = glm::rotate(rotationMatrix, angle, glm::vec3(0, 1, 0));
        for (unsigned int i = 0; i < points.size(); i++) {
            points[i] = glm::vec3(rotationMatrix * glm::vec4(points[i], 1.0));
        }
        rotationMatrix = glm::mat4(1);
        rotationMatrix = glm::rotate(rotationMatrix, angle, glm::vec3(1, 0, 0));
        for (unsigned int i = 0; i < points.size(); i++) {
            points[i] = glm::vec3(rotationMatrix * glm::vec4(points[i], 1.0));
        }
        rotationMatrix = glm::mat4(1);
        rotationMatrix = glm::rotate(rotationMatrix, angle, glm::vec3(0, 0, 1));
        for (unsigned int i = 0; i < points.size(); i++) {
            points[i] = glm::vec3(rotationMatrix * glm::vec4(points[i], 1.0));
        }
    }
};

class Pyramid : Figure {
public:
    Pyramid(glm::vec3 b1, glm::vec3 b2, glm::vec3 b3, glm::vec3 b4, glm::vec3 t) {
        points.insert(points.end(), b1);
        points.insert(points.end(), b2);
        points.insert(points.end(), b3);
        points.insert(points.end(), b4);
        points.insert(points.end(), t);
    }
    void Draw() {
        DrawTriangle(points[4], points[1], points[0]);
        DrawTriangle(points[4], points[2], points[0]);
        DrawTriangle(points[4], points[3], points[2]);
        DrawTriangle(points[4], points[3], points[1]);
        DrawTriangle(points[0], points[2], points[3]);
        DrawTriangle(points[0], points[1], points[3]);
    }
    void Rotate(float angle = 0) {
        glm::mat4 rotationMatrix(1);
        glm::vec3 centerOfThePyramid = (points[0] + glm::vec3((points[3] - points[0]).x / 2, (points[3] - points[0]).y / 2, (points[3] - points[0]).z / 2));
        centerOfThePyramid = (centerOfThePyramid + glm::vec3((points[4] - centerOfThePyramid).x / 2, (points[4] - centerOfThePyramid).y / 2, (points[4] - centerOfThePyramid).z / 2));
        rotationMatrix = glm::rotate(rotationMatrix, 0.02f, centerOfThePyramid);
        for (unsigned int i = 0; i < points.size(); i++) {
            points[i] = glm::vec3(rotationMatrix * glm::vec4(points[i], 1.0));
        }
    }
};

std::vector<Figure*>* figures = new std::vector<Figure*>();
float BackGroundR = 0;
float BackGroundB = 0;
float BackGroundG = 0;
float EyeX = 10;
float EyeY = -5;
float EyeZ = 5;
float lookAtX = 0;
float lookAtY = 0.5;
float lookAtZ = 0.5;
bool DoRotation = true;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(BackGroundR, BackGroundB, BackGroundG, 1);
    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 0.1f, 100.0f); // Adjust near and far planes

    // Set up modelview
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(EyeX, EyeY, EyeZ,   // Camera position
        lookAtX, lookAtY, lookAtZ, // Look at point
        0.0, 1.0, 0.0); // Up vector
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    for (unsigned int i = 0; i < figures->size(); i++) {
        float b = i;
        (*figures)[i][0].Draw();
        (*figures)[i][0].Move();
        if(DoRotation)
            (*figures)[i][0].Rotate(0.02 + (b / 100));
    }
    glFlush();
}

void Loop(int i)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, Loop, 0);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'c':
            BackGroundR = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            BackGroundG = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            BackGroundB = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            break;
        case 'f':
            EyeX = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
            EyeY = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
            EyeZ = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
            lookAtX = 0;
            lookAtY = 0.5;
            lookAtZ = 0.5;
            break;
        case 'w':
            EyeX += 1;
            lookAtX += 1;
            break;
        case 's':
            EyeX -= 1;
            lookAtX -= 1;
            break;
        case 'a':
            EyeZ -= 1;
            lookAtZ -= 1;
            break;
        case 'd':
            EyeZ += 1;
            lookAtZ += 1;
            break;
        case 'z':
            EyeY += 1;
            lookAtY += 1;
            break;
        case 'x':
            EyeY -= 1;
            lookAtY -= 1;
            break;
        case 'k':
            DoRotation = !DoRotation;
            break;
        default:
            break;
    }

}


int main(int argc, char** argv) {
    Cube* cub = new Cube(
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, 1),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 1),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 1),
        glm::vec3(1, 1, 0),
        glm::vec3(1, 1, 1)
    );
    figures->insert(figures->begin(), (Figure*)cub);
    Cube* cub2 = new Cube(
        glm::vec3(2, 0, 2),
        glm::vec3(2, 0, 3),
        glm::vec3(3, 0, 2),
        glm::vec3(3, 0, 3),
        glm::vec3(2, 1, 2),
        glm::vec3(2, 1, 3),
        glm::vec3(3, 1, 2),
        glm::vec3(3, 1, 3)
    );
    figures->insert(figures->begin(), (Figure*)cub2);
    Cube* cub3 = new Cube(
        glm::vec3(-2, 0, -2),
        glm::vec3(-2, 0, -1),
        glm::vec3(-1, 0, -2),
        glm::vec3(-1, 0, -1),
        glm::vec3(-2, 1, -2),
        glm::vec3(-2, 1, -1),
        glm::vec3(-1, 1, -2),
        glm::vec3(-1, 1, -1)
    );
    figures->insert(figures->begin(), (Figure*)cub3);
    Cube* cub4 = new Cube(
        glm::vec3(0, 2, 0),
        glm::vec3(0, 2, 1),
        glm::vec3(1, 2, 0),
        glm::vec3(1, 2, 1),
        glm::vec3(0, 3, 0),
        glm::vec3(0, 3, 1),
        glm::vec3(1, 3, 0),
        glm::vec3(1, 3, 1)
    );
    figures->insert(figures->begin(), (Figure*)cub4);
    figures->insert(figures->begin(), 
        (Figure*)new Cube(
            glm::vec3(0, -15, 0),
            glm::vec3(0, -15, 10),
            glm::vec3(10, -15, 0),
            glm::vec3(10, -15, 10),
            glm::vec3(0, -5, 0),
            glm::vec3(0, -5, 10),
            glm::vec3(10, -5, 0),
            glm::vec3(10, -5, 10)
            )
        );
    figures->insert(figures->begin(), (Figure*)new Cube(glm::vec3(6, 6, 6), 0.05));
    figures->insert(figures->begin(), (Figure*)new Cube(glm::vec3(6, 6, 6), 10));
    Pyramid* pyramid1 = new Pyramid(
        glm::vec3(5, 0, 0),
        glm::vec3(5, 0, 1),
        glm::vec3(6, 0, 0),
        glm::vec3(6, 0, 1),
        glm::vec3(5.5, 1, 0.5)
    );
    figures->insert(figures->begin(), (Figure*)pyramid1);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    //glutInitWindowPosition(0, 0);
    //glutInitWindowSize(600, 600);
    glutCreateWindow("Cubes rotation");
    glutKeyboardFunc(keyboard);
    glutFullScreen();
    glutDisplayFunc(display);
    Loop(1);
    glutMainLoop();
}
