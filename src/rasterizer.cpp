//
// Created by FryFly55 on 22.04.2023.
//

// takes vertices and triangles array and draws the geometry onto the screen

#include <windows.h>
#include <cmath>

#include "rasterizer.h"
#include "initialiser.h"
#include "main.h"

namespace Renderer {
    void rasterize() {
        // only needs to be calculated once technically, but in case I ever want it to be changed dynamically during
        // runtime, it's calculated every frame.
        double hFov = (GameVar::FOV / 2) * (Window::PI/180);

        // todo: arrays are filled with garbage values, if something fails and those are accessed, the program may crash!
        // iterates through the first of three vertex-indices of each triangle
        for (int t = 0; t < Renderer::cTriangles; t += 3) {
            // stores the transformed x,y and z components of each vertex
            float screenSpaceVertices[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
            // stores the screenX,Y as well as transformed Z component of each vertex / point
            float screenPoints[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

            for (int i = 0; i < 3; i++) {
                // one vertex relative to the player
                int point = Renderer::triangles[t+i];
                float x = Renderer::vertices[point*3] - GameVar::playerX;
                float y = Renderer::vertices[point*3+1] - GameVar::playerY;
                float z = Renderer::vertices[point*3+2] - GameVar::playerZ;

                // transform (rotate) the vertices directly in here, before filling in screenSpaceVertices[]
                float xPos = x * cos(GameVar::radPlayerAngleX) - z  * sin(GameVar::radPlayerAngleX);
                float zPos = x * sin(GameVar::radPlayerAngleX) + z * cos(GameVar::radPlayerAngleX);
                x = xPos;
                z = zPos;

                // don't want to get a 'divide by zero' error (although it isn't even an official exception in c++)
                if (z == 0) {
                    z = 0.01f;
                }

                // calculate the screenX and screenY of each point, check if they are within bounds beforehand.
                bool inDistance = z < GameVar::farClippingPlane;
                bool inBoundsX = x < tan(hFov) * z && x > -tan(hFov) * z;
                bool inBoundsY = y < tan(hFov) * z && y > -tan(hFov) * z;

                if (!inDistance) { // this is just a bunch of debug code todo: remove later.
                    /*screenBuffer[1].Char.UnicodeChar = PIXEL_SOLID;
                    screenBuffer[1].Attributes = FG_RED;*/
                    safeDraw(1, 0, Pixel::PIXEL_SOLID, Pixel::FG_RED);
                }
                if (!inBoundsX) {
                    /*screenBuffer[2].Char.UnicodeChar = PIXEL_SOLID;
                    screenBuffer[2].Attributes = FG_RED;*/
                    safeDraw(2, 0, Pixel::PIXEL_SOLID, Pixel::FG_RED);
                }
                if (!inBoundsY) {
                    /*screenBuffer[3].Char.UnicodeChar = PIXEL_SOLID;
                    screenBuffer[3].Attributes = FG_RED;*/
                    safeDraw(3, 0, Pixel::PIXEL_SOLID, Pixel::FG_RED);
                }

                if (inDistance && inBoundsX) {
                    // now populate screenSpaceVertices[]
                    screenSpaceVertices[i*3] = x;
                    screenSpaceVertices[i*3+1] = y;
                    screenSpaceVertices[i*3+2] = z;

                    double ratioX = x * (1 / (tan(hFov) * z)); // how much of half a screen wide/high is occupied
                    double ratioY = y * (1 / (tan(hFov) * z));

                    // Now calculate it into screenSpace, populate screenPoints[] array
                    screenPoints[i*3] = (Window::screenWidth / 2) + (int)(ratioX * Window::screenWidth * 0.5f);
                    screenPoints[i*3+1] = (Window::screenHeight / 2) + (int)(ratioY * Window::screenWidth * 0.5f);

                    screenPoints[i*3+2] = z; // maybe I will normalise/scale z somehow later, so it's good to have.
                }
            }

            // Now that all corners of the triangle are calculated, we can start drawing it.
            for (int i = 0; i < 3 && GameVar::doLineRender; i++) {
                int curX = (int)screenPoints[i*3]; // should always be int values, the array is only float because of z
                int curY = (int)screenPoints[i*3+1];
                int targetX = screenPoints[0];
                int targetY = screenPoints[1];

                if (i < 2) {
                    targetX = (int)screenPoints[(i+1)*3];
                    targetY = (int)screenPoints[((i+1)*3)+1];
                }

                if (curX < 0 || curY < 0 || targetX < 0 || targetY < 0)
                    continue;

                // calculating the distance of each pixel to the next one, just need to draw a line now.
                // Also getting the sign of each delta, and the y's per x as well as x's per y
                float deltaX = targetX - curX;
                float deltaY = targetY - curY;

                float xRat = deltaX/abs(deltaY);
                if (deltaX < 1 && deltaX > -1)
                    xRat = 0;
                float yRat = deltaY/abs(deltaX);
                if (deltaY < 1 && deltaY > -1)
                    yRat = 0;

                int xSign = 0;
                int ySign = 0;
                if (deltaX != 0)
                    xSign = (int) deltaX / abs(deltaX); // calculate it now to save performance, is either 1, -1
                if (deltaY != 0)
                    ySign = (int) deltaY / abs(deltaY);

                // whenever it passes the threshold to a new pixel, round off and draw.
                float xOff = curX;
                float yOff = curY;

                // rendering in different colors to help differentiate faces. (edges are used by more than one triangle,
                // so it isn't always 100% perfect, but still helps to find bugs)
                Pixel::COLOUR renderColour = Pixel::FG_RED;
                float triangle = t/3;
                if (triangle == 0 || triangle == 2)
                    renderColour = Pixel::FG_WHITE;
                else if (triangle == 1 || triangle == 3)
                    renderColour = Pixel::FG_BLUE;
                else if (triangle > 3)
                    renderColour = Pixel::FG_YELLOW;

                // set the std colour
                if (!GameVar::doColourDebug)
                    renderColour = Pixel::FG_BLUE;

                float bothZero = deltaX + deltaY;

                bool curXinProx = curX <= targetX + 1 && curX >= targetX - 1;
                bool curYinProx = curY <= targetY + 1 && curY >= targetY - 1;
                if (abs(deltaX) >= abs(deltaY) && bothZero != 0) {
                    while (!curXinProx || !curYinProx) {
                        curX += xSign;
                        yOff += yRat;
                        if (yOff >= curY + ySign)
                            curY = (int) std::floor(yOff);
                        safeDraw(curX, curY, Pixel::PIXEL_SOLID, renderColour);
                        curXinProx = curX <= targetX + 1 && curX >= targetX - 1;
                        curYinProx = curY <= targetY + 1 && curY >= targetY - 1;
                    }
                }
                else if (abs(deltaY) > abs(deltaX) && bothZero != 0) {
                    while (!curXinProx || !curYinProx) {
                        curY += ySign;
                        xOff += xRat;
                        if (xOff >= curX + xSign)
                            curX = (int) std::floor(xOff);
                        safeDraw(curX, curY, Pixel::PIXEL_SOLID, renderColour);
                        curXinProx = curX <= targetX + 1 && curX >= targetX - 1;
                        curYinProx = curY <= targetY + 1 && curY >= targetY - 1;
                    }
                }
            }

            for (int i = 0; i < 3; i++) {
                float x = screenSpaceVertices[i*3];
                float y = screenSpaceVertices[i*3+1];
                float z = screenSpaceVertices[i*3+2];

                // check if the point is within bounds before drawing, instead of calculating
                bool inBoundsX = x < tan(hFov) * z && x > -tan(hFov) * z;
                bool inBoundsY = y < tan(hFov) * z && y > -tan(hFov) * z;

                if (inBoundsX && inBoundsY)
                    safeDraw((int)screenPoints[i*3], (int)screenPoints[i*3+1], Pixel::PIXEL_SOLID, Pixel::FG_GREEN);
            }
        }
    }
}
