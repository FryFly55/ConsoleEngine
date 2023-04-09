/*
 *  made by FryFly, 2023-03-29
 */

#include <windows.h>

#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <cstring>
#include <cmath>

int Error(const wchar_t* msg);
static BOOL CloseHandler(DWORD evt);
void draw();
void getInput();

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
    BG_WHITE		= 0x00F0,
};
COLOUR colours[] = {FG_DARK_BLUE, FG_DARK_GREEN, FG_DARK_RED, FG_DARK_MAGENTA, FG_GREEN, FG_MAGENTA, FG_YELLOW, FG_WHITE};

enum PIXEL_TYPE
{
    PIXEL_SOLID = 0x2588,
    PIXEL_THREEQUARTERS = 0x2593,
    PIXEL_HALF = 0x2592,
    PIXEL_QUARTER = 0x2591,
};

enum pos {
    X = 0,
    Y = 1,
    Z = 2
};

class Vertex {
public:
    Vertex(float aX, float aY, float aZ) {
        x = aX;
        y = aY;
        z = aZ;
    }
    float x;
    float y;
    float z;

};

// 480, 270, 4, 4 will create a 1920x1080 Window, with 480x270 console pixels, each of which being 4 physical pixels. 512x320 also works
int screenWidth = 512;
int screenHeight = 320;
int fontWidth = 4;
int fontHeight = 4;

const double PI = 3.14159265369;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
HANDLE hOriginalConsole;

SMALL_RECT rectWindow;

CHAR_INFO *screenBuffer;

const wchar_t* windowTitle = L"Engine";

float vertices[] = {
        // first square, described from the bottom left, counterclockwise
        -1, -1, 14, // bottom left front
         1, -1, 14, // bottom right front
         1,  1, 14,
         -1,  1, 14,
         -1, -1, 16,
         1, -1, 16,
         1,  1, 16, // ...
         // -1,  1, 16, // top left back

        // second square
        /*-1,  2, 14,
         1,  2, 14,
         1,  4, 14,
        -1,  4, 14,
        -1,  2, 16,
         1,  2, 16,
         1,  4, 16,
        -1,  4, 16,

        // third square
        -1, -4, 14,
         1, -4, 14,
         1, -2, 14,
        -1, -2, 14,
        -1, -4, 16,
         1, -4, 16,
         1, -2, 16,
        -1, -2, 16,

        // right row
         5, -1, 24,
         7, -1, 24,
         7,  1, 24,
         5,  1, 24,
         5, -1, 26,
         7, -1, 26,
         7,  1, 26,
         5,  1, 26,

         5,  2, 24,
         7,  2, 24,
         7,  4, 24,
         5,  4, 24,
         5,  2, 26,
         7,  2, 26,
         7,  4, 26,
         5,  4, 26,

         5, -4, 24,
         7, -4, 24,
         7, -2, 24,
         5, -2, 24,
         5, -4, 26,
         7, -4, 26,
         7, -2, 26,
         5, -2, 26,

        // left row
        -5, -1, 24,
        -7, -1, 24,
        -7,  1, 24,
        -5,  1, 24,
        -5, -1, 26,
        -7, -1, 26,
        -7,  1, 26,
        -5,  1, 26,

        -5,  2, 24,
        -7,  2, 24,
        -7,  4, 24,
        -5,  4, 24,
        -5,  2, 26,
        -7,  2, 26,
        -7,  4, 26,
        -5,  4, 26,

        -5, -4, 24,
        -7, -4, 24,
        -7, -2, 24,
        -5, -2, 24,
        -5, -4, 26,
        -7, -4, 26,
        -7, -2, 26,
        -5, -2, 26,*/
};

// this array stores the indices of all the points that are supposed to be rendered as a triangle.
int triangles[] = {
        // cube consists of 12 triangles, 2 per side
        // each of the entries references a vertex (3 floats) from the vertices array.
        0, 1, 3, // first triangle + front face
        1, 2, 3, // second triangle
        1, 5, 2, // ... + right face
        5, 6, 2,
        4, 5, 7, // back face
        5, 6, 7,
        1, 4, 3, // left face
        4, 7, 3,
        3, 2, 7, // top face
        2, 6, 7,
        0, 1, 4, // bottom face
        1, 5, 4
};

// FOV in degrees
const double FOV = 90;
// FOV in radians
const double radFOV = FOV * (PI/180);
const double farClippingPlane = 50.0f;

float playerX = 0.0f;
float playerY = 0.0f;
float playerZ = 0.0f;
double playerAngleX = 0.0f;
double radPlayerAngleX = 0.0f;
double playerAngleY = 0.0f;
double radPlayerAngleY = 0.0f;
const float playerBaseSpeed = 3.0f;
float playerSpeed = 3.0f;
float turnSpeed = 30.0f;

float dt;

// Set everything you need inside the game-loop or draw-cycle here.
float colourOffset = 0;

int main() {
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
    COORD coord = {(short) screenWidth, (short) screenHeight};
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
    cfi.dwFontSize.X = fontWidth;
    cfi.dwFontSize.Y = fontHeight;
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
    if (screenHeight > bufferInfo.dwMaximumWindowSize.Y) {
        Error(L"[int fontHeight] to big");
        return 6;
    }
    if (screenWidth > bufferInfo.dwMaximumWindowSize.X) {
        Error(L"[int fontWidth] to big");
        return 7;
    }

    // Now set console window size to be bigger again, after changing the buffer.
    // Size now depends on fontSize and resolution/size of the buffer
    rectWindow = {0, 0, static_cast<SHORT>((short) screenWidth - 1), static_cast<SHORT>((short) screenHeight - 1)};
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
    screenBuffer = new CHAR_INFO[screenWidth * screenHeight];
    std::memset(screenBuffer, 0, sizeof(CHAR_INFO) * screenWidth * screenHeight);

    SetConsoleCtrlHandler(nullptr, FALSE);

    // Game initialisation logic
    playerSpeed = playerBaseSpeed;

    // calculate first dt timestep
    dt = 0.0f;
    auto tpLast = std::chrono::system_clock::now();

    // Game loop basically
    while(true) {
        // clear screenBuffer
        std::memset(screenBuffer, 0, sizeof(CHAR_INFO) * screenWidth * screenHeight);

        getInput();

        // after getting input, update everything needed inside draw()
        radPlayerAngleX = playerAngleX * (PI/180);
        radPlayerAngleY = playerAngleY * (PI/180);

        draw();

        // Refresh screen buffer
        SetConsoleActiveScreenBuffer(hConsole);
        // The following code will only display in white, and interpret the char as well as the attribute for each
        // character as its own unicode entity, thus rendering two characters for each element in screenBuffer. Apart
        // from that it works though.
        /*DWORD byteWriteCount;
        WriteConsoleOutputCharacterA(hConsole, reinterpret_cast<LPCSTR>(screenBuffer), screenWidth * screenHeight, {0, 0}, &byteWriteCount);*/
        PSMALL_RECT pRect = &rectWindow;
        const _CHAR_INFO * copyBuffer = screenBuffer;
        WriteConsoleOutputW(hConsole, copyBuffer, coord,{0, 0}, pRect);

        // at last calculate dt
        auto tpCurrent = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tpCurrent - tpLast;
        tpLast = tpCurrent;
        dt = elapsedTime.count();
    }

    return 0;
}

// In here comes all the drawing code, that isn't clearing / refreshing the screenBuffer.
// also ignore the out-commented stuff, it was (is) for testing different ideas.
void draw() {

    // draw the ten debug lamps on top green by default. Also add the scale thingy below
    for (int j = 1; j < 11; j++) {
        screenBuffer[j].Char.UnicodeChar = PIXEL_SOLID;
        screenBuffer[j].Attributes = FG_GREEN;
        if (j % 2 == 0) {
            screenBuffer[screenWidth + j].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[screenWidth + j].Attributes = FG_WHITE;
        }
        else {
            screenBuffer[screenWidth + j].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[screenWidth + j].Attributes = FG_DARK_BLUE;
        }
    }

    // trying to fix the angle rotation thing here.
    for (int e = 0; e < sizeof(vertices) / sizeof(float); e += 3) {
        // getting initial position relative to the player for each vertex
        float vertex[] = {(vertices[e] - playerX), vertices[e+1] - playerY, (vertices[e+2] - playerZ)};

        // rotating the point accordingly. In order for this to work, we need to get the distance from the player
        // (length of a vector) first. Pythagorean Theorem time.
        // Also don't use pow(), because some implementation suffer from floating point issues
        // Legacy rotation (doesn't fully work anyway)
        //float radius = sqrt((vertex[X] * vertex[X]) + (vertex[Z] * vertex[Z]));
        //vertex[X] = vertex[X] + sin(-radPlayerAngleX) * radius;
        //vertex[Z] = vertex[Z] + ((radius * cos(-radPlayerAngleX)) - radius);
        // new implementation below this comment, radius isn't needed too. Does work, although the points are rotated
        // around the initial player position, and not relative to the player.
        // todo: fix this. ^
        float xPos = vertex[X] * cos(radPlayerAngleX) - vertex[Z]  * sin(radPlayerAngleX);
        float zPos = vertex[X] * sin(radPlayerAngleX) + vertex[Z] * cos(radPlayerAngleX);
        vertex[X] = xPos;
        vertex[Z] = zPos;

        // may divide by zero otherwise later in the code. Instead of just not rendering it in that case, I risk the
        // visual inaccuracy and just render the point slightly wrong. Too lazy to figure out a better solution.
        if (vertex[Z] == 0) {
            vertex[Z] = 0.01f;
        }

        // Now only continue rendering if the vertex is within the bounds of the fov. In radians
        double hFov = (FOV / 2) * (PI/180);
        double vFov = (screenHeight / screenWidth) * hFov;
        bool inDistance = vertex[Z] < farClippingPlane;
        bool inBoundsX = vertex[X] < tan(hFov) * vertex[Z] && vertex[X] > -tan(hFov) * vertex[Z];
        bool inBoundsY = vertex[Y] < tan(hFov) * vertex[Z] && vertex[Y] > (tan(hFov) * vertex[Z]) * -1;
        // displaying results of each test individually
        if (!inDistance) {
            screenBuffer[1].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[1].Attributes = FG_RED;
        }
        if (!inBoundsX) {
            screenBuffer[2].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[2].Attributes = FG_RED;
        }
        if (!inBoundsY) {
            screenBuffer[3].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[3].Attributes = FG_RED;
        }

        if (inDistance && inBoundsX && inBoundsY) {
            // how much of half a screen wide/high is occupied
            // divide by zero might occur, this is why I checked for (vertex[Z] == 0) earlier.
            double ratioX = vertex[X] * (1 / (tan(hFov) * vertex[Z]));
            // the commented part makes no difference for some reason. Even though it should increase the portion of
            // the screen that is taken up in a vertical direction. (Assuming Width is greater than Height)
            double ratioY = /*(screenWidth / screenHeight) * */vertex[Y] * (1 / (tan(hFov) * vertex[Z]));

            // Now calculate it into screenSpace
            int screenX = (screenWidth / 2) + (int)(ratioX * screenWidth * 0.5f);
            int screenY = (screenHeight / 2) + (int)(ratioY * screenWidth * 0.5f);

            // Only draw if the results do not exceed the screenBuffer and aren't negative
            int index = screenY * screenWidth + screenX;
            if (index < screenWidth * screenHeight && index > (-screenWidth * screenHeight)) {
                screenBuffer[index].Char.UnicodeChar = PIXEL_SOLID;
                screenBuffer[index].Attributes = FG_GREEN;
            }
            else {
                screenBuffer[4].Char.UnicodeChar = PIXEL_SOLID;
                screenBuffer[4].Attributes = FG_RED;
            }
        }
    }

    // draw pixels to the corners to mark the bounds of screenBuffer array
    // I do this last to render it on top of everything else
    screenBuffer[0].Char.UnicodeChar = PIXEL_SOLID;
    screenBuffer[0].Attributes = FG_RED;
    screenBuffer[screenWidth * screenHeight - 1].Char.UnicodeChar = PIXEL_SOLID;
    screenBuffer[screenWidth * screenHeight - 1].Attributes = FG_RED;
    screenBuffer[(screenWidth * screenHeight) / 2 + (screenWidth / 2)].Char.UnicodeChar = PIXEL_SOLID;
    screenBuffer[(screenWidth * screenHeight) / 2 + (screenWidth / 2)].Attributes = FG_RED;
}

// crappy way to get input ik. No idea how to register mouse inputs yet as well.
void getInput() {
    // basic movement, always relative to the players rotation
    if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
        playerX += sinf(radPlayerAngleX) * playerSpeed * dt;
        playerZ += cosf(radPlayerAngleX) * playerSpeed * dt;
    }
    if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
        playerX -= sinf(radPlayerAngleX) * playerSpeed * dt;
        playerZ -= cosf(radPlayerAngleX) * playerSpeed * dt;
    }
    if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
        playerX -= cosf(radPlayerAngleX) * playerSpeed * dt;
        playerZ += sinf(radPlayerAngleX) * playerSpeed * dt;
    }
    if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
        playerX += cosf(radPlayerAngleX) * playerSpeed * dt;
        playerZ -= sinf(radPlayerAngleX) * playerSpeed * dt;
    }

    //move up and down
    if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
        playerY -= playerSpeed * dt;
    if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
        playerY += playerSpeed * dt;

    // change movement speed.
    if (GetAsyncKeyState((unsigned short)'F') & 0x8000)
        playerSpeed -= 5.0f * dt;
    if (GetAsyncKeyState((unsigned short)'G') & 0x8000)
        playerSpeed = playerBaseSpeed;
    if (GetAsyncKeyState((unsigned short)'H') & 0x8000)
        playerSpeed += 5.0f * dt;

    // Rotation horizontal
    if (GetAsyncKeyState((unsigned short)'J') & 0x8000)
        playerAngleX -= turnSpeed * dt;
    if (GetAsyncKeyState((unsigned short) 'L') & 0x8000)
        playerAngleX += turnSpeed * dt;

    // and vertical
    if (GetAsyncKeyState((unsigned short)'I') & 0x8000)
        playerAngleY += turnSpeed * dt;
    if (GetAsyncKeyState((unsigned short)'K') & 0x8000)
        playerAngleY -= turnSpeed * dt;
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
