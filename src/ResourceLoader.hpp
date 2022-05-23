//
// Created by turlakovai on 28.03.2022.
//

#include <string>
#include <iostream>
#include "raylib.h"
#include "../libs/cmrc.hpp"

CMRC_DECLARE(skns);
using namespace std;

Image loadTextureFromMemory(std::string path) {
    auto fileSystem = cmrc::skns::get_filesystem();
    auto file = fileSystem.open(path);
    string num = string(file.begin(), file.end());
    return LoadImageFromMemory(".png", (const unsigned char *) num.c_str(), num.length());
}