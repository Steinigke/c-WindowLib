//
// Created by jstei on 11/03/2024.
//

#ifndef WNDLIB_COLOR_H
#define WNDLIB_COLOR_H

namespace winLib {
    struct Color {

        float r, g, b, a;

        Color();
        Color(int hex);
        Color(float r, float g, float b, float a = 1.0f);


    };
}

#endif //WNDLIB_COLOR_H
