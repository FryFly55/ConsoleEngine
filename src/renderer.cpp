// manages the render pipeline and when to do which calculations

//
// Created by FryFly55 on 16.06.2023.
//

#include "renderer.h"
#include "worldspace.h"
#include "scene.h"

int Renderer::rasterize() {
    // transforming worldspace coordinates into viewspace coordinates
    vec3<float> playerPos = {0, 0, 0};
    vec3<float> angles = {10, -10, 0};
    Worldspace::relate(playerPos, Scene::vertices);
    Worldspace::rotate(angles, Scene::vertices);
}