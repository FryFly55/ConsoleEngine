//
// Created by FryFly55 on 22.04.2023.
//

#include <fstream>
#include <iostream>
#include <list>
#include "initialiser.h"
#include "rasterizer.h"
#include <string>

// will set up all config variables at application start

void initVertices() {
    // first read the vertexSource line by line
    std::ifstream fVertex(config::vertexPath);
    std::string row;
    std::cout << "Vertices" << std::endl;
    while (std::getline(fVertex, row)) {
        // filter out wrongly formatted, empty or commented lines
        if (row[0] == '#' || row.size() != 32)
            continue;

        std::cout << row << std::endl;

        std::string sx;
        std::string sy;
        std::string sz;
        for (int i = 7; i < 14; i++) {
            sx += row[i];
        }
        for (int i = 16; i < 23; i++) {
            sy += row[i];
        }
        for (int i = 25; i < 32; i++) {
            sz += row[i];
        }
        float x = (float) std::stod(sx);
        float y = (float) std::stod(sy);
        float z = (float) std::stof(sz);
        std::cout << x << ", " << y << ", " << z << std::endl;
        Renderer::vertices[Renderer::cVertices] = x;
        Renderer::vertices[Renderer::cVertices + 1] = y;
        Renderer::vertices[Renderer::cVertices + 2] = z;
        Renderer::cVertices += 3;
    }
    fVertex.close();
}

void initTriangles() {
    // read triangles source line by line
    std::ifstream fTriangles(config::trianglePath);
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

float getFloatFromRow(std::string row) {
    std::string sub;
    bool passedEquals = false;
    for (int i = 0; i < row.size(); i++) {
        if (row[i] == '=') {
            passedEquals = true;
            continue;
        }
        if (passedEquals) {
            sub += row[i];
        }
    }

    return std::stof(sub);
}

bool getBoolFromRow(std::string row) {
    for (int i = 0; i < row.size(); i++) {
        if (row[i] == '=') {
            if (row[i+1] == 'f' || row[i+1] == '0')
                return false;
            else
                return true;
        }
    }

    return false;
}

bool getIntFromRow(std::string row) {
    std::string sub;
    bool passedEquals = false;
    for (int i = 0; i < row.size(); i++) {
        if (row[i] == '=') {
            passedEquals = true;
            continue;
        }
        if (passedEquals) {
            sub += row[i];
        }
    }

    return std::stoi(sub);
}

double getDoubleFromRow(std::string row) {
    std::string sub;
    bool passedEquals = false;
    for (int i = 0; i < row.size(); i++) {
        if (row[i] == '=') {
            passedEquals = true;
            continue;
        }
        if (passedEquals) {
            sub += row[i];
        }
    }

    return std::stod(sub);
}

void initGame() {

}

void init() {
    initVertices();
    initTriangles();
    initGame();
}