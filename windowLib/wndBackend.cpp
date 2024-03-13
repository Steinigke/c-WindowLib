#ifndef UNICODE
#define UNICODE

#endif

#include "libs/conDebug.h"
#include "wndBackend.h"
#include <d2d1.h>
#include <string>
#include <thread>

//////////////////////////////////////////////////////////////////////
//																	//
//							  WRAPPER								//
//																	//
//////////////////////////////////////////////////////////////////////



unsigned int wndWrapper::count{0};
unsigned int wndWrapper::idc{0};

LRESULT CALLBACK wndWrapper::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    auto render = reinterpret_cast<wndRender *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch(uMsg){
			case WM_CLOSE:
				--count;
				DestroyWindow(hwnd);

            {
                conDebug out{"", "dsWnd::", "[id = " + std::to_string(render->id) + "]"};
                out("Window Destoryed");
                std::thread([render](){
                    render->wrapper.pWindowTarget.reset();
                    render->wrapper.pFactory.reset();
                    render->wrapper.pBrush.reset();
                }).detach();
            }
				return 0;
			
			case WM_CREATE:{
                conDebug out{"","csWnd",""};
                auto *pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
                auto *self = static_cast<wndRender*>(pCreate->lpCreateParams);
                self->wrapper.hwnd = hwnd;
                if(!self->wrapper.createRenderObjects())
                    return -1;

                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
               }
				return 0;
			case WM_DESTROY:
				if(count == 0){
					PostQuitMessage(0);
					return 0;
				}
				
			case WM_PAINT:
				{
				std::thread([render](){
                    PAINTSTRUCT ps{};
                    BeginPaint(render->wrapper.hwnd,&ps);
                    render->onPaint();
                    EndPaint(render->wrapper.hwnd, &ps);
				}).detach();
                return 0;
				}
        default:	return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

}

bool wndWrapper::createRenderObjects() {
    ID2D1Factory* pFac;
    if(FAILED( D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFac))){
        return false;
    }
    pFactory.reset(pFac);
    RECT r;
    GetClientRect(hwnd, &r);
    ID2D1HwndRenderTarget* pRenderTarget;
    if(FAILED(pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
                                                   D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(r.right, r.bottom)),
                                                   &pRenderTarget)))
        return false;
    pWindowTarget.reset(pRenderTarget);
    ID2D1SolidColorBrush* pBr;
    if(FAILED(pWindowTarget->CreateSolidColorBrush(D2D1::ColorF{1.0f,0.0f,0.0f}, &pBr))){
        return false;
    }
    pBrush.reset(pBr);

    return true;
}

bool wndWrapper::createWindow(wndRender *render) const {
	conDebug out{"", "crWnd::", "[id = " + std::to_string(idc) + "]"};
	WNDCLASS wc{};	
	std::wstring cn = std::to_wstring(idc);
    render->id = idc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = windowProc;
	wc.lpszClassName = cn.c_str();
	out(render);
	RegisterClass(&wc);
	CreateWindowEx(
		0,
		wc.lpszClassName,
		L"",
		WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr,
		nullptr,
		wc.hInstance,
		render
	);
	if(hwnd == nullptr){
		out("windowFailed to Create");

		return false;
	}
	out("windowSucceded to Create");
	++count;
	render->id = idc;
	++idc;
	return true;
	
}

void wndWrapper::onPaint(char* buffer, unsigned int id, const winLib::Color& background){
    std::unique_lock l{drawMutex};

    conDebug out(" Paint","opWnd::","[id = " + std::to_string(id) + "]");
    if(!pFactory || !pWindowTarget || !pBrush){
        if(!createRenderObjects()){
            return;
        }
    }
    out("Calling BeginDraw");
    pWindowTarget->BeginDraw();

    unsigned int drawCounter{0};

    if(!buffer){
		out("i swear i am painting");
        return;
	}

	out("Enter drawingLoop");
	drawTypes type;
    pWindowTarget->Clear(D2D1::ColorF(0.0f,0.0f,0.0f));
	do{
        type = static_cast<drawTypes>(buffer[drawCounter]);
        ++drawCounter;
		switch(type){
			case end: 
				out("Last drawing Complet");
				break;
			
			case rect: {
                auto *rect = reinterpret_cast<wndClass::Rect *>(&buffer[drawCounter]);
//                out("drawing Rect");
                pBrush->SetColor(rect->str.getDrawingClr());
                D2D1_RECT_F rc = rect->getDrawingRect();
                pWindowTarget->DrawRectangle(&rc,pBrush.get(),rect->str.thickness);
                drawCounter += rect->next;
                break;
            }
            case rectFill: {
//                out("drawing Filled Rect");
                auto *rectFill = reinterpret_cast<wndClass::RectFill *>(&buffer[drawCounter]);
                pBrush->SetColor(rectFill->str.getDrawingClr());
                D2D1_RECT_F rc = rectFill->getDrawingRect();
                pWindowTarget->FillRectangle(rc, pBrush.get());
                drawCounter += rectFill->next;
            }
                break;

            case line: {
                auto *line = reinterpret_cast<wndClass::Line *>(&buffer[drawCounter]);
                pBrush->SetColor(line->str.getDrawingClr());
                pWindowTarget->DrawLine(line->getDrawingStart(),line->getDrawingEnd(),pBrush.get(),line->str.thickness);
                drawCounter += line->next;
                break;
            }

            case ellipse: {

                auto *ellipse = reinterpret_cast<wndClass::Ellipse*>(&buffer[drawCounter]);
                pBrush->SetColor(ellipse->str.getDrawingClr());

                D2D1_ELLIPSE e = D2D1::Ellipse(D2D1::Point2F(ellipse->x, ellipse->y) , ellipse->radiusX, ellipse->radiusY);

                if(ellipse->filled){
                    pWindowTarget->FillEllipse(e, pBrush.get());
                } else {
                    pWindowTarget->DrawEllipse(e, pBrush.get(), ellipse->str.thickness);
//                    pWindowTarget->Draw
                }

                drawCounter += ellipse->next;
            }
			default: out("HOLY SHIT WHAT IS HAPPENING");
		}
		
	}while(type != end);
    out("Calling EndDraw");
	if(pWindowTarget->EndDraw() == D2DERR_RECREATE_TARGET){
        pWindowTarget.reset();
        pFactory.reset();
        pBrush.reset();
    }

}


//////////////////////////////////////////////////////////////////////
//																	//
//							  RENDERER								//
//																	//
//////////////////////////////////////////////////////////////////////

wndRender::wndRender() : wrapper{}, id{0}{
	
	// drawBuffer(new char[BUFFER_SIZE]);
	
}

void wndRender::onPaint(){
	drawBuffer[drawBufferSize] = static_cast<char>(end);
	wrapper.onPaint(drawBuffer.get(), id);
	
}

//////////////////////////////////////////////////////////////////////
//																	//
//							  	CMD	  								//
//																	//
//////////////////////////////////////////////////////////////////////


wndClass::Stroke::Stroke(float thick, winLib::Color clr) : thickness{thick}, clr{clr}{}

wndClass::Stroke::Stroke() : clr{1.0f, 1.0f, 1.0f} , thickness(3.0f){}

D2D1_COLOR_F wndClass::Stroke::getDrawingClr() const {
            return D2D1_COLOR_F{clr.r, clr.g, clr.b, clr.a};
}

wndClass::Rect::Rect(float t, float b, float l, float r) : top{t}, bottom{b}, left{l}, right{r} {
    next = sizeof(Rect);
}

wndClass::Rect::Rect(wndClass::Stroke str, float t, float b, float l, float r) : top{t}, bottom{b}, left{l}, right{r}   {
    next = sizeof(Rect);
    this->str = str;
}

D2D1_RECT_F wndClass::Rect::getDrawingRect() const{
    return D2D1_RECT_F{left,top,right,bottom};
}

wndClass::RectFill::RectFill(winLib::Color clr, float t, float b, float l, float r) : Rect(Stroke{0, clr},t,b,l,r) {}


wndClass::Line::Line(wndClass::Stroke, float xS, float yS, float xE, float yE) : xStart{xS}, yStart{yS}, xEnd{xE}, yEnd{yE}{
    next = sizeof(Line);
    this->str = str;
}

wndClass::Line::Line(float xS, float yS, float xE, float yE) : xStart{xS}, yStart{yS}, xEnd{xE}, yEnd{yE}{
    next = sizeof(Line);
}

D2D1_POINT_2F wndClass::Line::getDrawingStart() const{
    return D2D1_POINT_2F{xStart, yStart};
}

D2D1_POINT_2F wndClass::Line::getDrawingEnd() const{
    return {xEnd,yEnd};
}


//////////////////////////////////////////////////////////////////////
//																	//
//							Window Loop								//
//																	//
//////////////////////////////////////////////////////////////////////

void wndWindowLoop(){
	MSG msg = { };
	while (GetMessage(&msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}