//
// Created by FryFly55 on 22.04.2023.
//

#ifndef CONSOLEENGINE_RASTERIZER_H
#define CONSOLEENGINE_RASTERIZER_H

#include "initialiser.h"

namespace Pixel {
    enum COLOUR
    {
        FG_BLACK		= 0x0000,
        FG_DARK_BLUE    = 0x0001,
        FG_DARK_GREEN   = 0x0002,
        FG_DARK_CYAN    = 0x0003,
        FG_DARK_RED     = 0x0004,
        FG_DARK_MAGENTA = 0x0005,
        FG_DARK_YELLOW  = 0x0006,
        FG_GREY			= 0x0007, // Thanks MS :-/
        FG_DARK_GREY    = 0x0008,
        FG_BLUE			= 0x0009,
        FG_GREEN		= 0x000A,
        FG_CYAN			= 0x000B,
        FG_RED			= 0x000C,
        FG_MAGENTA		= 0x000D,
        FG_YELLOW		= 0x000E,
        FG_WHITE		= 0x000F,
        BG_BLACK		= 0x0000,
        BG_DARK_BLUE	= 0x0010,
        BG_DARK_GREEN	= 0x0020,
        BG_DARK_CYAN	= 0x0030,
        BG_DARK_RED		= 0x0040,
        BG_DARK_MAGENTA = 0x0050,
        BG_DARK_YELLOW	= 0x0060,
        BG_GREY			= 0x0070,
        BG_DARK_GREY	= 0x0080,
        BG_BLUE			= 0x0090,
        BG_GREEN		= 0x00A0,
        BG_CYAN			= 0x00B0,
        BG_RED			= 0x00C0,
        BG_MAGENTA		= 0x00D0,
        BG_YELLOW		= 0x00E0,
        BG_WHITE		= 0x00F0,ds
    };
    inline COLOUR colours[] = {FG_DARK_BLUE, FG_DARK_GREEN, FG_DARK_RED, FG_DARK_MAGENTA, FG_GREEN, FG_MAGENTA, FG_YELLOW, FG_WHITE};

    enum PIXEL_TYPE
    {
        PIXEL_SOLID = 0x2588,
        PIXEL_THREEQUARTERS = 0x2593,
        PIXEL_HALF = 0x2592,
        PIXEL_QUARTER = 0x2591,
    };
}

namespace Renderer {
// stores all vertices within your scene
    inline float vertices[125*8*3]; // 125 cubes * 8 points per cube * 3 floats per point
    inline int cVertices = 0;

// stores which vertices are connected (as triangles)
    inline int triangles[125*12*3]; // 125 cubes * 12 triangles per cube * 3 points per triangle
    inline int cTriangles = 0;

    void rasterize();
}

namespace UI {
    inline const char* crosshair = {
            "..#.."
            "..#.."
            "#####"
            "..#.."
            "..#.."
    };
    inline int crosshairWidth = 5;
    inline int crosshairHeight = 5;
    inline Pixel::PIXEL_TYPE crosshairPixelType = Pixel::PIXEL_SOLID;
    inline Pixel::COLOUR crosshairColour = Pixel::FG_RED;
}

#endif