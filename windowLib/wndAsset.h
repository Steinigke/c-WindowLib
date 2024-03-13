//
// Created by jstei on 11/03/2024.
//

#ifndef WNDLIB_WNDASSET_H
#define WNDLIB_WNDASSET_H

struct wndRender;

struct wndAsset{

    float x, y;

    virtual bool updateBuffer(wndRender &ren, float xOffset = 0, float yOffset = 0) = 0;

};


#endif //WNDLIB_WNDASSET_H
