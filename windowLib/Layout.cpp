//
// Created by jstei on 11/03/2024.
//

#include "Layout.h"
#include <limits>
#include "wndBackend.h"

using namespace winLib;

void Layout::setDimensions(float nWidth, float nHeight) {
    if(nWidth < 0)
        nWidth = std::numeric_limits<float>::infinity();
    if(nHeight < 0)
        nHeight = std::numeric_limits<float>::infinity();

    width = nWidth;
    height = nHeight;
}


bool Layout::updateBuffer(wndRender &ren, float xOffset, float yOffset) {

//    ren.addCmdToBuffer(rectFill, wndClass::RectFill{})

    for(wndAsset &t : members) {
        if(t.x > width || t.y > height){
            continue;
        }

        t.updateBuffer(ren, x, y);

    }
    return true;
}


void wndLayout::addAsset(wndAsset asset) {


}