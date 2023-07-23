//
// Created by fried on 26.06.2023.
//

#include "Camera.h"
#include "math.h"

namespace Camera {
    int init(vec3<double> pos, vec3<double> vLook, double aFOV, double aNear, double aFar) {
        position = pos;
        lookVector = vLook;
        FOV = aFOV;
        farClippingPlane = aFar;
        nearClippingPlane = aNear;
        rFOV = FOV * (PI / 180);

        return 0;
    }

    struct vec3<double> position;
    struct vec3<double> lookVector;

    double FOV;
    double rFOV;
    double farClippingPlane;
    double nearClippingPlane;
}