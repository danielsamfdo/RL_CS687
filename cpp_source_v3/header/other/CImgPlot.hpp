#pragma once

#include <stdafx.h>

// Object for creating plots.
class CImgPlot
{
public:
	// Create a blank plot object - it is up to the user to specify values.
	CImgPlot();

	// Clean up memory
	~CImgPlot();

	void setWindowSize(const int width, const int height);

	void setTitle(const std::string & title);

	// Each row is a different line to plot
	void setY(const Eigen::MatrixXd & Y);

	// If the x-positions are not 1,...,Y.cols(), they are set here
	void setX(const Eigen::VectorXd & x);

	// Comma-separated labels for each data series
	void setLabels(const std::string & s);

	// Turn legend on or off
	void setLegend(bool on);

	// Set x-axis range
	void setXRange(const double min, const double max);

	// Set step of x-axis marks
	void setXStep(const double step);

	// Set y-axis range
	void setYRange(const double min, const double max);

	// Set step of y-axis marks
	void setYStep(const double step);

	// Turn markers on or off, and set their size and fill
	void setMarkers(bool on, int size = 3);

	// Turn on or off error bars?
	void setErrorBars(bool on);

	void setClearPrevious(bool newValue);

	// Set error bar ranges (does not draw them unless you toggle errorBars(true)
	void setError(const Eigen::MatrixXd & range);

	// Set error bar ranges (does not draw them unless you toggle errorBars(true)
	void setErrors(const Eigen::MatrixXd & upper, const Eigen::MatrixXd & lower);

	// Make curves transparent?
	void setTransparency(const double alpha);

	void setColors(std::vector<Color> colors);

	void setXLabel(const std::string & s);

	void setYLabel(const std::string & s);

	void setPrecision(const int & precision);

	// Create the actual plot. 
	void draw();

	/*
	Saves to file. fileName extension determines file type. Works with bmp
	always. May with with other formats depending on other installed
	extensions.

	If number is specified, it is appended to the fileName so that you can
	make out1.bmp, out2.bmp, out3.bmp, etc. easily.

	If digits is specified, it says how many digits to use to represent
	number.

	NOTE: You do *not* have to call draw() before calling save. You can save
	without ever actually drawing the plot
	*/
	void save(const std::string & fileName, int number = -1, int digits = 5);

	// Close the plot window without destroying the Plot object.
	void close();

	// Get the window object... be careful with this!
	CImgWindow& getWindow();

private:
	// All of the arguments that the user can set
	int winWidth;
	int winHeight;
	std::string title;
	Eigen::MatrixXd Y;
	Eigen::VectorXd x;
	std::vector<std::string> labels;
	std::string xLabel;
	std::string yLabel;
	bool legend;
	double xMin;
	double xMax;
	bool xRangeSet;	// Did the user set the range, or are we using automatic values?
	double xStep;
	bool xStepSet;	// Did the user set it, or are we using automatic values?
	double yMin;
	double yMax;
	bool yRangeSet;	// Did the user set the range, or are we using automatic values?
	double yStep;
	bool yStepSet;	// Did the user set it, or are we using automatic values?
	double alpha;
	bool markers;
	int markerSize;
	int precision;	// For labels - default = 4
	bool errorBars;
	bool clearPrevious; // If false, then we don't clear the screen
	Eigen::MatrixXd upperError;
	Eigen::MatrixXd lowerError;
	std::vector<Color> colors;

	// Padding around sides of plot area used for writing axis labels and title.
	const int leftPad = 50;
	const int rightPad = 30;
	const int topPad = 50;
	const int bottomPad = 50;

	// Pointer to the window object. We keep 
	// the window open until the user calls close() or deletes this object.
	CImgWindow w;
	std::string curWTitle;	// Track the current title - to know if we need to recreate the window object

							// Function that does the work of plotting. Loads the provided window * with the plot
							// will be used. draw() just draws this to a window while save() saves it
							// to a file.
	void makePlot();
	
	// Given an x,y coordinate in the plot, convert it to a pixel location.
	void toWindowCoords(const double x, const double y, int & xBuff, int & yBuff);
};