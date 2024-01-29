#include "winlib.h"
#include "libs/conDebug.h"
#include <assert.h>

int pointerRef(const int* pInt);

int main(){

	conDebug out{"Main"};

	wndHandler handler{};
//	wndHandler handler2{};
//
	assert(createWindowHandler(&handler));
//	assert(createWindowHandler(&handler2));
//
    handler.render.addCmdToBuffer(rect, wndClass::Rect{10, 15, 10, 15});
    handler.render.addCmdToBuffer(rect,wndClass::Rect{wndClass::stroke{3,wndClass::color{0.0f,1.0f,0.0f}},
                                                      30,130,30,130});
    handler.render.addCmdToBuffer(line, wndClass::Line{80,80,200,200});
//    handler2.render.addCmdToBuffer(rect, wndClass::Rect{40, 40, 50, 50});
//
	wndWindowLoop();

	return 0;
}



