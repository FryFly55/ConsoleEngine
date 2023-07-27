// manages the render pipeline and when to do which calculations

//
// Created by FryFly55 on 16.06.2023.
//

#include "renderer.h"
#include "commons.h"
#include "scene.h"
#include "Camera.h"
#include <cmath>

using namespace Util;

namespace Renderer {
    GLubyte* screen = new GLubyte[Window::screenWidth * Window::screenHeight * 3];

    const char* testMap = "   ##   "
                          "   ##   "
                          "   ##   "
                          "########"
                          "########"
                          "   ##   "
                          "   ##   "
                          "   ##   ";

    bool sColour = true;
}

int Renderer::rasterize() {
    // todo: fix bug where lines don't render at 45° angle
    // todo: fix bug where some lines are drawn to points outside the screen and others aren't
    for (int t = 0; t < 34; t += 3) {
        // stores transformed x, y and z components of each vertex in screenSpace, plus one element to verify validity
        float screenSpaceVertices[12];
        // stores transformed x, y and z components of each vertex in viewspace + validity verification
        float viewSpaceVertices[12];
        // stores the screenX, Y and transformed Z-Component of each vertex + verify
        int screenPoints[9];
        // stores the screen coordinates of each vertex on the screen-plane, even if it's not in screen space + verify
        int planePoints[9];
        // stores depth for every screenPoint + verify
        float depthBuffer[6];
        for (int v = 0; v < 3; v++) {
            bool zAllowed = true;
            // transforming into viewspace
            int point = Scene::triangles[t + v];
            float x = Scene::vertices[point*3] - Camera::position.x;
            float y = Scene::vertices[point*3 + 1] - Camera::position.y;
            float z = Scene::vertices[point*3 + 2] - Camera::position.z;

            float xPos = x * cos(Camera::radAngle) - z * sin(Camera::radAngle);
            float zPos = x * sin(Camera::radAngle) + z * cos(Camera::radAngle);
            x = xPos;
            z = zPos;

            viewSpaceVertices[v*4] = 1; // one => verified, anything but one => unverified
            viewSpaceVertices[v*4+1] = x;
            viewSpaceVertices[v*4+2] = y;
            viewSpaceVertices[v*4+3] = z;

            // clipping to screenSpace
            bool inDistance = z < Camera::farClippingPlane && z > Camera::nearClippingPlane;
            bool inBoundsX = (x < 1 / tan(Camera::rFOV / 2) * z) && (x > 1 / -tan(Camera::rFOV / 2) * z);
            bool inBoundsY = (y < 1 / tan(Camera::rFOV / 2) * z) && (y > 1 / -tan(Camera::rFOV / 2) * z);
            if (inDistance) {
                float ratioX = x * ((Window::screenWidth / 2) / (tan(Camera::rFOV / 2) * z));
                float ratioY = y * ((Window::screenWidth / 2) / (tan(Camera::rFOV / 2) * z));

                int screenX = (int)(Window::screenWidth / 2) + (int)(ratioX);
                int screenY = (int)(Window::screenHeight / 2) + (int)(ratioY);


                planePoints[v*3] = 1;
                planePoints[v*3+1] = screenX;
                planePoints[v*3+2] = screenY;

                if (inBoundsX && inBoundsY) {
                    screenSpaceVertices[v*4] = 1;
                    screenSpaceVertices[v*4+1] = x;
                    screenSpaceVertices[v*4+2] = y;
                    screenSpaceVertices[v*4+3] = z;

                    screenPoints[v*3] = 1;
                    screenPoints[v*3+1] = screenX;
                    screenPoints[v*3+2] = screenY;
                    depthBuffer[v*2] = 1;
                    depthBuffer[v*2+1] = z;

                    Colour colour = WHITE;
                    if (Input::keyPressed(GLFW_KEY_SPACE)) {
                        flip(sColour);
                    }
                    if (!sColour)
                        colour = FULL_RED;

                    safeDraw(screenX, screenY, colour);
                }
            }
        }

        for (int v = 0; v < 3; v++) {
            if (planePoints[v*3] == 1) {
                if (v == 2) {
                    safeDrawLine({planePoints[v*3+1], planePoints[v*3+2]},
                                 {planePoints[1], planePoints[2]},
                                 WHITE);
                }
                else {
                    safeDrawLine({planePoints[v*3+1], planePoints[v*3+2]},
                                 {planePoints[(v+1)*3+1], planePoints[(v+1)*3+2]},
                                 WHITE);
                }
            }
        }
    }

    return 0;
}

int Renderer::oRasterize() {
    // crosshair
    oDrawPixelMap(Window::windowWidth / 2, Window::windowHeight / 2, testMap,
                  8, 8, FULL_GREEN, true);

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
    if (y < Window::screenHeight && x < Window::screenWidth && y >= 0 && x >= 0) {
        Renderer::screen[y * Window::screenWidth * 3 + x * 3] = colour.r;
        Renderer::screen[y * Window::screenWidth * 3 + x * 3 + 1] = colour.g;
        Renderer::screen[y * Window::screenWidth * 3 + x * 3 + 2] = colour.b;
    }
    else {
        return 1; // coordinates outside of range
    }

    return 0;
}

int Renderer::oSafeDraw(int x, int y, struct Colour colour) {
    if (y < Window::windowHeight && x < Window::windowWidth && y >= 0 && x >= 0) {
        Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3] = colour.r;
        Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3 + 1] = colour.g;
        Window::m_screenBuffer[y * Window::windowWidth * 3 + x * 3 + 2] = colour.b;
    }
    else {
        return 1; // coordinates outside of range
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
    else if (bothZero != 0) {
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
    else if (bothZero != 0) {
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

int Renderer::safeDrawLine(vec2<int> start, vec2<int> end, struct Colour colour) {
    bool safeCheck = true;
    if (start.x < Window::screenWidth && start.x > 0 && start.y < Window::screenHeight && start.y > 0 &&
            end.x < Window::screenWidth && end.x > 0 && end.y < Window::screenHeight && end.y > 0) {
        safeCheck = false;
    }
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

    // Technically division by zero can occur here, but if deltaX is zero,
    // xSign isn't going to be used and vice versa. So it does not matter.
    int xSign = (int) (deltaX / abs(deltaX));
    int ySign = (int) (deltaY / abs(deltaY));

    // whenever it passes the threshold to a new pixel, round off and draw.
    float xOff = start.x;
    float yOff = start.y;

    float bothZero = deltaX + deltaY;

    bool curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
    bool curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
    if (abs(deltaX) >= abs(deltaY) && bothZero != 0) {
        if (safeCheck) {
            while (!curXinProx || !curYinProx) {
                start.x += xSign;
                yOff += yRat;
                start.y = (int) std::floor(yOff);
                if (safeDraw(start.x, start.y, colour)) {
                    return 1;
                }
                curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
                curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
            }
        }
        else {
            while (!curXinProx || !curYinProx) {
                start.x += xSign;
                yOff += yRat;
                start.y = (int) std::floor(yOff);
                fastDraw(start.x, start.y, colour);
                curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
                curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
            }
        }
    }
    else if (bothZero != 0) {
        if (safeCheck) {
            while (!curXinProx || !curYinProx) {
                start.y += ySign;
                xOff += xRat;
                start.x = (int) std::floor(xOff);
                if (safeDraw(start.x, start.y, colour)) {
                    return 1;
                }
                curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
                curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
            }
        }
        else {
            while (!curXinProx || !curYinProx) {
                start.y += ySign;
                xOff += xRat;
                start.x = (int) std::floor(xOff);
                fastDraw(start.x, start.y, colour);
                curXinProx = start.x <= end.x + 1 && start.x >= end.x - 1;
                curYinProx = start.y <= end.y + 1 && start.y >= end.y - 1;
            }
        }
    }

    return 0;
}

int Renderer::drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY,
                           Colour colour, bool center) {
    if (center) {
        y -= mapY / 2;
        x -= mapX / 2;
    }
    for (int i = 0; i < mapX * mapY; i++) {
        int screenY = y + (mapY - ((int) i / mapX));
        int screenX = x + (i % mapX);
        if (pixelMap[i] == '#') {
            safeDraw(screenX, screenY, colour);
        }
    }

    return 0;
}

int Renderer::oDrawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY,
                            Colour colour, bool center) {
    if (center) {
        y -= mapY / 2;
        x -= mapX / 2;
    }
    for (int i = 0; i < mapX * mapY; i++) {
        int screenY = y + (mapY - ((int) i / mapX));
        int screenX = x + (i % mapX);
        if (pixelMap[i] == '#') {
            oSafeDraw(screenX, screenY, colour);
        }
    }

    return 0;
}