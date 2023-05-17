//
// Created by FryFly55 on 25.04.2023.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

std::vector<std::string> readFile(std::string path, int &fileLength);
std::string getCommand(std::string input);
std::vector<std::string> getArgs(std::string input);
void addCube(std::vector<std::string> args, std::vector<std::string> &vertices, std::vector<std::string> &triangles, int cV, int cT);
int getHeaderInformation(int key, std::vector<std::string> &file);

std::string configPath = "config/game_config.txt";
std::string verticesPath = "config/vertices.txt";
std::string trianglesPath = "config/triangles.txt";

int main() {
    // first read files into collection of rows to edit them. When finished we write the new lines into the files again.
    std::vector<std::string> verticesFile;
    int cLinesVerticesFile = 0;
    int cVertices;
    std::vector<std::string> trianglesFile;
    int cLinesTrianglesFile = 0;
    int cTriangles;

    verticesFile = readFile(verticesPath, cLinesVerticesFile);
    trianglesFile = readFile(trianglesPath, cLinesTrianglesFile);

    cVertices = getHeaderInformation(1, verticesFile);
    cTriangles = getHeaderInformation(1, trianglesFile);

    bool exit = false;
    while (!exit) {
        std::string input;
        std::cout << "$ ";
        std::getline(std::cin, input);

        if (input == "exit") {
            exit = true;
            continue;
        }
        std::string command = getCommand(input);
        std::vector<std::string> args = getArgs(input);

        if (command == "add_cube") {
            addCube(args, verticesFile, trianglesFile, cVertices, cTriangles);
        }
    }

    return 0;
}

// add_cube(x, y, z, width, height, depth, identifier, comment)
void addCube(std::vector<std::string> args, std::vector<std::string> &vertices, std::vector<std::string> &triangles, int cV, int cT) {
    float x = std::stof(args.at(0));
    float y = std::stof(args.at(1));
    float z = std::stof(args.at(2));
    float width = std::stof(args.at(3));
    float height = std::stof(args.at(4));
    float depth = std::stof(args.at(5));
    std::string id = args.at(6);
    std::string comment = args.at(7);

    float points[] = {
            x, y, z,
            x + width, y, z,
            x + width, y + height, z,
            x, y + height, z,
            x, y, z + depth,
            x + width, y, z + depth,
            x + width, y + height, z + depth,
            x, y + height, z + depth,
    };

    int trianglePoints[] = {
            cV, cV + 1, cV + 2,
            cV, cV + 2, cV + 3,
            cV + 1, cV + 5, cV + 6,
            cV + 1, cV + 6, cV + 2,
            cV + 5, cV + 4, cV + 7,
            cV + 5, cV + 7, cV + 6,
            cV + 4, cV, cV + 3,
            cV + 4, cV + 3, cV + 7,
            cV + 3, cV + 2, cV + 6,
            cV + 3, cV + 6, cV + 7,
            cV + 4, cV + 5, cV + 1,
            cV + 4, cV + 1, cV
    };
}

std::vector<std::string> readFile(std::string path, int &fileLength) {
    std::vector<std::string> rows;
    std::ifstream fOpen(path);
    std::string row;
    while(std::getline(fOpen, row)) {
        if (row[0] == '#' || row.size() < 2) {
            continue;
        }
        rows.push_back(row);
        fileLength++;
    }

    return rows;
}

std::string getCommand(std::string input) {
    std::string command;

    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '(')
            break;
        command += input[i];
    }

    return command;
}

std::vector<std::string> getArgs(std::string input) {
    std::vector<std::string> args;
    std::string arg;
    bool passedCheck = false;
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '(') {
            passedCheck = true;
            continue;
        }
        if (passedCheck) {
            if (input[i] == ' ')
                continue;
            if (input[i] != ',' && input[i] != ')') {
                arg += input[i];
            }
            else {
                args.push_back(arg);
                arg = "";
            }
        }
    }

    return args;
}

int getHeaderInformation(int key, std::vector<std::string> &file) {
    std::string row;
    std::string ret;
    for (int i = 0; i < file.size(); i++) {
        row = file.at(i);
        if (row[0] == '$') {
            break;
        }
    }
    int equalsCount = 0;
    int startIndex = -1;
    int endIndex = -1;
    for (int i = 0; i < row.size(); i++) {
        if (row[i] == '=') {
            equalsCount++;
        }
        if (equalsCount == key) {
            startIndex = i++;
            break;
        }
    }
    for (int i = startIndex; i < row.size() && startIndex >= 0; i++) {
        if (row[i] == ',') {
            endIndex = i;
        }
    }

    if (startIndex > 0 && endIndex > 0) {
        for (int i = startIndex; i < endIndex - startIndex; i++)  {
            ret += row[i];
        }
        return std::stoi(ret);
    }
    else {
        return -1;
    }
}

int setHeaderInformation(int key, std::vector<std::string> &file, std::string info) {
    std::string row;
    int rowIndex = -1;
    for (int i = 0; i < file.size(); i++) {
        row = file.at(i);
        if (row[0] == '$') {
            rowIndex = i;
            break;
        }
    }
    if (rowIndex < 0)
        return 1;

    int writeStart = -1;
    int writeEnd = -1;
    int passedEquals = 0;
    for (int i = 0; i < row.size(); i++) {
        if (row[i] == '=') {
            passedEquals++;
        }
        if (passedEquals == key) {
            writeStart = i++;
            break;
        }
    }
    for (int i = writeStart; i < row.size() && writeStart >= 0; i++) {
        if (row[i] == ',') {
            writeEnd = i;
        }
    }
    if (!(writeEnd >= 0 && writeStart >= 0))
        return 2;

    for (int i = writeStart; i < row.size(); i++) {

    }
}
