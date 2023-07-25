//
// Created by FryFly55 on 16.06.2023.
//

// see "https://www.glfw.org/docs/latest/input_guide.html" for reference

#include "input.h"

namespace Input {
     keyState *keyBuffer = new keyState[500];
}

void Input::keyCallBack(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action < 3) {
        keyBuffer[key] = static_cast<keyState>(action);
    }
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
