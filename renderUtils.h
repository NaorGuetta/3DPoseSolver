#pragma once
#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include <GLFW/glfw3.h>

void renderScene();

void renderNormalMode();
void renderLeftNormalViewport();
void renderRightNormalViewport();

void renderPickingMode();
void renderLeftPickingViewport();
void renderRightPickingViewport();

void renderChooseMode();
void renderLeftChooseViewport();
void renderRightChooseViewport();


void drawDividerLine();
glm::vec3 getClickedPoint(int x, int y);
void setupRightViewport();
void drawSpheres(const glm::vec3& threeDPoint, glm::vec3& color);



#endif // RENDER_UTILS_H