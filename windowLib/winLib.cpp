#include "winLib.h"




bool createWindowHandler(wndHandler *handler){
	if(handler == nullptr){
		return false;
	}

	return handler->render.wrapper.createWindow(&handler->render);
	
}



