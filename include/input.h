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

    void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    int updateKeyState();
    bool keyPressed(int key);
    bool keyHeld(int key);
    bool keyReleased(int key);
    bool keyJustReleased(int key);
    bool keyPressedOrHeld(int key);
    keyState getKeyState(int key);
}

#endif //CONSOLEENGINE_INPUT_H
