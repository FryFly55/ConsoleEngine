//
// Created by FryFly55 on 22.04.2023.
//

#ifndef CONSOLEENGINE_INITIALISER_H
#define CONSOLEENGINE_INITIALISER_H

#include <string>

namespace Window {
// 480, 270, 4, 4 will create a 1920x1080 Window, with 480x270 console pixels, each of which being 4 physical pixels. 512x320 also works
    inline int screenWidth = 480; // 512
    inline int screenHeight = 270; // 320
    inline int fontWidth = 4;
    inline int fontHeight = 4;
    inline const double PI = 3.14159265369;
}

namespace GameVar {
// FOV in degrees
    inline double FOV = 70;
// FOV in radians
    inline const double radFOV = FOV * (Window::PI/180);
    inline double farClippingPlane = 100.0f;

    inline float playerX = 0.0f;
    inline float playerY = 0.0f;
    inline float playerZ = 0.0f;
    inline double playerAngleX = 0.0f;
    inline double radPlayerAngleX = 0.0f;
    inline double playerAngleY = 0.0f;
    inline double radPlayerAngleY = 0.0f;
    inline float playerBaseSpeed = 3.0f;
    inline float playerSpeed = 3.0f;
    inline float turnSpeed = 40.0f;

// game variables and toggles
// starting with the line render option
    inline bool doLineRender = true;
    inline float lineToggleCD = 0.5f;
    inline float lineToggleCounter = 0.0f;

// coloured edges and faces
    inline bool doColourDebug = false;
    inline float colourDebugToggleCD = 0.5f;
    inline float colourDebugCounter = 0.0f;

    inline float dt;
    inline float fps;
    inline int fpsOffset = 10; // fps-display distance from the right screen border
}

namespace config {
    inline std::string vertexPath = "./config/vertices.txt";
    inline std::string trianglePath = "./config/triangles.txt";
    inline std::string configPath = "./config/game_config.txt";
}

void init();

#endif
