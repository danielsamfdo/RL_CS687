#pragma once

// Values for the user to set when compiling:
//
// _DISPLAY_GRAPHICS -	If true, then can create graphics windows using CImg
//						If false, then GUI never created, but can still save
//						plots to file using CImg or Window classes.
//
// _USE_MULTITHREADING - If false, then code should be single-threaded
//
// _MATLAB_ENGINE	 -  If true, then the project should be linked with the
//					    Matlab engine, allowing for calls to Matlab. If not
//					    defined, all Matlab calls do nothing. The Matlab engine
//						should only be used for visualizations, not computation.
//
// _CLOSE_MATLAB_ENGINE_ON_TERMINATE - If true, then you should close the matlab
//						engine when the program ends, otherwise leave it open
//						so that it doesn't have to be re-opened for the next run.
//
// _SHOW_MATLAB_CONSOLE - Make the matlab console visible, or hide it after it
//						has been opened?
//
// 

#define _MAX_THREADS 34		// The number of threads is always min-ed with _MAX_THREADS

// Define the suggested window widths and heights
#define _SUGGESTED_WIN_WIDTH 1024
#define _SUGGESTED_WIN_HEIGHT 768


// All of the standard defines that we use
#define _USE_MATH_DEFINES	// Have math.h include constants like M_PI

// All of the standard C++ includes that we use
#include <iostream>			// Console I/O
#include <fstream>			// File I/O
#include <string>
#include <vector>
#include <math.h>
#include <random>
#include <assert.h>
#include <stdarg.h>			// For variadic functions
#include <iomanip>
#include <sstream>
#include <limits.h>
#include <time.h>

// All of the extra libraries that we use
#ifdef EIGEN_MATRIXBASE_H
#error Do not include Eigen - stdafx.h will include Eigen with extensions.
#endif
#define EIGEN_MATRIXBASE_PLUGIN <other/EigenExtensions.hpp>
#include <Eigen/Dense>

// CImg has to come after Eigen or Eigen will throw an error due to
// strange #defines in -X11. The error says something like:
// "The preprocessor symbol 'Success' is defined"
#ifndef _DISPLAY_GRAPHICS
#define cimg_display 0 // With this set, no third party libraries are needed
#endif
#include <CImg.h>

// Files to include
#include <other/MathUtils.hpp>
#include <other/IOUtils.hpp>
#include <other/MarsagliaGenerator.hpp>
#include <other/Timer.hpp>
#include <other/mvnpdf.hpp>
#include <other/mvnrnd.hpp>
#include <other/CMAES.hpp>
#include <other/CImgWindow.hpp>
#include <other/CImgPlot.hpp>
#include <other/GradientDescent.hpp>
#include <other/tinv.hpp>
#include <other/MatlabInterface.hpp>
#include <other/Thing.hpp>
#include <other/History.hpp>
#include <other/PlotLearningCurves.hpp>

// Include unit tests for the code in "other" folders
#include <other/TestOther.hpp>

// Common RL Files
#include <policies/Policy.hpp>
#include <environments/Environment.hpp>
#include <agents/Agent.hpp>
#include <agents/BatchAgent.hpp>

// Common experimental files
#include <experimental/basicExperiments.hpp>