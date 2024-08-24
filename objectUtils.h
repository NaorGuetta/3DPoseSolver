#pragma once
#ifndef OBJECT_UTILS_H
#define OBJECT_UTILS_H

#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

void pickObject(GLFWwindow* window, int x, int y);
std::vector<glm::vec3> generatePoints(int k);
void drawPoints(const std::vector<glm::vec3>& points);

#endif // OBJECT_UTILS_H