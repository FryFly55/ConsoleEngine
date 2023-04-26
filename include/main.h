//
// Created by FryFly55 on 26.04.2023.
//

#ifndef CONSOLEENGINE_MAIN_H
#define CONSOLEENGINE_MAIN_H

#include "rasterizer.h"
#include <windows.h>

int Error(const wchar_t* msg);
static BOOL CloseHandler(DWORD evt);
void draw();
void getInput();
void safeDraw(int x, int y, Pixel::PIXEL_TYPE pixelType, Pixel::COLOUR colour);
void safeDraw(int x, int y, wchar_t chars, Pixel::COLOUR colour); // function overloading
void drawDigits(int i, int digit, int initialX, int initialY);
void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY);
void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY, Pixel::PIXEL_TYPE pixel, Pixel::COLOUR colour);

#endif //CONSOLEENGINE_MAIN_H
