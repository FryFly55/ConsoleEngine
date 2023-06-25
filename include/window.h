//
// Created by FryFly55 on 14.06.2023.
//

#ifndef CONSOLEENGINE_WINDOW_H
#define CONSOLEENGINE_WINDOW_H

#include <windows.h>
#include <chrono>
#include <list>
#include <thread>
#include <atomic>
#include <cstring>
#include <string>
#include <cmath>

// use namespace instead of class, since the class would be static either way, and the best replacement for what would
// be static classes in C# are namespaces in c++. Everything declared within the header can be accessed globally,
// everything declared in the cpp can only be accessed within the namespace. I also only need one window, as console
// windows are tied to the application, and Windows only gives one console to me anyway.
namespace Window {
    int Window(int width, int height, int fontWidth, int fontHeight);
    int createWindow();
    int updateWindow();
    int Error(const wchar_t* msg);
};


#endif //CONSOLEENGINE_WINDOW_H
