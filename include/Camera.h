//
// Created by fried on 26.06.2023.
//

#ifndef CONSOLEENGINE_CAMERA_H
#define CONSOLEENGINE_CAMERA_H

#pragma once

#include "math.h"

namespace Camera {
    extern struct vec3<double> position;
    extern struct vec3<double> lookVector;

    extern double FOV;
    extern double rFOV;
    extern double farClippingPlane;
    extern double nearClippingPlane;
    extern float angle;
    extern float radAngle;
    extern float speed;
    extern float turnSpeed;

    int init(vec3<double> pos, vec3<double> vLook,
             double aFOV, double aNear, double aFar, float aSpeed, float aTurnSpeed);
    int update();
}

#endif //CONSOLEENGINE_CAMERA_H
