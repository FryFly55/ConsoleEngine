//
// Created by FryFly55 on 24.04.2023.
//

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


namespace Renderer {
    float triangles[1000];
    int cTriangles = 0;
}

int main() {
    // read triangles source line by line
    std::ifstream fTriangles("config/triangles.txt");
    std::string row;
    std::cout << "Triangles" << std::endl;
    while (std::getline(fTriangles, row)) {
        // filter out wrongly formatted, empty or commented lines
        if (row[0] == '#' || row.size() != 23)
            continue;

        std::cout << row << std::endl;

        std::string sx;
        std::string sy;
        std::string sz;
        for (int i = 7; i < 11; i++) {
            sx += row[i];
        }
        for (int i = 13; i < 17; i++) {
            sy += row[i];
        }
        for (int i = 19; i < 23; i++) {
            sz += row[i];
        }
        int x = std::stoi(sx);
        int y = std::stoi(sy);
        int z = std::stoi(sz);
        std::cout << x << ", " << y << ", " << z << std::endl;
        Renderer::triangles[Renderer::cTriangles] = x;
        Renderer::triangles[Renderer::cTriangles + 1] = y;
        Renderer::triangles[Renderer::cTriangles + 2] = z;
        Renderer::cTriangles += 3;
    }
    fTriangles.close();
}
