//
// Created by FryFly55 on 16.06.2023.
//

#ifndef CONSOLEENGINE_WORLDSPACE_H
#define CONSOLEENGINE_WORLDSPACE_H

#pragma once

#include "math.h"
#include <vector>

namespace Worldspace {
    // saves the vertices after transformation -> how they appear
    // in viewspace.
    extern float relVertices[];

    // transforms all vertices relative to camera position
    int relate(vec3<double> vec, std::vector<float> vertices);
    // transforms all vertices relative to camera rotation ->
    // every position is relative to cameras x,y,z axis
    int rotate(vec3<double> vec, std::vector<float> vertices);
}

#endif //CONSOLEENGINE_WORLDSPACE_H
