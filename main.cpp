#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "globals.h"
#include "drawFunctions.h"
#include "cameraUtils.h"
#include "objectUtils.h"
#include "renderUtils.h"
#include "keyCallback.h"


// Function declarations
void setupScene();

void mouseButtonCallback(int button, int state, int x, int y);

GLFWwindow* window;
std::vector<glm::vec3> points;


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL Project");

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error: GLEW initialization failed: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    setupScene();
    points = generatePoints(10); // Generate 10 random points


    glutDisplayFunc(renderScene);

    //handle  on-mouseclick glut
    glutMouseFunc(mouseButtonCallback);

    //handling keypresses for glut input
    glutKeyboardFunc(updateKeyState);
    glutKeyboardUpFunc(updateKeyUp);
    glutSpecialFunc(updateSpecialKeyState);
    glutSpecialUpFunc(updateSpecialKeyUp);

    // Set the idle callback to continuously update the camera
    glutIdleFunc([]() {
        updateCamera();
        glutPostRedisplay(); // Request a redraw after updating the camera
        });

    glutMainLoop();

    return 0;
}

void setupScene() {
    glEnable(GL_DEPTH_TEST);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Define light properties
    GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void mouseButtonCallback(int button, int state, int x, int y) {
    if (pickingState) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

            // Check if the click is on the right side of the window
            if (x >= WIDTH / 2) {
                // Translate mouse coordinates to 3D world space

                glm::vec3 threeDPos = getClickedPoint(x, y);
                myClickData.addRightClick(threeDPos);

            }
            else {
                // Save the click for the left side
                myClickData.addLeftClick(x, y);
            }
        }
    }
}




//glm::vec3 calculateMouseRay(double mouseX, double mouseY) {
//    // Convert mouse coordinates to NDC
//    double x = (2.0 * mouseX) / WIDTH - 1.0;
//    double y = 1.0 - (2.0 * mouseY) / HEIGHT;
//    double z = -1.0; // Assuming near plane is at z = -1.0
//    float aspectRatio = WIDTH / static_cast<float>(HEIGHT); // Adjust WIDTH and HEIGHT accordingly
//
//    // Define clipping planes
//    float nearPlane = 0.1f; // Near clipping plane distance
//    float farPlane = 100.0f; // Far clipping plane distance
//
//    // Field of view in degrees
//    float fov = 45.0f;
//
//    // Ray in normalized device coordinates (NDC)
//    glm::vec4 ray_clip = glm::vec4(x, y, z, 1.0);
//
//    // Invert the projection matrix
//    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
//    glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);
//
//    // Transform the ray from clip space to eye space
//    glm::vec4 ray_eye = inverseProjectionMatrix * ray_clip;
//    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0); // Direction in eye space
//
//    // Invert the view matrix
//    glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//    glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
//
//    // Transform the ray from eye space to world space
//    glm::vec4 ray_world = inverseViewMatrix * ray_eye;
//    glm::vec3 ray_direction = glm::normalize(glm::vec3(ray_world));
//
//    return ray_direction;
//}
