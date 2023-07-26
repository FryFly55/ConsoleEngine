//
// Created by fried on 24.07.2023.
//

#ifndef CONSOLEENGINE_UTIL_H
#define CONSOLEENGINE_UTIL_H

namespace Util {
    int flip(bool &b);
    // check if a value is within screenBounds
    // multiple functions are faster than one
    // as it omits additional if-statements.
    bool checkBoundsX(float value);
    bool checkBoundsY(float value);
    bool checkBounds(float value);
    bool oCheckBoundsX(float value);
    bool oCheckBoundsY(float value);
    bool oCheckBounds(float value);
    bool checkBoundsX(int value);
    bool checkBoundsY(int value);
    bool checkBounds(int value);
    bool oCheckBoundsX(int value);
    bool oCheckBoundsY(int value);
    bool oCheckBounds(int value);
}

#endif //CONSOLEENGINE_UTIL_H
