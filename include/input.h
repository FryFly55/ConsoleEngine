//
// Created by FryFly55 on 16.06.2023.
//

#ifndef CONSOLEENGINE_INPUT_H
#define CONSOLEENGINE_INPUT_H

#include "glad/glad.h"
#include "glfw/glfw3.h"

namespace Input {
    enum keyState {
        KEY_JUST_RELEASED,
        KEY_PRESSED,
        KEY_HELD,
        KEY_RELEASED
    };
    extern keyState *keyBuffer; // 0 -> released, 1 -> just pressed, 2 -> held
    extern keyState *mouseBuffer;
    // Cursor movement over the window
    extern double cursorX;
    extern double cursorY;
    extern double lastCursorX;
    extern double lastCursorY;
    extern double dCursorX;
    extern double dCursorY;

    void keyCallBack(GLFWwindow* window, int key, int scancode,
                     int action, int mods);
    void mouseButtonCallBack(GLFWwindow* window, int button,
                             int action, int mods);
    void mouseCursorCallBack(GLFWwindow* window, double xPos, double yPos);
    int updateKeyState();
    // keyboard functions
    bool keyPressed(int key);
    bool keyHeld(int key);
    bool keyReleased(int key);
    bool keyJustReleased(int key);
    bool getKey(int key); // held or pressed, invert for (just-) released
    keyState getKeyState(int key);
    // mouse functions
    bool buttonPressed(int button);
    bool buttonHeld(int button);
    bool buttonReleased(int button);
    bool buttonJustReleased(int button);
    bool getButton(int button); // held or pressed
    keyState getButtonState(int button);
}

#endif //CONSOLEENGINE_INPUT_H
