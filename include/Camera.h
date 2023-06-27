//
// Created by fried on 26.06.2023.
//

#ifndef CONSOLEENGINE_CAMERA_H
#define CONSOLEENGINE_CAMERA_H

#include "math.h"

namespace Camera {
    struct vec3<double> position;
    struct vec3<double> lookVector;

    double FOV;
    double farClippingPlane;
    double nearClippingPlane;

    int init(vec3<double> pos, vec3<double> vLook,
             double aFOV, double aNear, double aFar);
}

#endif //CONSOLEENGINE_CAMERA_H
