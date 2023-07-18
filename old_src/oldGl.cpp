/*
 *  made by FryFly, 2023-03-29
 */

/*
 * Does not work anymore, My guess is that windows deprecated some older
 * legacy api features, that were needed for the project to work.
 * Anyway, resizing font, window and screenBuffer does not work anymore.
 */

#include <windows.h>

#include <chrono>
#include <list>
#include <thread>
#include <atomic>
#include <cstring>
#include <string>
#include <cmath>
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <iostream>

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

int Error(const wchar_t* msg);
void draw();
void getInput();
void safeDraw(int x, int y, PIXEL_TYPE pixelType, COLOUR colour);
void drawDigits(int i, int digit, int initialX, int initialY);
void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY);
void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY, PIXEL_TYPE pixel, COLOUR colour);

// 480, 270, 4, 4 will create a 1920x1080 Window, with 480x270 console pixels, each of which being 4 physical pixels. 512x320 also works
const int screenWidth = 512; // 480
const int screenHeight = 320; // 270
const int fontWidth = 4;
const int fontHeight = 4;

const double PI = 3.14159265369;

GLubyte screenBuffer[screenWidth * screenHeight * 3];
GLubyte windowBuffer[screenWidth * fontWidth * screenHeight * fontHeight * 3];

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
        -1,  2, 14,
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
        -5, -2, 26,
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
        1, 5, 4,

        /*8, 9, 11, // front face
        9, 10, 11,
        9, 13, 10, // right face
        13, 14, 10,
        12, 13, 15, // back face
        13, 14, 15,
        8, 12, 15, // left face
        12, 15, 11,
        11, 10, 15, // top face
        10, 14, 15,
        8, 9, 12, // bottom face
        9, 13, 12*/
};

const char* crosshair = {
        "..#.."
        "..#.."
        "#####"
        "..#.."
        "..#.."
};
int crosshairWidth = 5;
int crosshairHeight = 5;
PIXEL_TYPE crosshairPixelType = PIXEL_SOLID;
COLOUR crosshairColour = FG_RED;

// FOV in degrees
const double FOV = 70;
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

GLFWwindow* m_window;

// Set everything you need inside the game-loop or draw-cycle here.
float colourOffset = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void translateBuffers() {
    for (int x = 0; x < screenWidth * 3; x+=3) {
        for (int y = 0; y < screenHeight * 3; y+=3) {
            for (int i = 0; i < fontHeight*3; i+=3) {
                for (int j = 0; j < fontWidth*3; j+=3) {
                    windowBuffer[(y*fontHeight*screenWidth*fontWidth+i)+(x*fontWidth+j)] =
                            screenBuffer[y*screenWidth+x];
                    windowBuffer[(y*fontHeight*screenWidth*fontWidth+i)+(x*fontWidth+j)+1] =
                            screenBuffer[y*screenWidth+x+1];
                    windowBuffer[(y*fontHeight*screenWidth*fontWidth+i)+(x*fontWidth+j)+2] =
                            screenBuffer[y*screenWidth+x+2];

                }
            }
        }
    }
}

int main() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(screenWidth * fontWidth, screenHeight * fontHeight, "OldGL", NULL, NULL);
    m_window = window;
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Game initialisation logic
    playerSpeed = playerBaseSpeed;

    // calculate first dt timestep (or is it a delta-timestep? delta-time-step? delta time-step? To not use time twice?)
    dt = 0.0f;
    auto tpLast = std::chrono::system_clock::now();

    // Game loop basically
    while(!glfwWindowShouldClose(window)) {
        // clear screenBuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (int i = 0; i < sizeof(windowBuffer)/sizeof(GLubyte); i++) {
            windowBuffer[i] = 0;
        }
        for (int i = 0; i < sizeof(screenBuffer)/sizeof(GLubyte); i++) {
            screenBuffer[i] = 0;
        }

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

        translateBuffers();

        glDrawPixels(screenWidth * fontWidth, screenHeight * fontHeight, GL_RGB, GL_UNSIGNED_BYTE, windowBuffer);

        glfwSwapBuffers(m_window);
        glfwPollEvents();

        // at last calculate dt
        auto tpCurrent = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tpCurrent - tpLast;
        tpLast = tpCurrent;
        dt = elapsedTime.count();
    }
    glfwTerminate();

    return 0;
}

// In here comes all the drawing code, that isn't clearing / refreshing the screenBuffer.
// also ignore the out-commented stuff, it was (is) for testing different ideas.
void draw() {

    // only needs to be calculated once technically, but in case I ever want it to be changed dynamically during
    // runtime, it's calculated every frame.
    double hFov = (FOV / 2) * (PI/180);

    // todo: arrays are filled with garbage values, if something fails and those are accessed, the program may crash!
    // iterates through the first of three vertex-indices of each triangle
    for (int t = 0; t < sizeof(triangles)/sizeof(int); t += 3) {
        // stores the transformed x,y and z components of each vertex
        float screenSpaceVertices[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
        // stores the screenX,Y as well as transformed Z component of each vertex / point
        float screenPoints[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

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

            if (inDistance && inBoundsX) {
                // now populate screenSpaceVertices[]
                screenSpaceVertices[i*3] = x;
                screenSpaceVertices[i*3+1] = y;
                screenSpaceVertices[i*3+2] = z;

                double ratioX = x * (1 / (tan(hFov) * z)); // how much of half a screen wide/high is occupied
                double ratioY = y * (1 / (tan(hFov) * z));

                // Now calculate it into screenSpace, populate screenPoints[] array
                screenPoints[i*3] = (screenWidth / 2) + (int)(ratioX * screenWidth * 0.5f);
                screenPoints[i*3+1] = (screenHeight / 2) + (int)(ratioY * screenWidth * 0.5f);

                screenPoints[i*3+2] = z; // maybe I will normalise/scale z somehow later, so it's good to have.
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

            bool curXinProx = curX <= targetX + 1 && curX >= targetX - 1;
            bool curYinProx = curY <= targetY + 1 && curY >= targetY - 1;
            if (abs(deltaX) >= abs(deltaY) && bothZero != 0) {
                while (!curXinProx || !curYinProx) {
                    curX += xSign;
                    yOff += yRat;
                    if (yOff >= curY + ySign)
                        curY = (int) std::floor(yOff);
                    safeDraw(curX, curY, PIXEL_SOLID, renderColour);
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
                    safeDraw(curX, curY, PIXEL_SOLID, renderColour);
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
    //safeDraw(screenWidth / 2, screenHeight / 2, PIXEL_SOLID, FG_RED); // this draws a primitive crosshair

    // this draws a more advanced crosshair
    drawPixelMap((screenWidth / 2) - std::floor(crosshairWidth / 2),
                 (screenHeight / 2) - std::floor(crosshairHeight / 2), crosshair, crosshairWidth, crosshairHeight,
                 crosshairPixelType, crosshairColour);
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
    std::cout << msg << std::endl;
    return 0;
}

void safeDraw(int x, int y, PIXEL_TYPE pixelType, COLOUR colour) {
    if (y * screenWidth + x < screenWidth * screenHeight && y >= 0 && x >= 0) {
        screenBuffer[y*screenWidth+x*3] = 255;
        screenBuffer[y*screenWidth+x*3+1] = 255;
        screenBuffer[y*screenWidth+x*3+2] = 255;
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

void drawPixelMap(int x, int y, const char* pixelMap, int mapX, int mapY, PIXEL_TYPE pixel, COLOUR colour) {
    for (int i = 0; i < mapX * mapY; i++) {
        int screenY = y + ((int) i / mapX); // division
        int screenX = x + (i % mapX); // and remainder, hope it gets optimised by the compiler to not divide twice
        if (pixelMap[i] == '#') {
            safeDraw(screenX, screenY, pixel, colour);
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