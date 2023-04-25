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

enum pos {
    X = 0,
    Y = 1,
    Z = 2
};

int Error(const wchar_t* msg);
static BOOL CloseHandler(DWORD evt);
void draw();
void getInput();
void safeDraw(int x, int y, Pixel::PIXEL_TYPE pixelType, Pixel::COLOUR colour);
void safeDraw(int x, int y, wchar_t chars, Pixel::COLOUR colour); // function overloading
void drawDigits(int i, int digit, int initialX, int initialY);
void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY);
void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY, Pixel::PIXEL_TYPE pixel, Pixel::COLOUR colour);

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
