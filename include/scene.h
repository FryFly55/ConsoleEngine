//
// Created by FryFly55 on 17.06.2023.
//

#ifndef CONSOLEENGINE_SCENE_H
#define CONSOLEENGINE_SCENE_H

namespace Scene {
    float vertices[] = {
            -1, -1, -1,
            +1, -1, -1,
            +1, +1, -1,
            -1, +1, -1,
            -1, -1, +1,
            +1, -1, +1,
            +1, +1, +1,
            -1, +1, +1,
    };

    int triangles[] = {
            0, 1, 3, // front
            1, 3, 2,
            1, 5, 2, // right
            5, 2, 6,
            4, 6, 5, // back
            4, 6, 7,
            0, 7, 4, // left
            0, 7, 3,
            3, 2, 5, // top
            3, 5, 7,
            0, 5, 4, // bottom
            0, 5, 1
    };
}

#endif //CONSOLEENGINE_SCENE_H
