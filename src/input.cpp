//
// Created by FryFly55 on 16.06.2023.
//

// see "https://www.glfw.org/docs/latest/input_guide.html" for reference

#include "input.h"

namespace Input {
     keyState *keyBuffer = new keyState[500]; // surely enough for any keyboard
     keyState *mouseBuffer = new keyState[20]; // nobody has more than 20 mouse-buttons right?
     double cursorX = 0;
     double cursorY = 0;
     double dCursorX = 0;
     double dCursorY = 0;
     double lastCursorX = 0;
     double lastCursorY = 0;
}

void Input::keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action < 3) {
        keyBuffer[key] = static_cast<keyState>(action);
    }
}

void Input::mouseButtonCallBack(GLFWwindow *window, int button, int action, int mods) {
    if (action < 3) {
        mouseBuffer[button] = static_cast<keyState>(action);
    }
}

void Input::mouseCursorCallBack(GLFWwindow *window, double xPos, double yPos) {
    dCursorX = xPos - lastCursorX;
    dCursorY = yPos - lastCursorY;
    cursorX = xPos;
    cursorY = yPos;
    lastCursorX = xPos;
    lastCursorY = yPos;
}

int Input::updateKeyState() {
    for (int i = 0; i < 500; i++) {
        if (keyBuffer[i] == KEY_PRESSED) {
            keyBuffer[i] = KEY_HELD;
        }
        else if (keyBuffer[i] == KEY_JUST_RELEASED) {
            keyBuffer[i] = KEY_RELEASED;
        }
    }

    for (int i = 0; i < 20; i++) {
        if (mouseBuffer[i] == KEY_PRESSED) {
            mouseBuffer[i] = KEY_HELD;
        }
        else if (mouseBuffer[i] == KEY_JUST_RELEASED) {
            mouseBuffer[i] = KEY_RELEASED;
        }
    }

    return 0;
}

int Input::updateMouseState() {
    dCursorX = 0;
    dCursorY = 0;

    return 0;
}

bool Input::keyPressed(int key) {
    return keyBuffer[key] == KEY_PRESSED;
}

bool Input::keyHeld(int key) {
    return keyBuffer[key] == KEY_HELD;
}

bool Input::keyReleased(int key) {
    return keyBuffer[key] == KEY_RELEASED;
}

bool Input::keyJustReleased(int key) {
    return keyBuffer[key] == KEY_JUST_RELEASED;
}

bool Input::getKey(int key) {
    return keyBuffer[key] == KEY_PRESSED || keyBuffer[key] == KEY_HELD;
}

Input::keyState Input::getKeyState(int key) {
    return keyBuffer[key];
}

bool Input::buttonPressed(int button) {
    return mouseBuffer[button] == KEY_PRESSED;
}

bool Input::buttonHeld(int button) {
    return mouseBuffer[button] == KEY_HELD;
}

bool Input::buttonReleased(int button) {
    return mouseBuffer[button] == KEY_RELEASED;
}

bool Input::buttonJustReleased(int button) {
    return mouseBuffer[button] == KEY_JUST_RELEASED;
}

bool Input::getButton(int button) {
    return mouseBuffer[button] == KEY_PRESSED || mouseBuffer[button] == KEY_HELD;
}

Input::keyState Input::getButtonState(int button) {
    return mouseBuffer[button];
}
