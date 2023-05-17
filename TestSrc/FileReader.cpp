//
// Created by FryFly55 on 24.04.2023.
//

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

void modifyVector(std::vector<std::string> &one, std::string msg);
std::vector<std::string> modifyVector(std::vector<std::string> one, std::string msg, int j);

int main() {
    std::vector<std::string> one = {"String 1", "String 2", "String 3"};
    std::vector<std::string> two = {"Hello 1", "Hello 2", "Hello 3"};

    std::cout << "main one: " << std::endl;
    for (int i = 0; i < one.size(); i++) {
        std::cout << one.at(i) << std::endl;
    }
    std::cout << "main two: " << std::endl;
    for (int i = 0; i < two.size(); i++) {
        std::cout << two.at(i) << std::endl;
    }

    modifyVector(one, "Hello, World!");
    two = modifyVector(two, "Hello, Vector!", 1);

    std::cout << "main one: " << std::endl;
    for (int i = 0; i < one.size(); i++) {
        std::cout << one.at(i) << std::endl;
    }
    std::cout << "main two: " << std::endl;
    for (int i = 0; i < two.size(); i++) {
        std::cout << two.at(i) << std::endl;
    }
}

void modifyVector(std::vector<std::string> &one, std::string msg) {
    one.push_back(msg);
    std::cout << "func one: " << std::endl;

    for (int i = 0; i < one.size(); i++) {
        std::cout << one.at(i) << std::endl;
    }
}

std::vector<std::string> modifyVector(std::vector<std::string> one, std::string msg, int j) {
    one.push_back(msg);
    std::cout << "func: " << std::endl;

    for (int i = 0; i < one.size(); i++) {
        std::cout << one.at(i) << std::endl;
    }

    return one;
}
