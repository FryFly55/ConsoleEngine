//
// Created by FryFly55 on 14.06.2023.
//

#ifndef CONSOLEENGINE_WINDOW_H
#define CONSOLEENGINE_WINDOW_H

#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"

// use namespace instead of class, since the class would be static either way, and the best replacement for what would
// be static classes in C# are namespaces in c++. Everything declared within the header can be accessed globally,
// everything declared in the cpp can only be accessed within the namespace. I also only need one window, as console
// windows are tied to the application, and Windows only gives one console to me anyway.
namespace Window {
    int createWindow();
    int updateWindow();
    int run();
    int Error(const wchar_t* msg);

    extern GLFWwindow* m_window;

    extern const int screenWidth;
    extern const int screenHeight;
    extern const int pixelWidth;
    extern const int pixelHeight;
    extern const int windowWidth;
    extern const int windowHeight;

    extern double dt;

    extern GLubyte* m_screenBuffer;
}

#endif //CONSOLEENGINE_WINDOW_H
