//
// Created by jstei on 11/03/2024.
//

#ifndef WNDLIB_LAYOUT_H
#define WNDLIB_LAYOUT_H

#include "wndAsset.h"
#include "Color.h"
#include <vector>

namespace winLib {

    struct Layout : wndAsset {
    private:
        float width, height;
        std::vector<wndAsset> members;

    public:
        winLib::Color background;

        void setDimensions(float nWidth, float nHeight);

        bool updateBuffer(wndRender &ren, float xOffset, float yOffset) override;
    };

    struct wndLayout {
    private:
        std::vector<wndAsset> members;

    public:
        winLib::Color background;

        void addAsset(const wndAsset& asset);

    };

}
#endif //WNDLIB_LAYOUT_H
