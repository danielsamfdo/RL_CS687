#include <stdafx.h>

using namespace std;

Color::Color() {
	r = g = b = 255;
}

Color::Color(const unsigned char r, const unsigned char g, const unsigned char b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

CImgWindow::CImgWindow()
{
	img = nullptr;
#ifdef _DISPLAY_GRAPHICS
	display = nullptr;
#endif
}

// Just give a fresh window
CImgWindow::CImgWindow(const CImgWindow & rhs)
{
	img = nullptr;
#ifdef _DISPLAY_GRAPHICS
	display = nullptr;
#endif
}

// Just give a fresh window
CImgWindow & CImgWindow::operator=(const CImgWindow & rhs)
{
	if (img != nullptr)
	{
		delete img;
		img = nullptr;
	}
#ifdef _DISPLAY_GRAPHICS
	if (display != nullptr)
	{
		delete display;
		display = nullptr;
	}	
	
#endif
	return *this;
}

CImgWindow::~CImgWindow() {
	if (img != nullptr)
		delete img;
#ifdef _DISPLAY_GRAPHICS
	if (display != nullptr)
		delete display;
#endif
}

// @TODO: Add a check for whether the user closed the window. If so, during "draw", don't sleep ever

void CImgWindow::open(
	const int width, const int height,
	const string & title,
	const int xOffset,		// horizontal position of window
	const int yOffset)		// vertical position of window
{
#ifdef _DISPLAY_GRAPHICS
	bool createdDisplay; // Will be loaded with whether or not we created the display
	if (display == nullptr)
	{
		// Create display
		display = new cimg_library::CImgDisplay(
			(const unsigned int)width,
			(const unsigned int)height,
			title.c_str());
		createdDisplay = true;
		// Move it
		display->move(xOffset, yOffset);
	}
	else
	{
		createdDisplay = false;
		// Display already created - just resize and move it
		if ((display->width() != width) || (display->height() != height))
			display->resize(width, height);

		display->set_title(title.c_str());
	}
#endif

	// Create the image to display on the display
	if ((img != nullptr) &&
		((img->width() != width) || (img->height() != height)))
	{
		delete img;
		img = nullptr;
	}
	if (img == nullptr)
	{
		img = new cimg_library::CImg<unsigned char>(width, height, 1, 3, 1);
	}
	clearScreen(WHITE);
#ifdef _DISPLAY_GRAPHICS
	if (createdDisplay) // If not, don't force white background
		display->display(*img);	// Draw white img for background
#endif
}

void CImgWindow::close()
{
#ifdef _DISPLAY_GRAPHICS
	if (display != nullptr)
		delete display;
	display = nullptr;
#endif
	if (img != nullptr)
		delete img;
	img = nullptr;
}

void CImgWindow::drawLine(
	const int x1,
	const int y1,
	const int x2,
	const int y2,
	const Color color,
	const double opacity)
{
	if (img == nullptr) // Don't actually do anything if the display is off
		return;

	int height = img->height();
	const unsigned char c[3] = { color.r, color.g, color.b };
	img->draw_line(x1, height - 1 - y1, x2, height - 1 - y2, c, (float)opacity);
}

void CImgWindow::drawTriangle(
	const int x0,
	const int y0,
	const int x1,
	const int y1,
	const int x2,
	const int y2,
	const Color color,
	bool filled,
	const double opacity)
{
	if (img == nullptr) // Don't actually do anything if the display is off
		return;

	int height = img->height();
	const unsigned char c[3] = { color.r, color.g, color.b };
	if (filled)
	{
		img->draw_triangle(x0, height - y0 - 1, x1, height - y1 - 1, x2,
			height - y2 - 1, c, (float)opacity);
	}
	else
	{
		img->draw_triangle(x0, height - y0 - 1, x1, height - y1 - 1, x2,
			height - y2 - 1, c, (float)opacity, -1);
	}
}

void CImgWindow::drawRectangle(
	const int x0,
	const int y0,
	const int x1,
	const int y1,
	const Color color,
	bool filled,
	const double opacity)
{
	if (img == nullptr) // Don't actually do anything if the display is off
		return;

	int height = img->height();
	const unsigned char c[3] = { color.r, color.g, color.b };
	if (filled)
		img->draw_rectangle(x0, height - y0 - 1, x1, height - y1 - 1,
			c, (float)opacity);
	else
		img->draw_rectangle(x0, height - y0 - 1, x1, height - y1 - 1,
			c, (float)opacity, -1);
}

void CImgWindow::drawCircle(
	const int x,
	const int y,
	const int radius,
	const Color color,
	bool filled,
	const double opacity)
{
	if (img == nullptr) // Don't actually do anything if the display is off
		return;

	int height = img->height();
	const unsigned char c[3] = { color.r, color.g, color.b };
	if (filled)
		img->draw_circle(x, height - y - 1, radius, c, (float)opacity);
	else
		img->draw_circle(x, height - y - 1, radius, c, (float)opacity, -1);
}

void CImgWindow::drawText(
	const int x,
	const int y,
	const string & text,
	const Color color)
{
	if (img == nullptr) // Don't actually do anything if the display is off
		return;

	int height = img->height();
	const unsigned char c[3] = { color.r, color.g, color.b };
	img->draw_text(x, height - y - 1, text.c_str(), c);
}

void CImgWindow::clearScreen(const Color color)
{
	if (img == nullptr) // Don't actually do anything if the display is off
		return;

	const unsigned char c[3] = { color.r, color.g, color.b };
	img->draw_rectangle(0, 0, img->width() - 1, img->height() - 1, c);
}

int CImgWindow::getWidth()
{
	if (img != nullptr)		
		return img->width();
	return -1;
}

int CImgWindow::getHeight() {
	if (img != nullptr)
		return img->height();
	return -1;
}

void CImgWindow::update() {	// Actually draw the stuff that was loaded
#ifdef _DISPLAY_GRAPHICS
	if ((display == nullptr) || (img == nullptr)) // Don't actually do anything if the display is off
		return; 
	
	display->display(*img);
#endif
}

int CImgWindow::getMouseX() {	// negative if off the screen
#ifdef _DISPLAY_GRAPHICS
	if (display != nullptr)
		return display->mouse_x();
#endif
	return -1; // Avoid compile errors about returning values
}

int CImgWindow::getMouseY() {	// negative if off the screen
#ifdef _DISPLAY_GRAPHICS
	if (display != nullptr)
		return img->height() - display->mouse_y() - 1;
#endif
	return -1; // Avoid compile errors about returning values
}

/*
Wait for a click and return the mouse position at click
Sets x,y < 0 if off the screen somehow
*/
void CImgWindow::waitClick(
	int & xBuff,
	int & yBuff)
{
#ifdef _DISPLAY_GRAPHICS
	if (img == nullptr) // Don't actually do anything if the display is off
	{
		xBuff = -1;
		yBuff = -1;
		return;
	}

	while (display->button() == 0)
		display->wait();
	xBuff = display->mouse_x();
	yBuff = img->height() - display->mouse_y() - 1;
#endif
}

// Wait for a click
void CImgWindow::waitClick()
{
#ifdef _DISPLAY_GRAPHICS
	if (img == nullptr) // Don't actually do anything if the display is off
		return;
	while (display->button() == 0)
		display->wait();
#endif
}

/*
Returns true if there was a left click. Also, only returns true if the last
time it returned true was msGap milliseconds ago.
*/
bool CImgWindow::leftClick(double msGap) {
#ifdef _DISPLAY_GRAPHICS
	if ((img != nullptr) &&
		(display->button() & 1) &&
		(leftClickTimer.getMS() > msGap))
	{
		leftClickTimer.start();
		return true;
	}
#endif
	return false; // Avoid warning that it must return a value
}

/*
Returns true if there was a right click. Only returns true if the last
time it returned true was msGap milliseconds ago.
*/
bool CImgWindow::rightClick(double msGap) {
#ifdef _DISPLAY_GRAPHICS
	if ((img != nullptr) &&
		(display->button() & 2) &&
		(rightClickTimer.getMS() > msGap))
	{
		rightClickTimer.start();
		return true;

	}
#endif
	return false; // Avoid warning that it must return a value
}

// Reposition the window
void CImgWindow::move(
	const int newX,
	const int newY) {
#ifdef _DISPLAY_GRAPHICS
	if (display != nullptr)
		display->move(newX, newY);
#endif
}

// Returns true if the window is closed, false if there is an active display window
bool CImgWindow::isClosed() {
#ifdef _DISPLAY_GRAPHICS
	return ((display == nullptr) || (img == nullptr) || (display->is_closed()));
#endif
	return true;
}

bool CImgWindow::userClosed() {
#ifdef _DISPLAY_GRAPHICS
	return ((display != nullptr) && (display->is_closed()));
#endif
	return true;
}

void CImgWindow::save(
	const std::string & fileName,
	int number,
	int digits)
{
	img->save(fileName.c_str(), number, digits);
}
