//
// Created by fried on 26.06.2023.
//

#include "Camera.h"

int Camera::init(vec3<double> pos, vec3<double> vLook, double aFOV, double aNear, double aFar) {
    position = pos;
    lookVector = vLook;
    FOV = aFOV;
    farClippingPlane = aFar;
    nearClippingPlane = aNear;

    return 0;
}