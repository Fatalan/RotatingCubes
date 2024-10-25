#include <stdlib.h>
#include <vector>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <random>
#include <set>
#include <unordered_set>

template<> struct std::equal_to<glm::vec3> {
    using argument_type = glm::vec3;
    using result_type = bool;
    constexpr bool operator()(const glm::vec3& lhs, const glm::vec3& rhs) const {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
    }
};

bool operator==(const glm::vec3& lhs, const glm::vec3& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
}

struct cube {
    std::vector<glm::vec3>* points = new std::vector<glm::vec3>();
};

struct Hash {
    size_t operator()(const glm::vec3& vec) const {
        return std::hash<float>{}(vec.x) + std::hash<float>{}(vec.y) + std::hash<float>{}(vec.z);
    }
};

std::vector<cube>* cubes = new std::vector<cube>();
float step = 0.01;

void DrawCube(cube cub) {
    int count = 0;
    std::vector<glm::vec3> points = *cub.points;
    int n = points.size();
    //std::unordered_set<std::unordered_set<glm::vec3, Hash>>* drawn = new std::unordered_set<std::unordered_set<glm::vec3, Hash>>();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                std::unordered_set<float>* xset = new std::unordered_set<float>();
                std::unordered_set<float>* yset = new std::unordered_set<float>();
                std::unordered_set<float>* zset = new std::unordered_set<float>();
                xset->insert(xset->begin(), points[i].x);
                xset->insert(xset->begin(), points[j].x);
                xset->insert(xset->begin(), points[k].x);
                yset->insert(yset->begin(), points[i].y);
                yset->insert(yset->begin(), points[j].y);
                yset->insert(yset->begin(), points[k].y);
                zset->insert(zset->begin(), points[i].z);
                zset->insert(zset->begin(), points[j].z);
                zset->insert(zset->begin(), points[k].z);
                //std::unordered_set<glm::vec3, Hash>* vectorsSet = new std::unordered_set<glm::vec3, Hash>();
                if (xset->size() + yset->size() + zset->size() == 5 /*&& drawn->find(*vectorsSet) == drawn->end()*/)
                {
                    glBegin(GL_POLYGON);
                    glColor3f(1, 0, 0); glVertex3f(points[i].x, points[i].y, points[i].z);
                    glColor3f(0, 1, 0); glVertex3f(points[j].x, points[j].y, points[j].z);
                    glColor3f(0, 0, 1); glVertex3f(points[k].x, points[k].y, points[k].z);
                    glEnd();
                    //vectorsSet->insert(vectorsSet->begin(), points[i]);
                    //vectorsSet->insert(vectorsSet->begin(), points[j]);
                    //vectorsSet->insert(vectorsSet->begin(), points[k]);
                    //drawn->insert(drawn->begin(), *vectorsSet);
                    count++;
                }
                delete xset;
                delete yset;
                delete zset;
            }
        }
    }
    //delete drawn;
    printf("%i\n", count);
}

void MoveCube(cube& cub) {
    std::vector<glm::vec3> points = *cub.points;
    std::vector<glm::vec3>* newPoints = new std::vector<glm::vec3>();
    if (points[0].x > 2) {
        step = -0.1;
    }
    else if(points[0].x < -2) {
        step = 0.1;
    }
    for (unsigned int i = 0; i < points.size(); i++) {
        newPoints->insert(newPoints->begin(), glm::vec3(points[i].x + step, points[i].y + step, points[i].z));
    }
    std::vector<glm::vec3>* temp = cub.points;
    cub.points = newPoints;
    delete temp;
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
    cube* cub = new cube();
    cub->points = new std::vector<glm::vec3>();
    cub->points->insert(cub->points->begin(), glm::vec3(0, 0, 0));
    cub->points->insert(cub->points->begin(), glm::vec3(0, 1, 0));
    cub->points->insert(cub->points->begin(), glm::vec3(1, 0, 0));
    cub->points->insert(cub->points->begin(), glm::vec3(1, 1, 0));
    cub->points->insert(cub->points->begin(), glm::vec3(0, 0, 1));
    cub->points->insert(cub->points->begin(), glm::vec3(0, 1, 1));
    cub->points->insert(cub->points->begin(), glm::vec3(1, 0, 1));
    cub->points->insert(cub->points->begin(), glm::vec3(1, 1, 1));
    cubes->insert(cubes->begin(), *cub);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(400, 300);
    glutCreateWindow("Cubes rotation");
    glutDisplayFunc(display);
    Loop(1);
    glutMainLoop();
}
