#pragma once

#include <stdafx.h>

/*
Functions to create an interface with matlab, pass variables back and forth,
and make arbitrary matlab calls.

NOTE: This module is only currently set up to work in Windows.
*/

#ifdef _MATLAB_ENGINE
#include <engine.h>				// The matlab engine
#endif

namespace MatlabEngine {

#ifdef _MATLAB_ENGINE
	extern Engine *g_matlabEngine;
#endif

	/* Execute a command in the matlab engine */
	void matlab(const char * command);

	/* Initialize the matlab engine */
	void initMatlab();

	/* Execute the provided matlab command */
	void matlab(const char * command, char * buff, int buffSize);	// Stores console output of the matlab call in buff

	/* If you called initMatlab, then you should end with this to close the Matlab handle */
	void closeMatlab();

	/* char*  to matlab variable */
	void addVar(const char * string, const char * varName);

	/*
	double to matlab variable - If we templated this then it can't determine which version of addVar to use when you call with an Eigen VectorXd.
	Any other type will be cast to a double - so if you want to add an integer, just use this funciton,
	casting your int to a double.
	*/
	void addVar(const double & x, const char * varName);

	/* String list for labels in plots and the such. Example:
	char * stringList[] = {"first", "second", "third"};
	addVar(stringList, 3, "matlabVarName");
	*/
	void addVar(char ** strings, const int & numStrings, const char * varName);

	// Get a vector or matrix from matlab
	void getVar(const char * varName, Eigen::MatrixXd & buff);

	//////////////////////////////////////////////////
	///// Templated functions
	//////////////////////////////////////////////////
	/* Add an Eigen matrix */
	template <typename Derived>
	void addVar(const Eigen::MatrixBase<Derived> & m, const char * varName)
	{
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr)
			initMatlab();

		// Create a matlab array from the provided vector
		mxArray * matlabMatrix = mxCreateDoubleMatrix(m.rows(), m.cols(), mxREAL);
		double * matlabMatrixPtr = mxGetPr(matlabMatrix);
		int counter = 0;
		for (int c = 0; c < m.cols(); c++)
			for (int r = 0; r < m.rows(); r++)
				matlabMatrixPtr[counter++] = (double)m(r, c);

		// Place v into the matlab environment
		engPutVariable(g_matlabEngine, varName, matlabMatrix);

		// Free memory
		mxDestroyArray(matlabMatrix);
#endif
	}

	// Get a scalar
	template <typename T>
	T getVar(const char * varName) {
#ifdef _MATLAB_ENGINE
		if (g_matlabEngine == nullptr)
			initMatlab();
		mxArray * var = engGetVariable(g_matlabEngine, varName);
		if (var == NULL) {
			string s = (string)"Failed to getVar " + (string)varName + (string)" from matlab engine.";
			errorExit(s.c_str());
		}
		auto numDimensions = mxGetNumberOfDimensions(var);
		if (numDimensions != 2)
			errorExit("getVar is currently not set up to receive objects from matlab with more than 2 dimensions.");
		const mwSize * dimSizes = mxGetDimensions(var);
		int debug1 = (int)(dimSizes[0]), debug2 = (int)(dimSizes[1]);
		if ((debug1 != 1) || (debug2 != 1))
			errorExit("The scalar version of getVar was called on a non-scalar matlab object.");
		T result = (T)(((double*)mxGetData(var))[0]);
		mxDestroyArray(var);
		return result;
#endif
	}

	//////////////////////////////////////////////////
	///// Plotting Functions
	//////////////////////////////////////////////////

	/* Hold on */
	void holdOn();

	/* Hold off */
	void holdOff();

	/* Plot a VectorXd as a line or a MatrixXd as several lines */
	template <typename Derived>
	void plot(const Eigen::MatrixBase<Derived> & m, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("handle=plot(tempMatrix);");
#endif
	}

	/* Plot a VectorXd as a line or a MatrixXd as several lines, with error bars */
	template <typename Derived>
	void errorbar(const Eigen::MatrixBase<Derived> & m, const Eigen::MatrixBase<Derived> & e, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		addVar(e, "tempMatrix2");
		if (newFigure) matlab("figure");
		matlab("handle=errorbar(tempMatrix,tempMatrix2);");
#endif
	}

	/* vector X vs vector Y */
	template <typename Derived>
	void plot(const Eigen::MatrixBase<Derived> & x, const Eigen::MatrixBase<Derived> & y, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(x, "tempX");
		addVar(y, "tempY");
		if (newFigure) matlab("figure");
		matlab("handle=plot(tempX,tempY);");
#endif
	}

	/* plot matrix as surface.
	X,Y vectors, Z matrix. Z should have rows = Y.length and cols = X.length
	Think of it as Z(row,col) = Z(y-index,x-index). */
	template <typename Derived1, typename Derived2, typename Derived3>
	void surf(const Eigen::MatrixBase<Derived1> & X, const Eigen::MatrixBase<Derived2> & Y, const Eigen::MatrixBase<Derived3> & Z, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(X, "tempX");
		addVar(Y, "tempY");
		addVar(Z, "tempZ");
		if (newFigure) matlab("figure");
		matlab("handle=surf(tempX,tempY,tempZ);");
#endif
	}

	/* plot matrix as surface */
	template <typename Derived>
	void surf(const Eigen::MatrixBase<Derived> & m, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("handle=surf(tempMatrix);");
#endif
	}

	/* 3D bar plot */
	template <typename Derived>
	void bar3(const Eigen::MatrixBase<Derived> & m, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("handle=bar3(tempMatrix);");
#endif
	}

	/* Histogram */
	template <typename Derived>
	void hist(const Eigen::MatrixBase<Derived> & m, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("hist(tempMatrix);");
#endif
	}

	/* Plot a point */
	void plotPoint(double x, double y, double z, bool newFigure = true);

	/* plot matrix as surface (same as bar3(m), but color based on height) )*/
	template <typename Derived>
	void plotMatrix(const Eigen::MatrixBase<Derived> & m, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("handle = bar3(tempMatrix); H = handle; numBars = size(tempMatrix,1); numSets = size(tempMatrix,2); for i = 1:numSets; zdata = ones(6*numBars,4); k = 1; for j = 0:6:(6*numBars-6); zdata(j+1:j+6,:) = tempMatrix(k,i); k = k+1; end; set(H(i),'Cdata',zdata); end;");
#endif
	}

	/* Scatter plot of vectors X,Y*/
	template <typename Derived>
	void scatter(const Eigen::MatrixBase<Derived> & X, const Eigen::MatrixBase<Derived>& Y, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(X, "tempVectorX");
		addVar(Y, "tempVectorY");
		if (newFigure) matlab("figure");
		matlab("handle=scatter(tempVectorX,tempVectorY);");
#endif
	}

	/* Scatter plot (3D), of vectors X,Y,Z */
	template <typename Derived>
	void scatter(const Eigen::MatrixBase<Derived> & X, const Eigen::MatrixBase<Derived> & Y, const Eigen::MatrixBase<Derived> & Z, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(X, "tempVectorX");
		addVar(Y, "tempVectorY");
		addVar(Z, "tempVectorZ");
		if (newFigure) matlab("figure");
		matlab("handle=scatter3(tempVectorX,tempVectorY,tempVectorZ);");
#endif
	}

	/* Scatter plot (3D), of vectors X,Y,Z */
	template <typename Derived>
	void scatter3(const Eigen::MatrixBase<Derived> & X, const Eigen::MatrixBase<Derived> & Y, const Eigen::MatrixBase<Derived> & Z, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		scatter(X, Y, Z, newFigure);
#endif
	}

	/* Plot with logarithmic x axis */
	template <typename Derived>
	void semilogx(const Eigen::MatrixBase<Derived> & m, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("handle=semilogx(tempMatrix);");
#endif
	}

	/* Plot with logarithmic y axis */
	template <typename Derived>
	void semilogy(const Eigen::MatrixBase<Derived> & m, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("handle=semilogy(tempMatrix);");
#endif
	}

	/* Plot with logarithmic x and y axes */
	template <typename Derived>
	void loglog(const Eigen::MatrixBase<Derived> & m, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("handle=loglog(tempMatrix);");
#endif
	}

	/* Bar graph (can give a matrix or a vector) */
	template <typename Derived>
	void bar(const Eigen::MatrixBase<Derived> & m, char ** names = nullptr, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(m, "tempMatrix");
		if (newFigure) matlab("figure");
		matlab("handle=bar(tempMatrix);");
		if (names) {
			addVar(names, (int)(m.cols() == 1 ? m.rows() : m.cols()), "tempList");
			matlab("set(gca,'XTickLabel',tempList)");
		}
#endif
	}

	/* pie graph (from a vector, v). If you pass names, it must contain the same number of
	elements as v. These strings will be used to name each slice of the pie chart.*/
	template <typename Derived>
	void pie(const Eigen::MatrixBase<Derived> & v, char ** names = nullptr, bool newFigure = true) {
#ifdef _MATLAB_ENGINE
		addVar(v, "tempMatrix");
		if (newFigure) matlab("figure");
		if (names) {
			addVar(names, (int)v.size(), "tempList");
			matlab("handle=pie(tempMatrix, tempList);");
		}
		else matlab("handle=pie(tempMatrix);");
#endif
	}

	/* Plot the provided function
	f = function that returns a double.
	Points are sampled from min to max in epsilon intervals
	*/
	void plot(std::function<double(double)> f, double min, double epsilon, double max, bool newFigure = true);

	/* Plot the provided function
	f = function that returns a double.
	The other numbers give the range of the function and the resolution with which points are sampled.
	*/
	void plot(std::function<double(const Eigen::Vector2d &)> f,
		const double & minX1, const double & epsilonX1, const double & maxX1,
		const double & minX2, const double & epsilonX2, const double & maxX2,
		bool newFigure = true);

	/* Plotting examples */
	void MatlabPlottingExamples();
} // End namespace Matlab