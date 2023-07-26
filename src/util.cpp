//
// Created by fried on 24.07.2023.
//

#include "util.h"
#include "commons.h"

int Util::flip(bool &b) {
    b = !b;

    return 0;
}

bool Util::checkBoundsX(float value) {
    if (value < Window::screenWidth && value >= 0)
        return true;
    return false;
}

bool Util::checkBoundsY(float value) {
    if (value < Window::screenHeight && value >= 0)
        return true;
    return false;
}

bool Util::checkBounds(float value) {
    if (value < Window::screenWidth * Window::screenHeight && value >= 0)
        return true;
    return false;
}

bool Util::oCheckBoundsX(float value) {
    if (value < Window::windowWidth && value >= 0)
        return true;
    return false;
}

bool Util::oCheckBoundsY(float value) {
    if (value < Window::windowHeight && value >= 0)
        return true;
    return false;
}

bool Util::oCheckBounds(float value) {
    if (value < Window::windowWidth * Window::windowHeight && value >= 0)
        return true;
    return false;
}

bool Util::checkBoundsX(int value) {
    if (value < Window::screenWidth && value >= 0)
        return true;
    return false;
}

bool Util::checkBoundsY(int value) {
    if (value < Window::screenHeight && value >= 0)
        return true;
    return false;
}

bool Util::checkBounds(int value) {
    if (value < Window::screenWidth * Window::screenHeight && value >= 0)
        return true;
    return false;
}

bool Util::oCheckBoundsX(int value) {
    if (value < Window::windowWidth && value >= 0)
        return true;
    return false;
}

bool Util::oCheckBoundsY(int value) {
    if (value < Window::windowHeight && value >= 0)
        return true;
    return false;
}

bool Util::oCheckBounds(int value) {
    if (value < Window::windowWidth * Window::windowHeight && value >= 0)
        return true;
    return false;
}