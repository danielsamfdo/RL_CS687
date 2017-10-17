#pragma once

#include <stdafx.h>

class Color
{
public:
	Color();

	Color(const unsigned char r, const unsigned char g, const unsigned char b);

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

// Some colors from https://en.wikipedia.org/wiki/Web_colors

// Pink colors
const Color PINK = { 255, 192, 203 };
const Color PINK_HOT = { 255, 105, 180 };
const Color PINK_DEEP = { 255, 20, 147 };

// Red colors
const Color RED = { 255, 0, 0 };
const Color RED_SALMON = { 250, 128, 114 };
const Color RED_CRIMSON = { 220, 20, 60 };
const Color RED_DARK = { 139, 0, 0 };

// Orange colors
const Color ORANGE_RED = { 255, 69, 0 };
const Color ORANGE_DARK = { 255, 140, 0 };

// Yellow colors
const Color YELLOW = { 255, 255, 0 };
const Color YELLOW_KHAKI = { 240, 230, 140 };
const Color YELLOW_GOLD = { 255, 215, 0 };

// Brown colors
const Color BROWN = { 165, 42, 42 };
const Color BROWN_SADDLE = { 139, 69, 19 };
const Color BROWN_BURLY_WOOD = { 222, 184, 135 };

// Green colors
const Color GREEN_LIME = { 0, 255, 0 };
const Color GREEN_OLIVE = { 85, 107, 47 };
const Color GREEN_DARK = { 0, 100, 0 };
const Color GREEN = { 0, 128, 0 };

// Cyan colors
const Color CYAN = { 0, 255, 255 };
const Color CYAN_AQUAMARINE = { 127, 255, 212 };
const Color CYAN_TEAL = { 0, 128, 128 };

// Blue colors
const Color BLUE = { 0, 0, 255 };
const Color BLUE_LIGHT = { 173, 216, 230 };
const Color BLUE_NAVY = { 0, 0, 128 };
const Color BLUE_DODGER = { 30, 144, 255 };
const Color BLUE_ZAFFRE = { 0, 20, 168 };

// Purple/ violet / magenta colors
const Color PURPLE = { 128, 0, 128 };
const Color PURPLE_INDIGO = { 75, 0, 130 };
const Color PURPLE_MAGENTA = { 255, 0, 255 };
const Color PURPLE_LAVENDER = { 230, 230, 250 };

// White colors
const Color WHITE = { 255, 255, 255 };
const Color WHITE_SNOW = { 255, 250, 250 };

// Gray colors
const Color GRAY = { 128, 128, 128 };
const Color GRAY_LIGHT = { 211, 211, 211 };

// Black
const Color BLACK = { 0, 0, 0 };


/*
Simple interface with CImg to draw images. 
CImg is included, so you can use it directly if you want. 
This interface is easier for simple drawings (like CMUGraphics back in the day).

If _DISPLAY_GRAPHICS is not defined in stdafx.h, then drawing abilities are
disabled (the function calls work, but do nothing). 

Coordinates start in bottom left at 0,0, and go to top right at width-1,
height-1
*/
class CImgWindow
{
public:
	CImgWindow();
	CImgWindow(const CImgWindow & rhs);
	CImgWindow & operator=(const CImgWindow & rhs);

	~CImgWindow();

	void open(const int width = 1024, const int height = 768,
		const std::string & title = (const std::string)"C++ Graphics Window",
		const int xOffset = 100, const int yOffset = 100);

	void close();

	void drawLine(
		const int x1,
		const int y1,
		const int x2,
		const int y2,
		const Color color = BLACK,
		const double opacity = 1.0);

	void drawTriangle(
		const int x0,
		const int y0,
		const int x1,
		const int y1,
		const int x2,
		const int y2,
		const Color color = BLACK,
		bool filled = false,
		const double opacity = 1.0);

	void drawRectangle(
		const int x0,
		const int y0,
		const int x1,
		const int y1,
		const Color color = BLACK,
		bool filled = false,
		const double opacity = 1.0);

	void drawCircle(
		const int x,
		const int y,
		const int radius,
		const Color color = BLACK,
		bool filled = false,
		const double opacity = 1.0);

	void drawText(
		const int x,
		const int y,
		const std::string & text,
		const Color color = BLACK);

	void clearScreen(
		const Color color = WHITE);

	int getWidth();

	int getHeight();

	void update();

	int getMouseX();

	int getMouseY();

	/*
	Wait for a click and return the mouse position at click
	Sets x,y < 0 if off the screen somehow
	*/
	void waitClick(
		int & xBuff,
		int & yBuff);

	// Wait for a click
	void waitClick();

	/*
	Returns true if there was a left click. Also, only returns true if the last
	time it returned true was msGap milliseconds ago.
	*/
	bool leftClick(double msGap = 200);

	/*
	Returns true if there was a right click. Only returns true if the last
	time it returned true was msGap milliseconds ago.
	*/
	bool rightClick(double msGap = 200);

	// Reposition the window
	void move(
		const int newX,
		const int newY);

	// Is the window closed?
	bool isClosed();

	// Did the user close the window (but the window was opened)?
	bool userClosed();

	/*
	Saves to file. fileName extension determines file type. Works with bmp
	always. May with with other formats depending on other installed
	extensions.

	If number is specified, it is appended to the fileName so that you can
	make out1.bmp, out2.bmp, out3.bmp, etc. easily.

	If digits is specified, it says how many digits to use to represent
	number.
	*/
	void save(
		const std::string & fileName,
		int number = -1,
		int digits = 5);


private:
#ifdef _DISPLAY_GRAPHICS
	cimg_library::CImgDisplay * display;
#endif
	cimg_library::CImg<unsigned char> * img;

	Timer leftClickTimer;
	Timer rightClickTimer;
};
