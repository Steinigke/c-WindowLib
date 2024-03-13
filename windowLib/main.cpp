#include "winlib.h"
#include "libs/conDebug.h"
#include <memory>
#include <assert.h>

int pointerRef(const int* pInt);

int main(){

	conDebug out{"Main"};

	winLib::wndHandler handler{};
    handler.createWindow();

//	wndHandler handler2{};
//
//	assert(handler.);
//	assert(createWindowHandler(&handler2));
//
//    handler.render.addCmdToBuffer(rect, wndClass::Rect{10, 15, 10, 15});
//    handler.render.addCmdToBuffer(rect,wndClass::Rect{wndClass::Stroke{3, wndClass::Color{0.0f, 1.0f, 0.0f}},
//                                                      30,130,30,130});
//    handler.render.addCmdToBuffer(line, wndClass::Line{80,80,80,200});
//    handler2.render.addCmdToBuffer(rect, wndClass::Rect{40, 40, 50, 50});
//
	windowLoop();

	return 0;
}



