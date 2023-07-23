// manages the render pipeline and when to do which calculations

//
// Created by FryFly55 on 16.06.2023.
//

#include "renderer.h"
#include "scene.h"
#include "Camera.h"
#include "window.h"
#include <cmath>
#include "input.h"

namespace Renderer {
    GLubyte* screen = new GLubyte[Window::screenWidth * Window::screenHeight * 3];
}

int Renderer::rasterize() {
    for (int i = 0; i < 22; i+=3) {
        float ratioX = 1 / (tan(Camera::rFOV / 2) * Scene::vertices[i+2] * Scene::vertices[i]);
        float ratioY = 1 / (tan(Camera::rFOV / 2) * Scene::vertices[i+2] * Scene::vertices[i+1]);

        int screenX = (Window::screenWidth / 2) + (int)(ratioX * Window::screenWidth * 0.5f);
        int screenY = (Window::screenHeight / 2) + (int)(ratioY * Window::screenWidth * 0.5f);

        Colour colour = WHITE;
        if (Input::keyPressedOrHeld(GLFW_KEY_SPACE)) {
            colour = FULL_RED;
        }

        safeDraw(screenX, screenY, colour);
    }

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

int Renderer::oSafeDraw(int x, int y, struct Colour colour) {
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

int Renderer::oFastDraw(int x, int y, struct Colour colour) {
    Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3] = colour.r;
    Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3 + 1] = colour.g;
    Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3 + 2] = colour.b;

    return 0;
}

int Renderer::drawLine(vec2<int> start, vec2<int> end, struct Colour colour) {
    // calculating the distance of each pixel to the next one, just need to draw a line now.
    // Also getting the sign of each delta, and the y's per x as well as x's per y
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;

    float xRat = deltaX/abs(deltaY);
    if (deltaX < 1 && deltaX > -1)
        xRat = 0;
    float yRat = deltaY/abs(deltaX);
    if (deltaY < 1 && deltaY > -1)
        yRat = 0;

    /*int xSign = 0;
    int ySign = 0;
    if (deltaX != 0)
        xSign = (int) (deltaX / abs(deltaX)); // calculate it now to save performance, is either 1, -1
    if (deltaY != 0)
        ySign = (int) (deltaY / abs(deltaY));*/

    int xSign = (int) (deltaX / abs(deltaX));
    int ySign = (int) (deltaY / abs(deltaY));

    // whenever it passes the threshold to a new pixel, round off and draw.
    float xOff = start.x;
    float yOff = start.y;

    float bothZero = deltaX + deltaY;

    bool curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
    bool curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
    if (abs(deltaX) >= abs(deltaY) && bothZero != 0) {
        while (!curXinProx || !curYinProx) {
            start.x += xSign;
            yOff += yRat;
            start.y = (int) std::floor(yOff);
            fastDraw(start.x, start.y, colour);
            curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
            curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
        }
    }
    else if (abs(deltaY) > abs(deltaX) && bothZero != 0) {
        while (!curXinProx || !curYinProx) {
            start.y += ySign;
            xOff += xRat;
            start.x = (int) std::floor(xOff);
            fastDraw(start.x, start.y, colour);
            curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
            curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
        }
    }

    return 0;
}

int Renderer::oDrawLine(vec2<int> start, vec2<int> end, struct Colour colour) {
    // calculating the distance of each pixel to the next one, just need to draw a line now.
    // Also getting the sign of each delta, and the y's per x as well as x's per y
    float deltaX = end.x - start.x;
    float deltaY = end.y - start.y;

    float xRat = deltaX/abs(deltaY);
    if (deltaX < 1 && deltaX > -1)
        xRat = 0;
    float yRat = deltaY/abs(deltaX);
    if (deltaY < 1 && deltaY > -1)
        yRat = 0;

    /*int xSign = 0;
    int ySign = 0;
    if (deltaX != 0)
        xSign = (int) deltaX / abs(deltaX); // calculate it now to save performance, is either 1, -1
    if (deltaY != 0)
        ySign = (int) deltaY / abs(deltaY);*/
    int xSign = (int) (deltaX / abs(deltaX));
    int ySign = (int) (deltaY / abs(deltaY));

    // whenever it passes the threshold to a new pixel, round off and draw.
    float xOff = start.x;
    float yOff = start.y;

    float bothZero = deltaX + deltaY;

    bool curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
    bool curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
    if (abs(deltaX) >= abs(deltaY) && bothZero != 0) {
        while (!curXinProx || !curYinProx) {
            start.x += xSign;
            yOff += yRat;
            start.y = (int) std::floor(yOff);
            oFastDraw(start.x, start.y, colour);
            curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
            curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
        }
    }
    else if (abs(deltaY) > abs(deltaX) && bothZero != 0) {
        while (!curXinProx || !curYinProx) {
            start.y += ySign;
            xOff += xRat;
            start.x = (int) std::floor(xOff);
            oFastDraw(start.x, start.y, colour);
            curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
            curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
        }
    }

    return 0;
}