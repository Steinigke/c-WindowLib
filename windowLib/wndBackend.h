#ifndef WNDBACK
#define WNDBACK
#include <windows.h>
#include <memory>
#include <d2d1.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

struct wndRender;
struct wndWrapper;
struct color;

enum drawTypes{
	end=0,
    rect,
	rectFill,
	line
};

struct wndWrapper{
	
	static unsigned int count;
	static unsigned int idc;
	
	static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	
	
	HWND hwnd;

    std::unique_ptr<ID2D1Factory> pFactory;
    std::unique_ptr<ID2D1HwndRenderTarget> pWindowTarget;
    std::unique_ptr<ID2D1SolidColorBrush> pBrush;

    bool createRenderObjects();

	bool createWindow(wndRender *render);



	void onPaint(char* buffer, int id);
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

    struct color {
        float r, g, b, a;

        color(float r, float g, float b, float a = 1.0f);

        D2D1_COLOR_F operator()();
    };

    struct stroke {

        float thickness;
        color clr;

        stroke();

        stroke(float thickness, wndClass::color clr);

    };

    struct cmd {
        unsigned int next;
        stroke str;

    };

    struct Rect : cmd {
        float top;
        float bottom;
        float left;
        float right;

        Rect(float t, float b, float l, float r);
        Rect(stroke str, float t, float b, float l, float r);

        D2D1_RECT_F getDrawingRect();
    };

    struct RectFill : Rect {
        color fill;

    };

    struct Line : cmd {
        float xStart;
        float yStart;
        float xEnd;
        float yEnd;

        Line(stroke str, float xS, float yS, float xE, float yE);
        Line(float xS, float yS, float xE, float yE);

        D2D1_POINT_2F getDrawingStart();

        D2D1_POINT_2F getDrawingEnd();


    };

}

#endif
