#include "globals.h"

// Window dimensions
const int WIDTH = 1152;
const int HEIGHT = 648;

// Camera transformation
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Camera rotation
float yaw = -90.0f; // Initialize yaw to -90.0 degrees to align with the initial cameraFront
float pitch = 0.0f;

// Saved camera state
glm::vec3 savedCameraPos;
glm::vec3 savedCameraFront;
float savedYaw;
float savedPitch;


// Object transformation
glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
float objectRotationX = 0.0f;
float objectRotationY = 0.0f;

// Image storage
std::vector<unsigned char> image;

// Saved camera states
std::vector<CameraState> savedCameraStates;
int currentCameraStateIndex = 0;
bool loadState = false;
bool pickingState = false;
bool chooseState = false;

std::vector<glm::vec3> cameraPath;

CameraState lastState;
CameraState lastPickingState;
CameraState cameraPnpSol;

KeyState keyState;



// Initialize the vector to store click coordinates
std::vector<ClickCoords> clickCoordinates;

ClickData myClickData;

glm::vec3 colors[8] = {
    glm::vec3(1.0f, 0.0f, 0.0f), // Red
    glm::vec3(0.0f, 1.0f, 0.0f), // Green
    glm::vec3(0.0f, 0.0f, 1.0f), // Blue
    glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
    glm::vec3(0.0f, 1.0f, 1.0f), // Cyan
    glm::vec3(1.0f, 0.0f, 1.0f), // Magenta
    glm::vec3(1.0f, 0.5f, 0.0f), // Orange
    glm::vec3(0.5f, 0.0f, 0.5f)  // Purple
};

Mode currentMode = Mode::NONE;

bool captureflag = 0;

CameraState estimated;
std::vector<CameraState> saveEstimatedPnp;