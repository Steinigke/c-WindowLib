//
// Created by jstei on 11/03/2024.
//

#include "Color.h"


    winLib::Color::Color(int hex) {
        int mask{3};

        b = static_cast<float>(hex & mask) / 255.0f;
        hex >> 2;
        g = static_cast<float>(hex & mask) / 255.0f;
        hex >> 2;
        r = static_cast<float>(hex & mask) / 255.0f;
    }

    winLib::Color::Color(float r, float g, float b, float a) : r{r}, g{g} , b{b}, a{a} {}

winLib::Color::Color() : r{1}, g{1}, b{1}, a{1} {}
