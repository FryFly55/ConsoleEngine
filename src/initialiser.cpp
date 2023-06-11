//
// Created by FryFly55 on 22.04.2023.
//

#include <fstream>
#include <iostream>
#include <list>
#include "initialiser.h"
#include "rasterizer.h"
#include <vector>

// will set up all config variables at application start

void initVertices() {
    // first read the vertexSource line by line
    std::ifstream fVertex(config::vertexPath);
    std::string row;
    std::cout << "Vertices" << std::endl;
    while (std::getline(fVertex, row)) {
        // filter out wrongly formatted, empty or commented lines
        if (row[0] == '#' || row.size() != 25 || row[0] == '[' || row[0] == '/')
            continue;

        std::cout << row << std::endl;

        std::string sx;
        std::string sy;
        std::string sz;
        for (int i = 0; i < 7; i++) {
            sx += row[i];
        }
        for (int i = 9; i < 16; i++) {
            sy += row[i];
        }
        for (int i = 18; i < 25; i++) {
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
        if (row[0] == '#' || row.size() != 16 || row[0] == '[' || row[0] == '/')
            continue;

        std::cout << row << std::endl;

        std::string sx;
        std::string sy;
        std::string sz;
        for (int i = 0; i < 4; i++) {
            sx += row[i];
        }
        for (int i = 6; i < 10; i++) {
            sy += row[i];
        }
        for (int i = 12; i < 16; i++) {
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
        if (passedEquals) {
            sub += row[i];
        }
        if (row[i] == '=') {
            passedEquals = true;
            continue;
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

int getIntFromRow(std::string row) {
    std::string sub;
    bool passedEquals = false;
    for (int i = 0; i < row.size(); i++) {
        if (passedEquals) {
            sub += row[i];
        }
        if (row[i] == '=') {
            passedEquals = true;
            continue;
        }
    }

    return std::stoi(sub);
}

double getDoubleFromRow(std::string row) {
    std::string sub;
    bool passedEquals = false;
    for (int i = 0; i < row.size(); i++) {
        if (passedEquals) {
            sub += row[i];
        }
        if (row[i] == '=') {
            passedEquals = true;
            continue;
        }
    }

    return std::stod(sub);
}

void initGame() {
    std::vector<std::string> rows;
    std::ifstream fConfig(config::configPath);
    std::string row;
    while (std::getline(fConfig, row)) {
        if (row[0] == '#' || row.size() < 2)
            continue;
        rows.push_back(row);
        std::cout << row << std::endl;
    }

    Window::screenWidth = getIntFromRow(rows.at(0));
    Window::screenHeight = getIntFromRow(rows.at(1));
    Window::fontWidth = getIntFromRow(rows.at(2));
    Window::fontHeight = getIntFromRow(rows.at(3));
    Window::PI = getDoubleFromRow(rows.at(5));

    GameVar::FOV = getDoubleFromRow(rows.at(13));
    GameVar::farClippingPlane = getDoubleFromRow(rows.at(14));
    GameVar::playerX = getFloatFromRow(rows.at(6));
    GameVar::playerY = getFloatFromRow(rows.at(7));
    GameVar::playerZ = getFloatFromRow(rows.at(8));
    GameVar::playerAngleX = getFloatFromRow(rows.at(9));
    GameVar::playerAngleY = getFloatFromRow(rows.at(10));
    GameVar::playerBaseSpeed = getFloatFromRow(rows.at(11));
    GameVar::turnSpeed = getFloatFromRow(rows.at(12));
    GameVar::doLineRender = getBoolFromRow(rows.at(15));
    GameVar::lineToggleCD = getFloatFromRow(rows.at(16));
    GameVar::doColourDebug = getBoolFromRow(rows.at(17));
    GameVar::colourDebugToggleCD = getFloatFromRow(rows.at(18));
    GameVar::showFpsDecimals = getBoolFromRow(rows.at(19));
    GameVar::fpsDecimalDebugToggleCD = getFloatFromRow(rows.at(20));

    // UI::crosshair = getStringFromRow(); todo: implement this
    UI::crosshairWidth = getIntFromRow(rows.at(22));
    UI::crosshairHeight = getIntFromRow(rows.at(23));
}

void init() {
    initVertices();
    initTriangles();
    initGame();
}