//
// Created by FryFly55 on 16.06.2023.
//

#ifndef CONSOLEENGINE_RENDERER_H
#define CONSOLEENGINE_RENDERER_H

#pragma once

#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace Renderer {


    int rasterize();
    // scales the abstracted screenBuffer to fit the openGL one
    int scale();
    int initBuffers();
    int safeDraw(int x, int y, struct Colour colour);
    int fastDraw(int x, int y, struct Colour colour);
    int safeSetPixel(int x, int y, struct Colour colour);
    int fastSetPixel(int x, int y, struct Colour colour);

    struct Colour {
        GLubyte r;
        GLubyte g;
        GLubyte b;
    };

    const struct Colour WHITE = {255, 255, 255};
    const struct Colour FULL_RED = {255, 0, 0};
    const struct Colour FULL_GREEN = {0, 255, 0};
    const struct Colour FULL_BLUE = {0, 0, 255};
    const struct Colour BLACK = {0, 0, 0};
    const struct Colour DARK_RED = {153, 0, 0};
    const struct Colour BRIGHT_RED = {255, 65, 17};
    const struct Colour DARK_GREEN = {0, 140, 40};
    const struct Colour GREEN = {51, 204, 51};
    const struct Colour DARK_BLUE = {0, 0, 153};
    const struct Colour BRIGHT_BLUE = {0, 204, 255};
    const struct Colour BRIGHT_YELLOW = {255, 255, 0};
    const struct Colour YELLOW = {237, 232, 18};
    const struct Colour PINK = {255, 0, 255};
    const struct Colour BROWN = {102, 51, 0};
    const struct Colour GRAY25 = {64, 64, 64};
    const struct Colour GRAY50 = {128, 128, 128};
    const struct Colour GRAY75 = {172, 172, 172};
    const struct Colour LIGHT_PINK = {255, 153, 255};

    struct Pixel {
        Colour colour;
        int x;
        int y;
    };

    extern GLubyte* screen;
}

#endif //CONSOLEENGINE_RENDERER_H
