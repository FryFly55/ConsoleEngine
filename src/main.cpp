/*
 *  made by FryFly, 2023-03-29
 */

#include <windows.h>

#include <chrono>
#include <list>
#include <thread>
#include <atomic>
#include <cstring>
#include <string>
#include <cmath>

#include "rasterizer.h"
#include "initialiser.h"
#include "main.h"

enum pos {
    X = 0,
    Y = 1,
    Z = 2
};

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
HANDLE hOriginalConsole;

SMALL_RECT rectWindow;

CHAR_INFO *screenBuffer;

const wchar_t* windowTitle = L"Engine";

// Set everything you need inside the game-loop or draw-cycle here.

int main() {
    init();

    // Set Console output to unicode
    SetConsoleCP(UNICODE);

    // check if handle is valid
    if (hConsole == INVALID_HANDLE_VALUE) {
        Error(L"Invalid Handle!");
        return 1;
    }

    // set visible console size to a minimum to ease working with the screen buffer.
    rectWindow = {0, 0, 1, 1};
    SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

    // set size of the screen buffer afterwards
    COORD coord = {(short) Window::screenWidth, (short) Window::screenHeight};
    if (!SetConsoleScreenBufferSize(hConsole, coord)) {
        Error(L"SetConsoleActiveScreenBuffer: setting screen buffer size failed");
        return 2;
    }
    // Now assign screen buffer to the console
    if (!SetConsoleActiveScreenBuffer(hConsole)) {
        Error(L"SetConsoleActiveScreenBuffer: assigning screen buffer failed");
        return 3;
    }

    // Now set font size
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = Window::fontWidth;
    cfi.dwFontSize.Y = Window::fontHeight;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Consolas");
    if (!SetCurrentConsoleFontEx(hConsole, false, &cfi)) {
        Error(L"SetCurrentConsoleFontEx");
        return 4;
    }


    // get buffer info and check that window size is within allowed bounds
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(hConsole, &bufferInfo)) {
        Error(L"GetConsoleScreenBufferInfo: failed");
        return 5;
    }
    if (Window::screenHeight > bufferInfo.dwMaximumWindowSize.Y) {
        Error(L"[int fontHeight] to big");
        return 6;
    }
    if (Window::screenWidth > bufferInfo.dwMaximumWindowSize.X) {
        Error(L"[int fontWidth] to big");
        return 7;
    }

    // Now set console window size to be bigger again, after changing the buffer.
    // Size now depends on fontSize and resolution/size of the buffer
    rectWindow = {0, 0, static_cast<SHORT>((short) Window::screenWidth - 1), static_cast<SHORT>((short) Window::screenHeight - 1)};
    if (!SetConsoleWindowInfo(hConsole, TRUE, &rectWindow)) {
        Error(L"SetConsoleWindowInfo, failed after changing window size according to buffer");
        return 8;
    }

    // allow mouse inputs
    if (!SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) {
        Error(L"SetConsoleMode: allowing mouse input failed");
        return 9;
    }

    // Allocate memory for screen buffer
    screenBuffer = new CHAR_INFO[Window::screenWidth * Window::screenHeight];
    std::memset(screenBuffer, 0, sizeof(CHAR_INFO) * Window::screenWidth * Window::screenHeight);

    SetConsoleCtrlHandler(nullptr, FALSE);

    // Game initialisation logic
    GameVar::playerSpeed = GameVar::playerBaseSpeed;

    // calculate first dt timestep (or is it a delta-timestep? delta-time-step? delta time-step? To not use time twice?)
    GameVar::dt = 0.0f;
    auto tpLast = std::chrono::system_clock::now();

    // Game loop basically
    while(true) {
        // clear screenBuffer
        std::memset(screenBuffer, 0, sizeof(CHAR_INFO) * Window::screenWidth * Window::screenHeight);

        getInput();

        // after getting input, update everything needed inside draw()
        GameVar::radPlayerAngleX = GameVar::playerAngleX * (Window::PI/180);
        GameVar::radPlayerAngleY = GameVar::playerAngleY * (Window::PI/180);

        // we need the fps value in draw(), so initialise it earlier.
        GameVar::fps = 0.1f / GameVar::dt; // I tried 1 / dt, but that gave results between 200 and 300, and it feels
        // more like 20 to 30 fps to me, especially when it gets lower you notice it.
        // remove the line below if you want fps shown with decimal places (kinda useless in most situations)
        if (!GameVar::showFpsDecimals)
            GameVar::fps = std::floor(GameVar::fps + 0.5f); // rounding by flooring the value + 0.5

        GameVar::lineToggleCounter += GameVar::dt;
        GameVar::colourDebugCounter += GameVar::dt;
        GameVar::fpsDecimalDebugCounter += GameVar::dt;

        draw();

        // Refresh screen buffer
        SetConsoleActiveScreenBuffer(hConsole);

        PSMALL_RECT pRect = &rectWindow;
        const _CHAR_INFO * copyBuffer = screenBuffer;
        WriteConsoleOutputW(hConsole, copyBuffer, coord,{0, 0}, pRect);

        // at last calculate dt
        auto tpCurrent = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tpCurrent - tpLast;
        tpLast = tpCurrent;
        GameVar::dt = elapsedTime.count();
    }

    return 0;
}

// In here comes all the drawing code, that isn't clearing / refreshing the screenBuffer.
// also ignore the out-commented stuff, it was (is) for testing different ideas.
void draw() {

    // draw the ten debug lamps on top green by default. Also add the scale thingy below
    for (int j = 1; j < 11; j++) {
        screenBuffer[j].Char.UnicodeChar = Pixel::PIXEL_SOLID;
        screenBuffer[j].Attributes = Pixel::FG_GREEN;
        if (j % 2 == 0) {
            /*screenBuffer[screenWidth + j].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[screenWidth + j].Attributes = FG_WHITE;*/
            safeDraw(j, 1, Pixel::PIXEL_SOLID, Pixel::FG_WHITE);
        }
        else {
            /*screenBuffer[screenWidth + j].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[screenWidth + j].Attributes = FG_DARK_BLUE;*/
            safeDraw(j, 1, Pixel::PIXEL_SOLID, Pixel::FG_BLACK);
        }
    }

    Renderer::rasterize();

    // render the fps-counter
    std::string sFps = std::to_string(GameVar::fps);
    int digits = sFps.length(); // u_long would be better, but int is fine for our purposes.
    // I will assume that the fps will never exceed 6 digits (unlikely anyway considering my (non-) optimisation)
    for (int i = 0; i < digits && i < 6; i++) {
        int digit = sFps[i] - 48; // converts a char to a number value, by shifting it 48 ascii positions
        if (digit <= 9 && digit >= 0) {
            drawDigits(i, digit, Window::screenWidth - (6 * 4) - GameVar::fpsOffset, 0);
        }
        if (sFps[i] == '.') {
            drawDigits(i, -1, Window::screenWidth - (6 * 4) - GameVar::fpsOffset, 0);
        }
    }

    // draw pixels to the corners to mark the bounds of screenBuffer array
    // I do this last to render it on top of everything else
    safeDraw(0, 0, Pixel::PIXEL_SOLID, Pixel::FG_RED);
    safeDraw(Window::screenWidth - 1, Window::screenHeight - 1, Pixel::PIXEL_SOLID, Pixel::FG_RED);
    //safeDraw(screenWidth / 2, screenHeight / 2, PIXEL_SOLID, FG_RED); // this draws a primitive crosshair

    // this draws a more advanced crosshair
    drawPixelMap((Window::screenWidth / 2) - std::floor(UI::crosshairWidth / 2),
                 (Window::screenHeight / 2) - std::floor(UI::crosshairHeight / 2), UI::crosshair, UI::crosshairWidth,
                 UI::crosshairHeight, UI::crosshairPixelType, UI::crosshairColour);
}

// crappy way to get input ik. No idea how to register mouse inputs yet as well.
void getInput() {
    // basic movement, always relative to the players rotation
    if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
        GameVar::playerX += sinf(GameVar::radPlayerAngleX) * GameVar::playerSpeed * GameVar::dt;
        GameVar::playerZ += cosf(GameVar::radPlayerAngleX) * GameVar::playerSpeed * GameVar::dt;
    }
    if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
        GameVar::playerX -= sinf(GameVar::radPlayerAngleX) * GameVar::playerSpeed * GameVar::dt;
        GameVar::playerZ -= cosf(GameVar::radPlayerAngleX) * GameVar::playerSpeed * GameVar::dt;
    }
    if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
        GameVar::playerX -= cosf(GameVar::radPlayerAngleX) * GameVar::playerSpeed * GameVar::dt;
        GameVar::playerZ += sinf(GameVar::radPlayerAngleX) * GameVar::playerSpeed * GameVar::dt;
    }
    if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
        GameVar::playerX += cosf(GameVar::radPlayerAngleX) * GameVar::playerSpeed * GameVar::dt;
        GameVar::playerZ -= sinf(GameVar::radPlayerAngleX) * GameVar::playerSpeed * GameVar::dt;
    }

    //move up and down
    if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
        GameVar::playerY -= GameVar::playerSpeed * GameVar::dt;
    if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
        GameVar::playerY += GameVar::playerSpeed * GameVar::dt;

    // change movement speed.
    if (GetAsyncKeyState((unsigned short)'F') & 0x8000)
        GameVar::playerSpeed -= 5.0f * GameVar::dt;
    if (GetAsyncKeyState((unsigned short)'G') & 0x8000)
        GameVar::playerSpeed = GameVar::playerBaseSpeed;
    if (GetAsyncKeyState((unsigned short)'H') & 0x8000)
        GameVar::playerSpeed += 5.0f * GameVar::dt;

    // Rotation horizontal
    if (GetAsyncKeyState((unsigned short)'J') & 0x8000)
        GameVar::playerAngleX -= GameVar::turnSpeed * GameVar::dt;
    if (GetAsyncKeyState((unsigned short) 'L') & 0x8000)
        GameVar::playerAngleX += GameVar::turnSpeed * GameVar::dt;

    // and vertical
    if (GetAsyncKeyState((unsigned short)'I') & 0x8000)
        GameVar::playerAngleY += GameVar::turnSpeed * GameVar::dt;
    if (GetAsyncKeyState((unsigned short)'K') & 0x8000)
        GameVar::playerAngleY -= GameVar::turnSpeed * GameVar::dt;

    if ((GetAsyncKeyState((unsigned short)'1') & 0x8000) && GameVar::lineToggleCounter > GameVar::lineToggleCD) {
        GameVar::doLineRender = !GameVar::doLineRender;
        GameVar::lineToggleCounter = 0.0f;
    }
    if ((GetAsyncKeyState((unsigned short)'2') & 0x8000) && GameVar::colourDebugCounter > GameVar::colourDebugToggleCD) {
        GameVar::doColourDebug = !GameVar::doColourDebug;
        GameVar::colourDebugCounter = 0.0f;
    }
    if ((GetAsyncKeyState((unsigned short)'3') & 0x8000) && GameVar::fpsDecimalDebugCounter > GameVar::fpsDecimalDebugToggleCD) {
        GameVar::showFpsDecimals = !GameVar::showFpsDecimals;
        GameVar::fpsDecimalDebugCounter = 0.0f;
    }
}

// quick tool to output error messages
// No idea if it works though. Sometimes when the program crashes I wonder if it was because an Error was detected,
// and logging crashed it. Guess I'll never know.
int Error(const wchar_t* msg)
{
    wchar_t buf[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  buf, 256, NULL);
    SetConsoleActiveScreenBuffer(hOriginalConsole);
    wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
    return 0;
}

// changes what ctrl + c does in the console, no idea about the underlying stuff, copied it from StackOverflow.
static BOOL CloseHandler(DWORD evt)
{
    return false;
}

void safeDraw(int x, int y, Pixel::PIXEL_TYPE pixelType, Pixel::COLOUR colour) {
    if (y * Window::screenWidth + x < Window::screenWidth * Window::screenHeight && y >= 0 && x >= 0) {
        screenBuffer[y * Window::screenWidth + x].Char.UnicodeChar = pixelType;
        screenBuffer[y * Window::screenWidth + x].Attributes = colour;
    }
}

void safeDraw(int x, int y, wchar_t chars, Pixel::COLOUR colour) {
    if (y * Window::screenWidth + x < Window::screenWidth * Window::screenHeight) {
        screenBuffer[y * Window::screenWidth + x].Char.UnicodeChar = chars;
        screenBuffer[y * Window::screenWidth + x].Char.UnicodeChar = colour;
    }
}

namespace digits {
// defining the pixel maps for each digit, maybe letters later.
const char* zero = {
        ".##."
        "#..#"
        "#..#"
        "#..#"
        ".##."
};

const char* one = {
        ".##."
        "..#."
        "..#."
        "..#."
        "..#."
};

const char* two = {
        ".##."
        "#..#"
        "..#."
        ".#.."
        "####"
};

const char* three = {
        "####"
        "...#"
        ".###"
        "...#"
        "####"
};

const char* four = {
        "#..#"
        "#..#"
        "####"
        "...#"
        "...#"
};

const char* five = {
        "####"
        "#..."
        "####"
        "...#"
        "####"
};

const char* six = {
        "####"
        "#..."
        "####"
        "#..#"
        "####"
};

const char* seven = {
        "####"
        "...#"
        "..#."
        ".#.."
        "#..."
};

const char* eight = {
        "####"
        "#..#"
        ".##."
        "#..#"
        "####"
};

const char* nine = {
        "####"
        "#..#"
        "####"
        "...#"
        ".###"
};

const char* dot = {
        "...."
        "...."
        "...."
        "##.."
        "##.."
};
}


// drawing a pixelMap to the screen, x, y mark the top left corner.
void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY) {
    for (int i = 0; i < mapX * mapY; i++) {
        int screenY = y + ((int) i / mapX); // division
        int screenX = x + (i % mapX); // and remainder, hope it gets optimised by the compiler to not divide twice
        if (pixelMap[i] == '#') {
            safeDraw(screenX, screenY, Pixel::PIXEL_SOLID, Pixel::FG_WHITE);
        }
        else {
            safeDraw(screenX, screenY, Pixel::PIXEL_SOLID, Pixel::FG_BLACK);
        }
    }
}

void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY, Pixel::PIXEL_TYPE pixel, Pixel::COLOUR colour) {
    for (int i = 0; i < mapX * mapY; i++) {
        int screenY = y + ((int) i / mapX); // division
        int screenX = x + (i % mapX); // and remainder, hope it gets optimised by the compiler to not divide twice
        if (pixelMap[i] == '#') {
            safeDraw(screenX, screenY, pixel, colour);
        }
        else {
            safeDraw(screenX, screenY, Pixel::PIXEL_SOLID, Pixel::FG_BLACK);
        }
    }
}

// x, y mark the top left corner. When digit is -1 it's going to draw a decimal point
void drawDigits(int i, int digit, int initialX, int initialY) {
    // todo: make an array implementation, for now a switch statement will do though
    switch(digit) {
        case -1:
            drawPixelMap(initialX + i*5, initialY, digits::dot, 4, 5);
            break;
        case 0:
            drawPixelMap(initialX + i*5, initialY, digits::zero, 4, 5);
            break;
        case 1:
            drawPixelMap(initialX + i*5, initialY, digits::one, 4, 5);
            break;
        case 2:
            drawPixelMap(initialX + i*5, initialY, digits::two, 4, 5);
            break;
        case 3:
            drawPixelMap(initialX + i*5, initialY, digits::three, 4, 5);
            break;
        case 4:
            drawPixelMap(initialX + i*5, initialY, digits::four, 4, 5);
            break;
        case 5:
            drawPixelMap(initialX + i*5, initialY, digits::five, 4, 5);
            break;
        case 6:
            drawPixelMap(initialX + i*5, initialY, digits::six, 4, 5);
            break;
        case 7:
            drawPixelMap(initialX + i*5, initialY, digits::seven, 4, 5);
            break;
        case 8:
            drawPixelMap(initialX + i*5, initialY, digits::eight, 4, 5);
            break;
        case 9:
            drawPixelMap(initialX + i*5, initialY, digits::nine, 4, 5);
            break;
        default:
            drawPixelMap(initialX + i*5, initialY, digits::zero, 4, 5);
    }
}
