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
    int rtn = Window::createWindow();
    if (rtn != 0) { return rtn; }

    Window::run();

    return 0;
}
