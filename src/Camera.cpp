//
// Created by fried on 26.06.2023.
//

#include "Camera.h"
#include "commons.h"
#include "math.h"
#include "input.h"
#include <cmath>
#include "window.h"

namespace Camera {
    int init(vec3<double> pos, vec3<double> vLook, double aFOV,
             double aNear, double aFar, float aSpeed, float aTurnSpeed) {
        position = pos;
        lookVector = vLook;
        FOV = aFOV;
        farClippingPlane = aFar;
        nearClippingPlane = aNear;
        rFOV = FOV * (PI / 180);
        angle = 0;
        radAngle = 0;
        speed = aSpeed;
        turnSpeed = aTurnSpeed;

        return 0;
    }

    struct vec3<double> position;
    struct vec3<double> lookVector;

    double FOV;
    double rFOV;
    double farClippingPlane;
    double nearClippingPlane;
    float angle;
    float radAngle;
    float speed;
    float turnSpeed;

}

int Camera::update() {
    // rotation
    if (Input::getKey(GLFW_KEY_J)) {
        angle -= turnSpeed * Window::dt;
    }
    if (Input::getKey(GLFW_KEY_L)) {
        angle += turnSpeed * Window::dt;
    }
    angle += Input::dCursorX * Window::dt;
    // angle should only ever be between -360 and 360, technically not necessary but might prevent bugs in the future.
    // only 2 additional if statements every frame, so performance hit is minimal
    if (angle >= 360) {
        angle = angle - 360;
    }
    else if (angle <= -360) {
        angle = angle + 360;
    }
    radAngle = angle * (PI / 180);

    // sideways movement
    if (Input::getKey(GLFW_KEY_W)) {
        position.x += sinf(radAngle) * speed * Window::dt;
        position.z += cosf(radAngle) * speed * Window::dt;
    }
    if (Input::getKey(GLFW_KEY_S)) {
        position.x -= sinf(radAngle) * speed * Window::dt;
        position.z -= cosf(radAngle) * speed * Window::dt;
    }
    if (Input::getKey(GLFW_KEY_A)) {
        position.x -= cosf(radAngle) * speed * Window::dt;
        position.z += sinf(radAngle) * speed * Window::dt;
    }
    if (Input::getKey(GLFW_KEY_D)) {
        position.x += cosf(radAngle) * speed * Window::dt;
        position.z -= sinf(radAngle) * speed * Window::dt;
    }

    // up and down movement
    if (Input::getKey(GLFW_KEY_Q)) {
        position.y -= speed * Window::dt;
    }
    if (Input::getKey(GLFW_KEY_E)) {
        position.y += speed * Window::dt;
    }

    return 0;
}