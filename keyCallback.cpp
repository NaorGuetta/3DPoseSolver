#include "keyCallback.h"
#include "globals.h"
#include "cameraUtils.h"
#include "objectUtils.h"
#include <iostream>
#include <GL/freeglut_std.h>
#include "pnp.h"
#include "renderUtils.h"



void handlePickingMode(unsigned char key) {
    if (key == 'p' || key == 'P') {
        if (savedCameraStates.size() == 0) {
            std::cout << "Error: No pictures have been taken !" << std::endl;
            return;
        }
        pickingState = !pickingState;
        if (pickingState) {
            std::cout << "Entering camera state picking mode." << std::endl;
            lastState.position = cameraPos;
            lastState.front = cameraFront;
            lastState.yaw = yaw;
            lastState.pitch = pitch;
        }
        else {
            std::cout << "Exiting camera state picking mode." << std::endl;
            cameraPos = lastState.position;
            cameraFront = lastState.front;
            yaw = lastState.yaw;
            pitch = lastState.pitch;

            // Recalculate the camera front vector based on the loaded yaw and pitch
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(front);

            currentMode = Mode::NONE;
        }
    }
    if (key == 'c' || key == 'C') { // i am in picking mode and pressed C
        if (myClickData.numLeft == 0) {
            std::cout << "cannot choose, not points have been picked." << std::endl;
            return;
        }
        else if (myClickData.numLeft != myClickData.numRight) {
            std::cout << "number of 2d and 3d points do not match! pick the same number of points." << std::endl;
            return;
        }
        else if (myClickData.numLeft < 4) {
            std::cout << "Number of 2d and 3d points must be more than 4 for position estimation. Pick more points." << std::endl;
            return;
        }
        currentMode = Mode::CHOOSE;
        handleChoosestateMode(key);
    }
}

void handleChoosestateMode(unsigned char key) {
    if (key == 'c' || key == 'C') {
        chooseState = !chooseState;
        if (!chooseState) {
            captureflag = 0;
        }
    }
}


void handleLoadStateMode(unsigned char key) {
    if (key == 'r' || key == 'R') {
        if (savedCameraStates.size() == 0) {
            std::cout << "Error: No pictures have been taken !" << std::endl;
            return;
        }
        loadState = !loadState;
        if (loadState) {
            std::cout << "Entering camera state load mode." << std::endl;
            lastState.position = cameraPos;
            lastState.front = cameraFront;
            lastState.yaw = yaw;
            lastState.pitch = pitch;
            loadNextCameraState();
        }
        else {
            std::cout << "Exiting camera state load mode." << std::endl;
            cameraPos = lastState.position;
            cameraFront = lastState.front;
            yaw = lastState.yaw;
            pitch = lastState.pitch;

            // Recalculate the camera front vector based on the loaded yaw and pitch
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(front);
            currentMode = Mode::NONE; // Exit load state mode
        }

    }

}

void keyCallback(unsigned char key, int x, int y) {
    // Handle mode selection
    if (currentMode == Mode::NONE) {
        if (key == 'p' || key == 'P') {
            currentMode = Mode::PICKING;
            std::cout << "Switching to picking mode." << std::endl;
            handlePickingMode(key);
            return;
        }
        else if (key == 'r' || key == 'R') {
            currentMode = Mode::LOAD_STATE;
            std::cout << "Switching to load state mode." << std::endl;
            handleLoadStateMode(key);
            return;
        }
    }

    // Check if a mode is active and handle mode-specific logic
    if (currentMode == Mode::PICKING) {
        handlePickingMode(key);
        if (!pickingState) {
            currentMode = Mode::NONE; // Exit picking mode
        }
        return;
    }
    else if (currentMode == Mode::CHOOSE) {
        handleChoosestateMode(key);
        if (!chooseState) {
            // clear all selected points on Exit
            clearPointsPicked();
            currentMode = Mode::PICKING;
        }
    }
    else if (currentMode == Mode::LOAD_STATE) {
        handleLoadStateMode(key);
        if (!loadState) {
            currentMode = Mode::NONE; // Exit load state mode
        }
        return;
    }
    else if (key == 'b' || key == 'B') {
        savedCameraStates.push_back(saveCameraState());
    }
}

void updateKeyState(unsigned char key, int x, int y) {
    if (loadState || chooseState) {// we send here in order to disable movement
        keyCallback(key, x, y);
        return;
    }
    switch (key) {
    case 'P': case 'p': case 'b': case 'B': case 'r': case 'R': case 'c':case 'C': keyCallback(key, x, y); break;
    case 'w': case 'W': keyState.W = true; break;
    case 'a': case 'A': keyState.A = true; break;
    case 's': case 'S': keyState.S = true; break;
    case 'd': case 'D': keyState.D = true; break;
    }
}

void updateKeyUp(unsigned char key, int x, int y) {
    if (loadState || chooseState) {
        return;
    }
    switch (key) {
    case 'P': case 'p': case 'b': case 'B': case 'r': case 'R':case 'c':case 'C':  break;
    case 'w': case 'W': keyState.W = false; break;
    case 'a': case 'A': keyState.A = false; break;
    case 's': case 'S': keyState.S = false; break;
    case 'd': case 'D': keyState.D = false; break;
    }
}

void updateSpecialKeyState(int key, int x, int y) {
    if (chooseState) {
        return;
    }
    if (loadState) {
        if (key == GLUT_KEY_RIGHT) {
            currentCameraStateIndex++;
            loadNextCameraState();
            return; // Early return to prevent further processing of this key event
        }
        else if (key == GLUT_KEY_LEFT) {
            currentCameraStateIndex--;
            if (currentCameraStateIndex < 0) {
                currentCameraStateIndex = (int)savedCameraStates.size() - 1;
            }
            loadNextCameraState();
            return; // Early return to prevent further processing of this key event
        }
    }
    switch (key) {
    case GLUT_KEY_LEFT: keyState.Left = true; break;
    case GLUT_KEY_RIGHT: keyState.Right = true; break;
    case GLUT_KEY_UP: keyState.Up = true; break;
    case GLUT_KEY_DOWN: keyState.Down = true; break;
    }
}

void updateSpecialKeyUp(int key, int x, int y) {
    if (loadState || chooseState) {
        return; // Early return to prevent further processing of this key event
    }
    switch (key) {
    case GLUT_KEY_LEFT: keyState.Left = false; break;
    case GLUT_KEY_RIGHT: keyState.Right = false; break;
    case GLUT_KEY_UP: keyState.Up = false; break;
    case GLUT_KEY_DOWN: keyState.Down = false; break;
    }
}

void clearPointsPicked() {
    myClickData.leftSideClicks.clear();
    myClickData.rightSideClicks.clear();
    myClickData.numLeft = 0;
    myClickData.numRight = 0;

}