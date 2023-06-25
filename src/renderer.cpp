// manages the render pipeline and when to do which calculations

//
// Created by FryFly55 on 16.06.2023.
//

#include "renderer.h"
#include "worldspace.h"

int Renderer::rasterize() {
    // transforming worldspace coordinates into viewspace coordinates
    Worldspace::relate();
    Worldspace::rotate();
}