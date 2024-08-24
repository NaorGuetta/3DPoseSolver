## Introduction 
This project was developed as part of a university project.
This project implements a 3D rendering system using OpenGL, The system estimates camera poses based on 2D-3D correspondences using the EPnP algorithm and provides a visual overlay of the results.
For the full report and results, you can find it [here](https://github.com/NaorGuetta/3DPoseSolver/blob/master/2D-3D%20Pose%20Estimation%20Mini-Project.pdf).


## Outcome
Below are some photos showcasing the results of our 3D rendering and pose estimation system:
### Teapot Scene Before and After EPnP Overlay
<div style="display: flex; justify-content: space-around; align-items: center;">
    <div style="text-align: center;">
        <h4>Scene Snapshot:</h4>
        <img src="https://github.com/NaorGuetta/3DPoseSolver/blob/master/3DSolverImg/image3.png" alt="photo was taken 1" width="300"/>
        <img src="https://github.com/NaorGuetta/3DPoseSolver/blob/master/3DSolverImg/image1.png" alt="photo was taken 2" width="300"/>
    </div>
    <div style="text-align: center;">
        <h4>Solve position and overlay result:</h4>
        <img src="https://github.com/NaorGuetta/3DPoseSolver/blob/master/3DSolverImg/image4.png" alt="Solve position 1" width="300"/>
        <img src="https://github.com/NaorGuetta/3DPoseSolver/blob/master/3DSolverImg/image2.png" alt="Solve position 2" width="300"/>
    </div>
</div>

## Setup:
To run this project, you'll need to have OpenGL and a C++ development environment.The project is configured to work with the EPnP algorithm for pose estimation.

## Usage:
After compiling the project, use the WASD keys for movement and the arrow keys for camera rotation. Press 'B' to capture images and 'R' to process and estimate camera poses.
