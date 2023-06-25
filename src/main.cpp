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

#include "window.h"

int main() {
    Window::Window(320, 180, 4, 4);
    Window::createWindow();

    while (true) {
        Window::updateWindow();
    }
}
