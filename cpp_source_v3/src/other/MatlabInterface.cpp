#include <stdafx.h>

using namespace std;
using namespace Eigen;

/*
Functions to create an interface with matlab, pass variables back and forth,
and make arbitrary matlab calls.

NOTE: This module is only currently set up to work in Windows.
*/

namespace MatlabEngine {

#ifdef _MATLAB_ENGINE
	Engine *g_matlabEngine;				// Intantiate the extern declared in MatlabInterface.h
#endif


/* If you call any plotting functions, you need to call this first */
	void initMatlab() {
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine)
			return;													// Already initialized!
		if (!(g_matlabEngine = engOpen("\0")))
			exit("initMatlab", "Can't start Matlab engine");	// Try to open the engine

#ifdef _SHOW_MATLAB_CONSOLE
		engSetVisible(g_matlabEngine, true);
#else
		engSetVisible(g_matlabEngine, false);
#endif

		matlab("clc; close all; clear all;");										// Clear everything
#endif
	}

	/* Execute the provided matlab command */
	void matlab(const char * command) {
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr) initMatlab();
		engEvalString(g_matlabEngine, command);
#endif
	}

	void matlab(const char * command, char * buff, int buffSize) {	// Stores console output of the matlab call in buff
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr) initMatlab();
		engOutputBuffer(g_matlabEngine, buff, buffSize);
		engEvalString(g_matlabEngine, command);
#endif
	}

	/* If you called initMatlab, then you should end with this to close the Matlab handle */
	void closeMatlab() {
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr) return;
		engEvalString(g_matlabEngine, "close all;");		// Close all of the open windows
		engEvalString(g_matlabEngine, "exit");				// Close the engine
		g_matlabEngine = nullptr;
#endif
	}

	/* char*  to matlab variable */
	void addVar(const char * string, const char * varName) {
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr) initMatlab();
		mxArray *matlabString = mxCreateString(string);			// Create a matlab string
		engPutVariable(g_matlabEngine, varName, matlabString);	// Place it into the matlab environment
		mxDestroyArray(matlabString);							// Free memory
#endif
	}

	/* Number to matlab variable */
	void addVar(const double & x, const char * varName) {
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr) initMatlab();
		mxArray *matlabMatrix = mxCreateDoubleMatrix(1, 1, mxREAL);		// Create a matlab matrix from the provided vector
		mxGetPr(matlabMatrix)[0] = double(x);							// Store x at the start of the matlab matrix
		engPutVariable(g_matlabEngine, varName, matlabMatrix);			// Add the matlab matrix to the matlab engine
		mxDestroyArray(matlabMatrix);									// Free memory
#endif
	}

	/*
	String list for labels in plots and the such. Example:
	char * stringList[] = {"first", "second", "third"};
	addVar(stringList, 3, "matlabVarName");
	*/
	void addVar(char ** strings, const int & numStrings, const char * varName)
	{
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr) initMatlab();				// Make sure that the engine handle is valid
		string command = (string)varName + (string)" = {";				// The beginning of the matlab command is "varName = {"
		for (int i = 0; i < numStrings; i++) {							// Loop over the variables and add them, so that we get a string like "varName = {'foo', 'bar', 'blat'}"
			command += (string)"'" + strings[i] + (string)"'";
			(i == numStrings - 1 ? command += "};" : command += ",");
		}
		matlab(command.c_str());										// Execute the command
#endif
	}

	// Get a vector or matrix from matlab
	void getVar(const char * varName, MatrixXf & buff) {
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr)
			initMatlab();
		mxArray * var = engGetVariable(g_matlabEngine, varName);
		if (var == NULL) {
			string s = (string)"Failed to getVar " + (string)varName + (string)" from matlab engine.";
			exit("MatlabInterface::getVar", s.c_str());
		}
		auto numDimensions = mxGetNumberOfDimensions(var);
		if (numDimensions != 2)
			exit("MatlabInterface::getVar", "getVar is currently not set up to receive objects from matlab with more than 2 dimensions.");
		const mwSize * dimSizes = mxGetDimensions(var);
		int debug1 = (int)(dimSizes[0]), debug2 = (int)(dimSizes[1]);
		buff.resize((int)dimSizes[0], (int)dimSizes[1]);	// Resize to the correct size.
		double * data = (double*)mxGetData(var);			// Get pointer to the actual data
		int counter = 0;
		for (int row = 0; row < (int)dimSizes[0]; row++) {
			for (int col = 0; col < (int)dimSizes[1]; col++) {
				buff(row, col) = (float)data[counter++];
			}
		}
		mxDestroyArray(var);
		return;
#endif
	}

	/* Hold on */
	void holdOn() {
#ifdef _MATLAB_ENGINE
		matlab("hold on;");
#endif
	}

	/* Hold off */
	void holdOff() {
#ifdef _MATLAB_ENGINE
		matlab("hold off;");
#endif
	}

	/* Plot a point */
	void plotPoint(double x, double y, double z, bool newFigure) {
#ifdef _MATLAB_ENGINE
		addVar(x, "temp1");
		addVar(y, "temp2");
		addVar(z, "temp3");
		if (newFigure) matlab("figure");
		matlab("plot3(temp1,temp2,temp3,'marker','o')");
#endif
	}

	void plot(std::function<double(double)> f, double min, double epsilon, double max, bool newFigure) {
		int numPoints = (int)((max - min) / epsilon);
		VectorXd X(numPoints), Y(numPoints);
		for (int i = 0; i < numPoints; i++) {
			X(i) = i*epsilon + min;
			Y(i) = f(X(i));
		}
		scatter(X, Y, newFigure);
	}

	/* Plot the provided function
	f = function that returns a double. (Input is Vector2d - if yours takes two floats, just wrap it with one that takes a Vector2d)
	The other numbers give the range of the function and the resolution with which points are sampled.
	*/
	void plot(std::function<double(const Vector2d &)> f,
		const double & minX1, const double & epsilonX1, const double & maxX1,
		const double & minX2, const double & epsilonX2, const double & maxX2,
		bool newFigure)
	{
		int numX1Values = (int)((maxX1 - minX1) / epsilonX1),
			numX2Values = (int)((maxX2 - minX2) / epsilonX2);
		VectorXd X1(numX1Values), X2(numX2Values);		// The coordinates of the points that we sample
		MatrixXd Z(numX2Values, numX1Values);
		for (int x2 = 0; x2 < numX2Values; x2++)
			X2(x2) = minX2 + x2*epsilonX2;
		for (int x1 = 0; x1 < numX1Values; x1++) {
			X1(x1) = minX1 + x1*epsilonX1;
			for (int x2 = 0; x2 < numX2Values; x2++) {
				Z(x2, x1) = f(Vector2d(X1(x1), X2(x2)));	// Compute the value for this point
			}
		}
		surf(X1, X2, Z, newFigure);						// Plot the surface
	}

	double f(const double & x)
	{
		return x*x;
	}

	double g(const VectorXd & x)
	{
		return x[0] * x[0] + x[1] * x[1] * x[0];
	}

	/* Plotting examples */
	void MatlabPlottingExamples()
	{
#ifdef _MATLAB_ENGINE
		initMatlab();

		mt19937_64 generator((unsigned int)time(NULL));
		uniform_real_distribution<double> distribution(0., 10.);
		uniform_real_distribution<double> distribution2(1, 100000);
		uniform_real_distribution<double> distribution3(0, 100);

		// Plot a vector.
		VectorXd v(10);
		for (int i = 0; i < 10; i++)
			v[i] = distribution(generator);
		plot(v);
		matlab("title('VectorXd')");
		semilogx(v);
		matlab("title('semilogx')");

		semilogy(v);
		matlab("title('semilogy')");

		loglog(v);
		matlab("title('loglog Plot')");

		// Plot a MatrixXd as many lines
		MatrixXd m(3, 3);
		int counter = 0;
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				m(r, c) = distribution2(generator);
		plot(m);
		matlab("title('MatrixXd')");
		matlab("legend('firstOne', 'le second', 'the last', 'Location', 'NorthWest')");

		loglog(m);
		matlab("title('MatrixXd loglog plot')");
		matlab("legend('firstOne', 'le second', 'the last', 'Location', 'NorthWest')");

		// Plot the matrix as a 3d bar
		plotMatrix(m); // samw as bar3
		matlab("title('Matrix as 3d bar plot');");

		// Surface from matrix
		surf(m);
		matlab("title('Matrix as surface');");

		VectorXd surfX(20), surfY(50);
		MatrixXd surfZ(50, 20);
		for (int i = 0; i < 20; i++)
			surfX[i] = -10 + i*.5;
		for (int i = 0; i < 50; i++)
			surfY[i] = -5 + i * 2;
		for (int i = 0; i < 20; i++)
			for (int j = 0; j < 50; j++)
				surfZ(j, i) = surfX[i] * surfY[j]; // The surface to plot
		surf(surfX, surfY, surfZ);
		matlab("title('Surface from X vector, Y vector, and resulting Z vector')");

		// Make a bar graph from the vector
		char * vectorBarNames[] = { "first", "second", "third", "fourth", "fifth", "sixth",
			"apple", "orange", "pear", "fruit", "ball", "cube" };
		bar(v, vectorBarNames);
		matlab("title('Bar of VectorXd');");

		// Make a bar graph from the matrix
		char * barNames[] = { "first", "second", "third" };
		bar(m, barNames);
		matlab("title('Bar of MatrixXd');");
		matlab("legend('foo', 'bar', 'blat');");

		// Make a pie graph from a vector
		char * pieNames[] = { "first", "second", "third", "fourth", "fifth", "sixth", "foo", "bar", "blat", "lastOne!" };
		pie(v, pieNames);

		// Scatter plot (2d)
		VectorXd v2(10);
		for (int i = 0; i < 10; i++)
			v2[i] = distribution3(generator);
		scatter(v, v2);
		matlab("title('Scatter Plot')");

		// Scatter plot (3d)
		VectorXd v3(10);
		for (int i = 0; i < 10; i++)
			v3[i] = distribution3(generator);
		scatter(v, v2, v3);
		matlab("title('Scatter Plot (3D)')");

		// Plot a 2D function
		std::function<double(double)> func = f;
		plot(func, -1, .01, 1);

		// Plot a 3D function
		VectorXd minBound = VectorXd::Ones(2)*-1.0,
			maxBound = VectorXd::Ones(2)*1.0,
			epsilon = VectorXd::Ones(2)*.1;
		std::function<double(const VectorXd & x)> func2 = g;
		plot(func2, -1, .1, 1, -2, .2, 2);

		cout << "Done. Press enter to close Matlab and continue." << endl;
		forceGetchar();
		closeMatlab();
#else
		exit("MatlabPlottingExamples", "You tried to run the Matlab plotting examples with _MATLAB_ENGINE set, which is not allowed.");
#endif
	}
} // End namespace Matlab
