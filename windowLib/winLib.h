#ifndef WNDLIB
#define WNDLIB

#include <vector>
#include <memory>
#include "Color.h"
#include "wndAsset.h"
#include "Layout.h"

struct wndRender;

namespace winLib {

    struct wndHandler {

        ~wndHandler();
        wndHandler();
//        wndLayout lay;


        bool createWindow();


    private:
        std::unique_ptr<wndRender> render;

    };
}
void windowLoop();

#endif