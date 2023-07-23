//
// Created by FryFly55 on 14.06.2023.
//

#include "window.h"
#include <iostream>
#include <windows.h>
#include <chrono>
#include <list>
#include <thread>
#include <atomic>
#include <cstring>
#include <string>
#include <cmath>
#include <chrono>

#include "renderer.h"
#include "Camera.h"
#include "input.h"

namespace Window {
    const int screenWidth = 200;
    const int screenHeight = 125;
    const int pixelWidth = 8;
    const int pixelHeight = 8;
    const int windowWidth = screenWidth * pixelWidth;
    const int windowHeight = screenHeight * pixelHeight;

    double dt = 0;

    GLFWwindow* m_window;

    GLubyte* m_screenBuffer = new GLubyte[windowWidth * windowHeight * 3];

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    int createWindow() {
        glfwInit();

        GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Rasterizer", NULL, NULL);
        m_window = window;
        if (m_window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        glViewport(0, 0, windowWidth, windowHeight);
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        glfwSetKeyCallback(m_window, Input::keyCallBack);

        Camera::init({0, 0, 0}, {0, 0, 1}, 90, 0.1, 100);

        return 0;
    }

    int updateWindow() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        std::memset(m_screenBuffer, 0, sizeof(GLubyte) *
        windowWidth * windowHeight * 3);
        std::memset(Renderer::screen, 0, sizeof(GLubyte) *
        screenWidth * screenHeight * 3);

        // change abstracted screenBuffer here
        // keep in mind that all changes to the original screenBuffer must happen
        // after Renderer::scale()
        Renderer::rasterize();
        Renderer::scale(); // scales the renderer screenBuffer to fit the window one
        // change original screenBuffer here

        glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, m_screenBuffer);

        Input::updateKeyState();

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        return 0;
    }

    int run() {
        while (!glfwWindowShouldClose(m_window)) {
            auto tpLast = std::chrono::system_clock::now();
            updateWindow();
            auto tpCurrent = std::chrono::system_clock::now();
            std::chrono::duration<float> elapsedTime = tpCurrent - tpLast;
            tpLast = tpCurrent;
            dt = elapsedTime.count();
        }

        glfwTerminate();
        return 0;
    }

    int Error(std::string msg, int file, int line) {
        std::cout << "Error! <" << file << ", " << line << ">\n" << msg << std::endl;

        return 0;
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}