#include <GL/glut.h>
#include <vector>
#include "objectUtils.h"
#include "globals.h" // Include the globals header file
#include "renderUtils.h" // Include the render utils header file for renderScene declaration


void pickObject(GLFWwindow* window, int x, int y) {

}

std::vector<glm::vec3> generatePoints(int k) {
    std::vector<glm::vec3> points;
    for (int i = 0; i < k; ++i) {
        points.push_back(glm::vec3(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5));
    }
    return points;
}

void drawPoints(const std::vector<glm::vec3>& points) {
    glBegin(GL_POINTS);
    for (const auto& point : points) {
        glVertex3f(point.x, point.y, point.z);
    }
    glEnd();
}