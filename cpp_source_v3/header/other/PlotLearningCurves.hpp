#pragma once

#include <stdafx.h>

// Plotting function useful for RL experiments
void PlotLearningCurves(
	const std::vector<Eigen::MatrixXd> & returns, // [numAlgorithms][numTrials][maxEps]
	const std::vector<std::string> & algNames,
	const char * title,	// nullptr to have no title
	double minY = 1,	// If not provided, uses matlab defaults
	double maxY = -1,
	bool stdErrorNotDev = true, // If true uses standard error. If false, uses standard deviation
	bool logX = false,
	bool logY = false,
	Eigen::VectorXi episodeNumbers = Eigen::VectorXi(), // If not provided, assumes 1,2,...
	bool newFigure = true,
	bool writeToCSV = true);