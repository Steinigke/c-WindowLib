#ifndef WNDBACK
#define WNDBACK
#include <windows.h>
#include <memory>
#include <d2d1.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

#include <mutex>
#include "Color.h"

struct wndRender;
struct wndWrapper;

enum drawTypes{
	end=0,
    rect,
	rectFill,
	line,
    ellipse
};

struct wndWrapper{

	static unsigned int count;
	static unsigned int idc;
	
	static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	
	
	HWND hwnd;

    std::unique_ptr<ID2D1Factory> pFactory;
    std::unique_ptr<ID2D1HwndRenderTarget> pWindowTarget;
    std::unique_ptr<ID2D1SolidColorBrush> pBrush;

    std::mutex drawMutex;

    bool createRenderObjects();

	bool createWindow(wndRender *render) const;



	void onPaint(char* buffer, unsigned int id, const winLib::Color& background);
};

struct wndRender{
	
	wndRender();
	unsigned int id{0};
	wndWrapper wrapper;

	unsigned int drawBufferSize{0};
	std::unique_ptr<char[]> drawBuffer{new char[BUFFER_SIZE]};
	
	void onPaint();
	
	template<typename command>
	bool addCmdToBuffer(drawTypes type, command cmd){
		
		int sizeCmd = cmd.next;
		
		if(sizeCmd + drawBufferSize + 1 >= BUFFER_SIZE){
			return false;
		}
		
		drawBuffer[drawBufferSize] = static_cast<char>(type);
		++drawBufferSize;
		memcpy(&drawBuffer[drawBufferSize], &cmd, sizeCmd);
		drawBufferSize += sizeCmd;
		return true;
	};
	
};

void wndWindowLoop();

namespace wndClass {

    struct Stroke {

        float thickness;
        winLib::Color clr;

        Stroke();
        Stroke(float thickness, winLib::Color clr);

        D2D1_COLOR_F getDrawingClr() const;


    };

    struct Cmd {
        unsigned int next;
        Stroke str;

    };

    struct Rect : Cmd {
        float top;
        float bottom;
        float left;
        float right;

        Rect(float t, float b, float l, float r);
        Rect(Stroke str, float t, float b, float l, float r);

        D2D1_RECT_F getDrawingRect() const ;
    };

    struct RectFill : Rect {

        RectFill(winLib::Color clr, float t, float b, float l, float r);

    };

    struct Ellipse : Cmd {

        float x;
        float y;
        float radiusX;
        float radiusY;
        bool filled;


        Ellipse(float xS, float xY, float radiusX, float radiusY);
        Ellipse(float xS, float xY, float radius);


    };

    struct Line : Cmd {
        float xStart;
        float yStart;
        float xEnd;
        float yEnd;

        Line(Stroke str, float xS, float yS, float xE, float yE);
        Line(float xS, float yS, float xE, float yE);

        D2D1_POINT_2F getDrawingStart() const;

        D2D1_POINT_2F getDrawingEnd() const;
    };

}

#endif
