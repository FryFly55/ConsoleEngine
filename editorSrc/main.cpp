//
// Created by FryFly55 on 25.04.2023.
//

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

void readFile(std::string filepath, std::vector<std::string> &src);
void initHeaders(std::vector<std::string> &verticesFile, std::vector<std::string> &trianglesFile);

std::vector<std::string> verticesFile;
std::string vertexPath = "./config/vertices.txt";
int cVertices;
int cVObj;
std::vector<std::string> trianglesFile;
std::string trianglePath = "./config/triangles.txt";
int cTriangles;
int cTObj;

int main() {
    readFile(vertexPath, verticesFile);
    readFile(trianglePath, trianglesFile);

    initHeaders(verticesFile, trianglesFile);
}

void readFile(std::string filepath, std::vector<std::string> &src) {
    std::ifstream file(filepath);
    std::string row;

    while (std::getline(file, row)) {
        if (row[0] == '#' || row.size() < 2)
            continue;

        src.push_back(row);
    }
}

void initHeaders(std::vector<std::string> &verticesFile, std::vector<std::string> &trianglesFile) {
    // could I have used a function for both files? maybe... todo: make one and fix this shit
    for (int i = 0; i < verticesFile.size(); i++) {
        if (verticesFile[i][0] != '$')
            continue;

        int passedEquals = 0;
        for (int j = 0; j < verticesFile[i].size(); j++) {
            if (verticesFile[i][j] == '=')
                passedEquals++;

            // yeah ik the if does not matter, idgaf
            if (passedEquals == 1) {
                int startIndex = j++;
                std::string sub;
                for (int k = startIndex; k < verticesFile[i].size() && verticesFile[i][k] != ' '; k++) {
                    sub += verticesFile[i][k];
                }
                cVertices = std::stoi(sub);
            }
            if (passedEquals == 2) {
                int startIndex = j++;
                std::string sub;
                for (int k = startIndex; k < verticesFile[i].size() && verticesFile[i][k] != ' '; k++) {
                    sub += verticesFile[i][k];
                }
                cVObj = std::stoi(sub);
            }
        }
        break;
    }

    for (int i = 0; i < trianglesFile.size(); i++) {
        if (trianglesFile[i][0] != '$')
            continue;

        int passedEquals = 0;
        for (int j = 0; j < trianglesFile[i].size(); j++) {
            if (trianglesFile[i][j] == '=')
                passedEquals++;

            // yeah ik the if does not matter, idgaf
            if (passedEquals == 1) {
                int startIndex = j++;
                std::string sub;
                for (int k = startIndex; k < trianglesFile[i].size() && trianglesFile[i][k] != ' '; k++) {
                    sub += trianglesFile[i][k];
                }
                cTriangles = std::stoi(sub);
            }
            if (passedEquals == 2) {
                int startIndex = j++;
                std::string sub;
                for (int k = startIndex; k < trianglesFile[i].size() && trianglesFile[i][k] != ' '; k++) {
                    sub += trianglesFile[i][k];
                }
                cTObj = std::stoi(sub);
            }
        }
        break;
    }
    std::cout << cVertices << ", " << cVObj << std::endl;
    std::cout << cTriangles << ", " << cTObj << std::endl;
}
