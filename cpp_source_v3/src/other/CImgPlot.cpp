#include <stdafx.h>

// Create a blank plot object - it is up to the user to specify values.
CImgPlot::CImgPlot()
{
	winWidth = 700;
	winHeight = 700;
	title = "Plot";
	Y.resize(0, 0);
	x.resize(0);
	labels.resize(0);
	xLabel = "x-axis";
	yLabel = "y-axis";
	legend = true;
	xMin = 0;
	xMax = 1.0;
	xRangeSet = false;
	xStep = 0.1;
	xStepSet = false;
	yMin = 0;
	yMax = 1.0;
	yRangeSet = false;
	yStep = 0.1;
	yStepSet = false;
	alpha = 1.0;
	markers = true;
	markerSize = 3;
	errorBars = false;
	clearPrevious = true;
	upperError.resize(0, 0);
	lowerError.resize(0, 0);
	colors.resize(10);
	colors[0] = RED_CRIMSON;
	colors[1] = GREEN_DARK;
	colors[2] = BLUE_ZAFFRE;
	colors[3] = CYAN_TEAL;
	colors[4] = PURPLE_INDIGO;
	colors[5] = BLACK;
	colors[6] = GRAY;
	colors[7] = ORANGE_RED;
	colors[8] = PINK_DEEP;
	colors[9] = BROWN_SADDLE;
	precision = -1;
}

// Clean up memory
CImgPlot::~CImgPlot()
{
	// Delete the window object if it was created
	close();
}

void CImgPlot::setWindowSize(const int width, const int height) {
	winWidth = width;
	winHeight = height;
}

void CImgPlot::setTitle(const std::string & title) {
	this->title = title;
}

// Each row is a different line to plot
void CImgPlot::setY(const Eigen::MatrixXd & Y) {
	this->Y = Y;
}

// If the x-positions are not 1,...,Y.cols(), they are set here
void CImgPlot::setX(const Eigen::VectorXd & x) {
	this->x = x;
}

// Comma-separated labels for each data series
void CImgPlot::setLabels(const std::string & s) {
	legend = true;
	labels.resize(0);
	std::istringstream ss(s);
	std::string token;
	while (getline(ss, token, ','))
		labels.push_back(token);
}

// Turn legend on or off
void CImgPlot::setLegend(bool on) {
	legend = on;
}

// Set x-axis range
void CImgPlot::setXRange(const double min, const double max) {
	xMin = min;
	xMax = max;
	xRangeSet = true;
}

// Set step of x-axis marks
void CImgPlot::setXStep(const double step) {
	xStep = step;
	xStepSet = true;
}

// Set y-axis range
void CImgPlot::setYRange(const double min, const double max) {
	yMin = min;
	yMax = max;
	yRangeSet = true;
}

// Set step of y-axis marks
void CImgPlot::setYStep(const double step) {
	yStep = step;
	yStepSet = true;
}

// Turn markers on or off, and set their size and fill
void CImgPlot::setMarkers(bool on, int size) {
	markers = on;
	markerSize = size;
}

// Turn on or off error bars?
void CImgPlot::setErrorBars(bool on) {
	errorBars = on;
}

void CImgPlot::setClearPrevious(bool newValue) {
	clearPrevious = newValue;
}

// Set error bar ranges (does not draw them unless you toggle errorBars(true)
void CImgPlot::setError(const Eigen::MatrixXd & range) {
	errorBars = true;
	upperError = range;
	lowerError = range;
}

// Set error bar ranges (does not draw them unless you toggle errorBars(true)
void CImgPlot::setErrors(const Eigen::MatrixXd & upper, const Eigen::MatrixXd & lower) {
	errorBars = true;
	upperError = upper;
	lowerError = lower;
}

// Make curves transparent?
void CImgPlot::setTransparency(const double alpha) {
	this->alpha = alpha;
}

void CImgPlot::setColors(std::vector<Color> colors) {
	this->colors = colors;
}

void CImgPlot::setXLabel(const std::string & s) {
	xLabel = s;
}

void CImgPlot::setYLabel(const std::string & s) {
	yLabel = s;
}

void CImgPlot::setPrecision(const int & precision) {
	this->precision = precision;
}

// Create the actual plot. 
void CImgPlot::draw() {
	makePlot();	// Remake the plot in w (if already allocated and the right size)
	w.update();	// Actually draw it
}

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
void CImgPlot::save(const std::string & fileName, int number, int digits) {
	// Use a new window object so that the drawn plot can stay open.
	makePlot();
	w.save(fileName, number, digits);
}

// Close the plot window without destroying the Plot object.
void CImgPlot::close() {
	w.close();
}

// Get the window object... be careful with this!
CImgWindow& CImgPlot::getWindow() {
	return w;
}

void CImgPlot::makePlot() {
	// Create the window object (not in w)
	w.open(winWidth, winHeight, title, 100, 100);
	curWTitle = title;

	if ((Y.rows() == 0) | (Y.cols() < 2))
		exit("Plot::makePlot", "Not enough points to plot");

	// If x is not initialized, set it to default values
	if (x.size() == 0)
	{
		x.resize(Y.cols());
		for (int i = 0; i < (int)x.size(); i++)
			x[i] = i;
	}

	// Get the min and max values of Y and x.
	// If log, then ignore values < 0.
	double minXCoeff = x.minCoeff(),
		maxXCoeff = x.maxCoeff(),
		minYCoeff = Y.minCoeff(),
		maxYCoeff = Y.maxCoeff();

	// If minX,maxX or minY,maxY are not set, set them
	if (!xRangeSet)
	{
		xMin = minXCoeff;
		xMax = maxXCoeff;
	}
	if (!yRangeSet)
	{
		yMin = minYCoeff;
		yMax = maxYCoeff;
	}

	// If precisions not set, set it
	if (precision == -1)
		precision = 4;

	// If ranges were set, make sure they're valid
	if (((xRangeSet) && (xMin > xMax)) ||
		((yRangeSet) && (yMin > yMax)))
		exit("Plot::makePlot", "x or y ranges invalid for log plots");

	// Make sure the step sizes are set
	if (!xStepSet)
		xStep = (xMax - xMin) / 10.0;
	if (!yStepSet)
		yStep = (yMax - yMin) / 10.0;

	// Get the tick mark locations and "names" (the numbers printed on them).
	// These might be different if using a log scale.
	std::vector<double> xTicks(0), yTicks(0);
	double curX = xMin;
	while (true)
	{
		xTicks.push_back(curX);
		curX += xStep;

		if (curX >= xMax + .001*(xMax - xMin))
			break;
	}

	double curY = yMin;
	while (true)
	{
		yTicks.push_back(curY);
		curY += yStep;

		if (curY >= yMax + .001*(yMax - yMin))
			break;
	}

	// Loop over the different curves and draw them.
	for (int i = 0; i < (int)Y.rows(); i++)
	{
		for (int j = 1; j < (int)Y.cols(); j++)
		{
			double x0 = x[j - 1], y0 = Y(i, j - 1), x1 = x[j], y1 = Y(i, j);
			int tx0, ty0, tx1, ty1;
			toWindowCoords(x0, y0, tx0, ty0);
			toWindowCoords(x1, y1, tx1, ty1);

			w.drawLine(tx0, ty0, tx1, ty1, colors[i % (int)colors.size()], alpha);

			if (markers)
			{
				// Draw the first point
				if (j == 1)
					w.drawCircle(
						tx0,
						ty0,
						markerSize,
						colors[i % (int)colors.size()],
						false,
						alpha);
				// Draw the current point
				w.drawCircle(
					tx1,
					ty1,
					markerSize,
					colors[i % (int)colors.size()],
					false,
					alpha);
			}
			if (errorBars)
			{
				if (j == 1)
				{
					// Need to draw the first one too
					double l = y0 - lowerError(i, j - 1),
						u = y0 + upperError(i, j - 1);
					int tl, tu;
					toWindowCoords(x0, l, tx0, tl);
					toWindowCoords(x0, u, tx0, tu);
					// Draw the bar
					w.drawLine(
						tx0, tl, tx0, tu,
						colors[i % (int)colors.size()], alpha / 4.0);
				}
				double l = y1 - lowerError(i, j),
					u = y1 + upperError(i, j);
				int tl, tu;
				toWindowCoords(x1, l, tx1, tl);
				toWindowCoords(x1, u, tx1, tu);
				// Draw the bar
				w.drawLine(
					tx1, tl, tx1, tu,
					colors[i % (int)colors.size()], alpha / 4.0);
			}
		}
	}

	// Redraw the buffer-boxes so that if lines went over where axis
	w.drawRectangle(0, 0, winWidth - 1, bottomPad, WHITE, true);
	w.drawRectangle(0, winHeight - 1, winWidth - 1, winHeight - bottomPad - 1, WHITE, true);
	w.drawRectangle(0, 0, leftPad, winHeight - 1, WHITE, true);
	w.drawRectangle(winWidth - 1, winHeight - 1, winWidth - rightPad - 1, 0, WHITE, true);

	// Draw bounding box
	// Left wall
	w.drawLine(leftPad, bottomPad,
		leftPad, winHeight - topPad - 1,
		GRAY_LIGHT);
	// Right wall
	w.drawLine(winWidth - rightPad - 1, bottomPad,
		winWidth - rightPad - 1, winHeight - topPad - 1,
		GRAY_LIGHT);
	// Top wall
	w.drawLine(leftPad, winHeight - topPad - 1,
		winWidth - rightPad - 1, winHeight - topPad - 1,
		GRAY_LIGHT);
	// Bottom pad
	w.drawLine(leftPad, bottomPad,
		winWidth - rightPad - 1, bottomPad,
		GRAY_LIGHT);

	// Write axis labels
	w.drawText(winWidth / 2, 20, xLabel.c_str(), BLACK);
	w.drawText(leftPad - 5, winHeight - topPad + 25, yLabel.c_str(), BLACK);

	// Draw the axes with tick marks
	for (int i = 0; i < (int)xTicks.size(); i++)
	{
		double x = xTicks[i];
		int y0 = bottomPad - 5,
			y1 = bottomPad + 5,
			tx, trash;
		// We only care about the x-coord
		toWindowCoords(x, y0, tx, trash);
		w.drawLine(tx, y0, tx, y1, GRAY_LIGHT);
		w.drawText(tx - 5, bottomPad - 6, to_string_with_precision(x, precision));
	}
	for (int i = 0; i < (int)yTicks.size(); i++)
	{
		double y = yTicks[i];
		int x0 = leftPad - 5,
			x1 = leftPad + 5,
			ty, trash;
		// We only care about the y-coord
		toWindowCoords(x0, y, trash, ty);
		w.drawLine(x0, ty, x1, ty, GRAY_LIGHT);
		w.drawText(2, ty + 8, to_string_with_precision(y, precision));
	}

	// Draw the title
	w.drawText(winWidth / 2, winHeight - 2, title);

	// Create labels
	while ((int)labels.size() < (int)Y.rows())
		labels.push_back("Series " + std::to_string(labels.size() + 1));

	// Make a box for the legend
	int legendHeightPerLine = 15;
	int maxLabelLen = 0;
	for (int i = 0; i < (int)Y.rows(); i++)
		maxLabelLen = std::max<int>(maxLabelLen, (int)labels[i].length());
	int legendWidth = 60 + maxLabelLen * 6,
		legendX = winWidth - rightPad - legendWidth,
		legendTopY = winWidth / 2,// winHeight - topPad,
		legendHeight = legendHeightPerLine * (int)Y.rows();
	int legendBorderWidth = 3;
	w.drawRectangle(
		legendX - legendBorderWidth,
		legendTopY - legendHeight - legendBorderWidth,
		legendX + legendWidth + legendBorderWidth,
		legendTopY + legendBorderWidth,
		BLACK, true, 0.5);
	w.drawRectangle(
		legendX,
		legendTopY - legendHeight,
		legendX + legendWidth,
		legendTopY,
		WHITE, true, 1);
	for (int i = 0; i < (int)Y.rows(); i++)
	{
		int y = legendTopY - legendHeightPerLine*i - 2 - legendHeightPerLine / 2;
		// Draw example line
		int exampleWidth = 50;
		w.drawLine(
			legendX + 2,
			y,
			legendX + exampleWidth + 2,
			y,
			colors[i],
			alpha);
		if (markers)
		{
			w.drawCircle(
				(legendX + exampleWidth + 2 + legendX + 2) / 2,
				y,
				3,
				colors[i],
				true, alpha);
		}
		// Draw label for this series
		w.drawText(
			legendX + exampleWidth + 2 + 5,
			legendTopY - legendHeightPerLine*i - 2,
			labels[i]);
	}
}

// Given an x,y coordinate in the plot, convert it to a pixel location.
void CImgPlot::toWindowCoords(const double x, const double y, int & xBuff, int & yBuff) {
	double xScale = (double)(winWidth - leftPad - rightPad) / (xMax - xMin);
	xBuff = (int)(leftPad + (x - xMin)*xScale);
	double yScale = (double)(winHeight - topPad - bottomPad) / (yMax - yMin);
	yBuff = (int)(bottomPad + (y - yMin)*yScale);
}