#include <GL/glut.h>
#include <glm/gtc/type_ptr.hpp>
#include "renderUtils.h"
#include "globals.h"
#include "drawFunctions.h"
#include "cameraUtils.h"
#include "pnp.h"



void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (currentMode) {
    case Mode::PICKING: renderPickingMode();
        break;
    case Mode::CHOOSE: renderChooseMode();
        break;
    default: renderNormalMode();
    }

    drawDividerLine();
    glutSwapBuffers(); // Use the passed window parameter
}



/*******************************************    NormalMode    ***************************************************/
void renderNormalMode() {
    renderLeftNormalViewport();
    renderRightNormalViewport();
}


void renderLeftNormalViewport() {
    // Left view (Global view)
    glViewport(0, 0, WIDTH / 2, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(WIDTH / 2) / (double)HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 globalView = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), objectPos, cameraUp);
    glLoadMatrixf(glm::value_ptr(globalView));

    // Apply object transformations for normal mode
    glPushMatrix();
    glTranslatef(objectPos.x, objectPos.y, objectPos.z);
    glRotatef(glm::degrees(objectRotationX), 1.0f, 0.0f, 0.0f);
    glRotatef(glm::degrees(objectRotationY), 0.0f, 1.0f, 0.0f);
    drawTeapot();
    glPopMatrix();

    // Draw the camera path
    if (!pickingState) {
        drawCameraPath();
    }
    if (!savedCameraStates.empty()) {
        for (size_t i = 0; i < savedCameraStates.size(); ++i) {
            bool flag = (currentCameraStateIndex == i);
            drawCameraViewTriangle(savedCameraStates[i].position, savedCameraStates[i].front, flag);
        }
    }
}

void renderRightNormalViewport() {
    // Right view (Camera view)
    glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(WIDTH / 2) / (double)HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glLoadMatrixf(glm::value_ptr(cameraView));



    // Apply object transformations for normal mode
    glPushMatrix();
    glTranslatef(objectPos.x, objectPos.y, objectPos.z);
    glRotatef(glm::degrees(objectRotationX), 1.0f, 0.0f, 0.0f);
    glRotatef(glm::degrees(objectRotationY), 0.0f, 1.0f, 0.0f);
    drawTeapot();
    glPopMatrix();
}


/*******************************************    PickingMode    ***************************************************/
void renderPickingMode() {
    renderLeftPickingViewport();
    renderRightPickingViewport();
}


void renderLeftPickingViewport() {
    // Ensure we have a saved camera state
    if (savedCameraStates.empty()) {
        std::cerr << "No saved camera states available!" << std::endl;
        return;
    }

    // Retrieve the image from the first saved camera state
    CameraState firstState = savedCameraStates[0];
    std::vector<unsigned char>& image = firstState.image;


    // Set viewport to the left half of the window
    glViewport(0, 0, WIDTH / 2, HEIGHT);

    // Draw the image
    glDrawPixels(WIDTH / 2, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image.data());

    // Save the current projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH / 2, 0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth test and enable blending
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw the saved clicks as colored circles
    float radius = 7.0f; // Radius of the circles
    int numSegments = 36; // Number of segments for the circle


    glDisable(GL_LIGHTING);
    for (size_t i = 0; i < myClickData.leftSideClicks.size(); ++i) {

        glm::vec2 clickPos = myClickData.leftSideClicks[i];

        glm::vec3 color = colors[i % 8];
        glColor3f(color.x, color.y, color.z);
        glBegin(GL_POLYGON);
        for (int j = 0; j < numSegments; ++j) {
            float theta = 2.0f * 3.1415926f * float(j) / float(numSegments);
            float dx = radius * cosf(theta);
            float dy = radius * sinf(theta);
            // Adjust y-coordinate if needed (OpenGL coordinates might differ)
            glVertex2f(clickPos.x + dx, HEIGHT - clickPos.y + dy);
        }
        glEnd();

    }
    glEnable(GL_LIGHTING);

    // Restore the original matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable depth test if it was previously enabled
    glEnable(GL_DEPTH_TEST);
}

void renderRightPickingViewport() {
    setupRightViewport();
    for (size_t i = 0; i < myClickData.rightSideClicks.size(); ++i) {
        glm::vec3 clickPos = myClickData.rightSideClicks[i];
        glm::vec3 color = colors[(int)i % 8];
        drawSpheres(clickPos, color); // Render the sphere at the clicked point
    }
    // Save current matrix state
    glPushMatrix();

    // Apply object transformations for normal mode (same as in renderRightNormalViewport)
    glTranslatef(objectPos.x, objectPos.y, objectPos.z);
    glRotatef(glm::degrees(objectRotationX), 1.0f, 0.0f, 0.0f);
    glRotatef(glm::degrees(objectRotationY), 0.0f, 1.0f, 0.0f);

    // Draw teapot (if needed for picking)
    drawTeapot();

    // Render picked spheres on the teapot's surface
    glDisable(GL_LIGHTING); // Disable lighting for sphere color consistency
    glEnable(GL_LIGHTING); // Re-enable lighting after rendering spheres

    // Restore previous matrix state
    glPopMatrix();
}


/*******************************************    ChooseMode    ***************************************************/

void renderChooseMode() {
    if (!captureflag)
    {
        //setup viewpoint
        glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (double)(WIDTH / 2) / (double)HEIGHT, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //calculate after pnp
        calculateEstimate();

        glm::mat4 cameraView = glm::lookAt(estimated.position, estimated.position + estimated.front, cameraUp);
        glLoadMatrixf(glm::value_ptr(cameraView));
        glPushMatrix();

        // Apply object transformations for normal mode (same as in renderRightNormalViewport)
        glTranslatef(objectPos.x, objectPos.y, objectPos.z);
        glRotatef(glm::degrees(objectRotationX), 1.0f, 0.0f, 0.0f);
        glRotatef(glm::degrees(objectRotationY), 0.0f, 1.0f, 0.0f);

        // Draw teapot (if needed for picking)
        drawTeapot();

        // Render picked spheres on the teapot's surface
        glDisable(GL_LIGHTING); // Disable lighting for sphere color consistency
        glEnable(GL_LIGHTING); // Re-enable lighting after rendering spheres

        // Restore previous matrix state
        glPopMatrix();
        int width = WIDTH / 2;
        int height = HEIGHT;
        std::vector<unsigned char> image(static_cast<size_t>(3) * static_cast<size_t>(width) * static_cast<size_t>(height));
        glReadPixels(width, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image.data());
        estimated.image = image;

        //save the estimated position and image for later use  
        CameraState tempState = { estimated.position,estimated.front ,estimated.yaw ,estimated.pitch ,estimated.image };
        saveEstimatedPnp.push_back(tempState);
        captureflag = 1;
    }
    renderLeftChooseViewport();
    renderRightChooseViewport();
}


void renderLeftChooseViewport() {
    std::vector<unsigned char>& image2 = estimated.image;
    std::vector<unsigned char>& image1 = savedCameraStates[0].image; // Assuming secondImage is defined elsewhere

    // Set viewport to the left half of the window
    glViewport(0, 0, WIDTH / 2, HEIGHT);

    // Draw the first image
    glDrawPixels(WIDTH / 2, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image1.data());

    // Save the current projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, WIDTH / 2, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Disable depth test and enable blending
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Draw the second image with an orange tint and transparency
    // Create an RGBA image from the RGB image to include the alpha channel
    std::vector<unsigned char> image2_rgba(WIDTH / 2 * HEIGHT * 4); // 4 channels: R, G, B, A
    for (size_t i = 0; i < image2.size(); i += 3) {
        image2_rgba[i * 4 / 3] = std::min(static_cast<int>(image2[i] * 1.5), 255); // Increase red channel
        image2_rgba[i * 4 / 3 + 1] = std::min(static_cast<int>(image2[i + 1] * 1.2), 255); // Increase green channel
        image2_rgba[i * 4 / 3 + 2] = image2[i + 2]; // Blue channel remains unchanged
        image2_rgba[i * 4 / 3 + 3] = 128; // Set transparency to 50%
    }
    glDrawPixels(WIDTH / 2, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, image2_rgba.data());

    // Restore the projection and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    // Re-enable depth test if it was previously enabled
    glEnable(GL_DEPTH_TEST);
}
void renderRightChooseViewport() {

    glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(WIDTH / 2) / (double)HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 globalView = glm::lookAt(glm::vec3(10.0f, 10.0f, 10.0f), objectPos, cameraUp);
    glLoadMatrixf(glm::value_ptr(globalView));

    // Apply object transformations for normal mode
    glPushMatrix();
    glTranslatef(objectPos.x, objectPos.y, objectPos.z);
    glRotatef(glm::degrees(objectRotationX), 1.0f, 0.0f, 0.0f);
    glRotatef(glm::degrees(objectRotationY), 0.0f, 1.0f, 0.0f);
    drawTeapot();
    glPopMatrix();
    drawCameraViewTriangle(estimated.position, estimated.front, false); // blue
    drawCameraViewTriangle(savedCameraStates[0].position, savedCameraStates[0].front, true); //red
    //renderRightNormalViewport();
}


/*******************************************    HelperFunctions    ***************************************************/

void drawDividerLine() {
    // Draw a vertical line in the middle
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f); // Set line color to white
    glBegin(GL_LINES);
    glVertex2f(WIDTH / 2, 0);
    glVertex2f(WIDTH / 2, HEIGHT);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void setupRightViewport() {
    // Right view (Camera view)
    glViewport(WIDTH / 2, 0, WIDTH / 2, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)(WIDTH / 2) / (double)HEIGHT, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glm::mat4 cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    glLoadMatrixf(glm::value_ptr(cameraView));
}

glm::vec3 getClickedPoint(int x, int y) {
    setupRightViewport();
    int viewport[4];
    double modelview[16];
    double projection[16];
    float winX, winY, winZ;
    double posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = static_cast<float>(x);
    winY = static_cast<float>(viewport[3] - y); // Invert Y coordinate
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    // Adjust for the position of the teapot
    glm::vec3 clickedPoint(posX, posY, posZ);
    clickedPoint -= objectPos; // Adjust for teapot's position if necessary

    return clickedPoint;
}

void drawSpheres(const glm::vec3& threeDPoint, glm::vec3& color) {
    // Define the properties of the sphere
    float sphereRadius = 0.04f; // Adjust radius as needed
    int slices = 16; // Number of slices (segments) for the sphere
    int stacks = 16; // Number of stacks (segments) for the sphere

    glDisable(GL_LIGHTING);
    // Save current matrix state
    glPushMatrix();

    // Translate to the sphere's center position relative to the teapot
    glTranslatef(threeDPoint.x, threeDPoint.y, threeDPoint.z);

    // Draw the sphere using OpenGL commands
    glColor3f(color.x, color.y, color.z); // Set sphere color (red)
    glutSolidSphere(sphereRadius, slices, stacks);

    // Restore previous matrix state
    glPopMatrix();

    glEnable(GL_LIGHTING);
}




