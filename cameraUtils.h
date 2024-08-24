#pragma once
#ifndef CAMERA_UTILS_H
#define CAMERA_UTILS_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

void drawCameraViewTriangle(const glm::vec3& position, const glm::vec3& direction, bool flag);
void drawCameraPath();
struct CameraState saveCameraState();
void loadCameraImage(GLFWwindow* window);
void loadNextCameraState();
void updateCamera();
void loadCameraWithParam(glm::vec3 calculatedPos, float calcYaw, float calcPitch);
void calculateEstimate();


#endif // CAMERA_UTILS_H