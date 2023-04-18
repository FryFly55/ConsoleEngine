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

int Error(const wchar_t* msg);
static BOOL CloseHandler(DWORD evt);
void draw();
void getInput();
void safeDraw(int x, int y, PIXEL_TYPE pixelType, COLOUR colour);
void safeDraw(int x, int y, wchar_t chars, COLOUR colour); // function overloading
void drawDigits(int i, int digit, int initialX, int initialY);

// 480, 270, 4, 4 will create a 1920x1080 Window, with 480x270 console pixels, each of which being 4 physical pixels. 512x320 also works
int screenWidth = 480; // 512
int screenHeight = 270; // 320
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
         -1,  1, 16, // top left back

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
        0, 4, 3, // left face
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
const double farClippingPlane = 100.0f;

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

// game variables and toggles
// starting with the line render option
bool doLineRender = true;
float lineToggleCD = 0.5f;
float lineToggleCounter = 0.0f;

// coloured edges and faces
bool doColourDebug = false;
float colourDebugToggleCD = 0.5f;
float colourDebugCounter = 0.0f;

float dt;
float fps;
int fpsOffset = 10; // fps-display distance from the right screen border

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

    // calculate first dt timestep (or is it a delta-timestep? delta-time-step? delta time-step? To not use time twice?)
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

        // we need the fps value in draw(), so initialise it earlier.
        fps = 1 / dt;
        // remove the line below if you want fps shown with decimal places (kinda useless in most situations)
        fps = std::floor(fps + 0.5f); // rounding by flooring the value + 0.5

        lineToggleCounter += dt;
        colourDebugCounter += dt;

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
            /*screenBuffer[screenWidth + j].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[screenWidth + j].Attributes = FG_WHITE;*/
            safeDraw(j, 1, PIXEL_SOLID, FG_WHITE);
        }
        else {
            /*screenBuffer[screenWidth + j].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[screenWidth + j].Attributes = FG_DARK_BLUE;*/
            safeDraw(j, 1, PIXEL_SOLID, FG_BLACK);
        }
    }

    // only needs to be calculated once technically, but in case I ever want it to be changed dynamically during
    // runtime, it's calculated every frame.
    double hFov = (FOV / 2) * (PI/180);

    // todo: arrays are filled with garbage values, if something fails and those are accessed, the program may crash!
    // iterates through the first of three vertex-indices of each triangle
    for (int t = 0; t < sizeof(triangles)/sizeof(int); t += 3) {
        // stores the transformed x,y and z components of each vertex
        float screenSpaceVertices[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        // stores the screenX,Y as well as transformed Z component of each vertex / point
        float screenPoints[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

        for (int i = 0; i < 3; i++) {
            // one vertex relative to the player
            int point = triangles[t+i];
            float x = vertices[point*3] - playerX;
            float y = vertices[point*3+1] - playerY;
            float z = vertices[point*3+2] - playerZ;

            // transform (rotate) the vertices directly in here, before filling in screenSpaceVertices[]
            float xPos = x * cos(radPlayerAngleX) - z  * sin(radPlayerAngleX);
            float zPos = x * sin(radPlayerAngleX) + z * cos(radPlayerAngleX);
            x = xPos;
            z = zPos;

            // don't want to get a 'divide by zero' error (although it isn't even an official exception in c++)
            if (z == 0) {
                z = 0.01f;
            }

            // calculate the screenX and screenY of each point, check if they are within bounds beforehand.
            bool inDistance = z < farClippingPlane;
            bool inBoundsX = x < tan(hFov) * z && x > -tan(hFov) * z;
            bool inBoundsY = y < tan(hFov) * z && y > -tan(hFov) * z;

            if (!inDistance) { // this is just a bunch of debug code todo: remove later.
                /*screenBuffer[1].Char.UnicodeChar = PIXEL_SOLID;
                screenBuffer[1].Attributes = FG_RED;*/
                safeDraw(1, 0, PIXEL_SOLID, FG_RED);
            }
            if (!inBoundsX) {
                /*screenBuffer[2].Char.UnicodeChar = PIXEL_SOLID;
                screenBuffer[2].Attributes = FG_RED;*/
                safeDraw(2, 0, PIXEL_SOLID, FG_RED);
            }
            if (!inBoundsY) {
                /*screenBuffer[3].Char.UnicodeChar = PIXEL_SOLID;
                screenBuffer[3].Attributes = FG_RED;*/
                safeDraw(3, 0, PIXEL_SOLID, FG_RED);
            }

            if (inDistance && inBoundsX && inBoundsY) {
                // now populate screenSpaceVertices[]
                screenSpaceVertices[i*3] = x;
                screenSpaceVertices[i*3+1] = y;
                screenSpaceVertices[i*3+2] = z;

                double ratioX = x * (1 / (tan(hFov) * z)); // how much of half a screen wide/high is occupied
                double ratioY = /*(screenWidth / screenHeight) * */y * (1 / (tan(hFov) * z)); // acc. for aspect ratio

                // Now calculate it into screenSpace, populate screenPoints[] array
                screenPoints[i*3] = (screenWidth / 2) + (int)(ratioX * screenWidth * 0.5f);
                screenPoints[i*3+1] = (screenHeight / 2) + (int)(ratioY * screenWidth * 0.5f);
                screenPoints[i*3+2] = z; // maybe I will normalise/scale z somehow later.
            }
        }

        // Now that all corners of the triangle are calculated, we can start drawing it.
        for (int i = 0; i < 3 && doLineRender; i++) {
            int curX = (int)screenPoints[i*3]; // should always be int values, the array is only float because of z
            int curY = (int)screenPoints[i*3+1];
            int targetX = screenPoints[0];
            int targetY = screenPoints[1];

            if (i < 2) {
                targetX = (int)screenPoints[(i+1)*3];
                targetY = (int)screenPoints[((i+1)*3)+1];
            }

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
            COLOUR renderColour = FG_RED;
            float triangle = t/3;
            if (triangle == 0 || triangle == 2)
                renderColour = FG_WHITE;
            else if (triangle == 1 || triangle == 3)
                renderColour = FG_BLUE;
            else if (triangle > 3)
                renderColour = FG_YELLOW;

            // set the std colour
            if (!doColourDebug)
                renderColour = FG_BLUE;

            float bothZero = deltaX + deltaY;
            // render the lines between points
            if (deltaX < 1 && deltaX > -1 && bothZero != 0) {
                while (curY != targetY) {
                    curY += ySign;
                    safeDraw(curX, curY, PIXEL_SOLID, renderColour);
                }
            }
            if (deltaY < 1 && deltaY > -1 && bothZero != 0) {
                while (curX != targetX) {
                    curX += xSign;
                    safeDraw(curX, curY, PIXEL_SOLID, renderColour);
                }
            }

            if (abs(deltaX) >= abs(deltaY) && bothZero != 0) {
                while (curX != targetX && curY != targetY) {
                    curX += xSign;
                    yOff += yRat;
                    if (yOff >= curY + ySign)
                        curY = (int) std::floor(yOff);
                    safeDraw(curX, curY, PIXEL_SOLID, renderColour);
                }
            }
            else if (abs(deltaY) > abs(deltaX) && bothZero != 0) {
                while (curX != targetX && curY != targetY) {
                    curY += ySign;
                    xOff += xRat;
                    if (xOff >= curX + xSign)
                        curX = (int) std::floor(xOff);
                    safeDraw(curX, curY, PIXEL_SOLID, renderColour);
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            /*screenBuffer[(int)screenPoints[i*3+1] * screenWidth + (int)screenPoints[i*3]].Char.UnicodeChar = PIXEL_SOLID;
            screenBuffer[(int)screenPoints[i*3+1] * screenWidth + (int)screenPoints[i*3]].Attributes = FG_GREEN;*/
            safeDraw((int)screenPoints[i*3], (int)screenPoints[i*3+1], PIXEL_SOLID, FG_GREEN);
        }
    }

    // render the fps-counter
    std::string sFps = std::to_string(fps);
    int digits = sFps.length(); // u_long would be better, but int is fine for our purposes.
    // I will assume that the fps will never exceed 6 digits (unlikely anyway considering my (non-) optimisation)
    for (int i = 0; i < digits && i < 6; i++) {
        int digit = sFps[i] - 48; // converts a char to a number value, by shifting it 48 ascii positions
        if (digit <= 9 && digit >= 0) {
            drawDigits(i, digit, screenWidth - (6 * 4) - fpsOffset, 0);
        }
    }

    // draw pixels to the corners to mark the bounds of screenBuffer array
    // I do this last to render it on top of everything else
    safeDraw(0, 0, PIXEL_SOLID, FG_RED);
    safeDraw(screenWidth - 1, screenHeight - 1, PIXEL_SOLID, FG_RED);
    safeDraw(screenWidth / 2, screenHeight / 2, PIXEL_SOLID, FG_RED);
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

    if ((GetAsyncKeyState((unsigned short)'1') & 0x8000) && lineToggleCounter > lineToggleCD) {
        doLineRender = !doLineRender;
        lineToggleCounter = 0.0f;
    }
    if ((GetAsyncKeyState((unsigned short)'2') & 0x8000) && colourDebugCounter > colourDebugToggleCD) {
        doColourDebug = !doColourDebug;
        colourDebugCounter = 0.0f;
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

void safeDraw(int x, int y, PIXEL_TYPE pixelType, COLOUR colour) {
    if (y * screenWidth + x < screenWidth * screenHeight) {
        screenBuffer[y * screenWidth + x].Char.UnicodeChar = pixelType;
        screenBuffer[y * screenWidth + x].Attributes = colour;
    }
}

void safeDraw(int x, int y, wchar_t chars, COLOUR colour) {
    if (y * screenWidth + x < screenWidth * screenHeight) {
        screenBuffer[y * screenWidth + x].Char.UnicodeChar = chars;
        screenBuffer[y * screenWidth + x].Char.UnicodeChar = colour;
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
}


// drawing a pixelMap to the screen, x, y mark the top left corner.
void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY) {
    for (int i = 0; i < mapX * mapY; i++) {
        int screenY = y + ((int) i / mapX); // division
        int screenX = x + (i % mapX); // and remainder, hope it gets optimised by the compiler to not divide twice
        if (pixelMap[i] == '#') {
            safeDraw(screenX, screenY, PIXEL_SOLID, FG_WHITE);
        }
        else {
            safeDraw(screenX, screenY, PIXEL_SOLID, FG_BLACK);
        }
    }
}

// x, y mark the top left corner
void drawDigits(int i, int digit, int initialX, int initialY) {
    // todo: make an array implementation, for now a switch statement will do though
    switch(digit) {
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
