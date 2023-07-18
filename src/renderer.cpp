// manages the render pipeline and when to do which calculations

//
// Created by FryFly55 on 16.06.2023.
//

#include "renderer.h"
#include "worldspace.h"
#include "scene.h"
#include "Camera.h"
#include "window.h"

namespace Renderer {
    GLubyte* screen = new GLubyte[Window::screenWidth * Window::screenHeight * 3];
}

int Renderer::rasterize() {
    // transforming worldspace coordinates into viewspace coordinates
    Worldspace::relate(Camera::position, Scene::vertices);
    Worldspace::rotate(Camera::lookVector, Scene::vertices);

    return 0;
}

int Renderer::scale() {
    // loop through abstracted screenBuffer to scale
    for (int x = 0; x < Window::screenWidth; x++) {
        for (int y = 0; y < Window::screenHeight; y++) {
            int startX = x * Window::pixelWidth;
            int startY = y * Window::pixelHeight;
            // now loop through pixel Height, width to scale the pixel
            for (int h = 0; h < Window::pixelHeight; h++) {
                for (int w = 0; w < Window::pixelWidth; w++) {
                    Window::m_screenBuffer[(startY + h) * Window::windowWidth * 3 + (startX + w) * 3] =
                            screen[y * Window::screenWidth * 3 + x * 3];
                    Window::m_screenBuffer[(startY + h) * Window::windowWidth * 3 + (startX + w) * 3 + 1] =
                            screen[y * Window::screenWidth * 3 + x * 3 + 1];
                    Window::m_screenBuffer[(startY + h) * Window::windowWidth * 3 + (startX + w) * 3 + 2] =
                            screen[y * Window::screenWidth * 3 + x * 3 + 2];
                }
            }
        }
    }

    return 0;
}

int Renderer::safeDraw(int x, int y, struct Colour colour) {
    if (y < Window::screenHeight && x < Window::screenWidth) {
        Renderer::screen[y * Window::screenWidth * 3 + x * 3] = colour.r;
        Renderer::screen[y * Window::screenWidth * 3 + x * 3 + 1] = colour.g;
        Renderer::screen[y * Window::screenWidth * 3 + x * 3 + 2] = colour.b;
    }

    return 0;
}

int Renderer::safeSetPixel(int x, int y, struct Colour colour) {
    if (y < Window::windowHeight && x < Window::windowWidth) {
        Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3] = colour.r;
        Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3 + 1] = colour.g;
        Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3 + 2] = colour.b;
    }

    return 0;
}

int Renderer::fastDraw(int x, int y, struct Colour colour) {
    Renderer::screen[y * Window::screenWidth * 3 + x * 3] = colour.r;
    Renderer::screen[y * Window::screenWidth * 3 + x * 3 + 1] = colour.g;
    Renderer::screen[y * Window::screenWidth * 3 + x * 3 + 2] = colour.b;

    return 0;
}

int Renderer::fastSetPixel(int x, int y, struct Colour colour) {
    Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3] = colour.r;
    Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3 + 1] = colour.g;
    Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3 + 2] = colour.b;

    return 0;
}

int Renderer::initBuffers() {
    return 0;
}