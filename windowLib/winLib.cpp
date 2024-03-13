#include "winLib.h"
#include "wndBackend.h"

using namespace winLib;


bool wndHandler::createWindow() {
    render = std::make_unique<wndRender>();
    return render->wrapper.createWindow(render.get());

}


wndHandler::~wndHandler() = default;
wndHandler::wndHandler() = default;


void windowLoop(){
    wndWindowLoop();
}


