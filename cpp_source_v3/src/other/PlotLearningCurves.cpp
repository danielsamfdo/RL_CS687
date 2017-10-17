#include <stdafx.h>

using namespace Eigen;
using namespace std;

/*
Plotting function useful for RL experiments
*/
void PlotLearningCurves(
	const vector<MatrixXd> & returns, // [numAlgorithms][numTrials][maxEps]
	const vector<string> & algNames,
	const char * title,	// nullptr to have no title
	double minY,
	double maxY,
	bool stdErrorNotDev, // If true uses standard error. If false, uses standard deviation
	bool logX,
	bool logY,
	VectorXi episodeNumbers, // If not provided, assumes 1,2,...
	bool newFigure,
	bool writeToCSV)
{
	// Get matrix and vector dimensions
	int numAlgs = (int)algNames.size(),
		maxEps = (int)returns[0].cols(); // .rows() = the number of trials run

										 // Convert to matrix of means and stddevs (or stderrs)
	MatrixXd meanReturns(numAlgs, maxEps), errorbarValues(numAlgs, maxEps);
	for (int i = 0; i < numAlgs; i++)
	{
		for (int j = 0; j < maxEps; j++)
		{
			meanReturns(i, j) = returns[i].col(j).mean();
			if (stdErrorNotDev)
				errorbarValues(i, j) = returns[i].col(j).stderror();
			else
				errorbarValues(i, j) = returns[i].col(j).stddev();
		}
	}

	// Load episodeNumbers if not provided
	if (episodeNumbers.size() == 0)
	{
		episodeNumbers.resize(maxEps);
		for (int i = 0; i < maxEps; i++)
			episodeNumbers[i] = i + 1;
	}

#ifdef _MATLAB_ENGINE
	// Add matrices to matlab and names array
	MatlabEngine::addVar(meanReturns, "meanReturns");
	MatlabEngine::addVar(errorbarValues, "errorbars");
	MatlabEngine::addVar(episodeNumbers, "episodeNumbers");
	string namesStr = "names = {";
	for (int i = 0; i < numAlgs; i++)
	{
		namesStr += "'" + algNames[i] + "'";
		if (i != numAlgs - 1)
			namesStr += ", ";
	}
	namesStr += "};";
	MatlabEngine::matlab(namesStr.c_str());

	// Make the actual plot call
	if (newFigure) MatlabEngine::matlab("figure");
	MatlabEngine::matlab("LearningCurve(meanReturns, errorbars, names, episodeNumbers);");
	if (minY < maxY)
	{
		string setYLimString = "ylim([" + to_string(minY) + ", " + to_string(maxY) + "]);";
		MatlabEngine::matlab(setYLimString.c_str());
	}
	if (title != nullptr)
		MatlabEngine::matlab(("title('" + (string)(title)+(string)"');").c_str());
	if (logX)
		MatlabEngine::matlab("set(gca, 'XScale', 'log');");
	if (logY)
		MatlabEngine::matlab("set(gca, 'YScale', 'log');");
#else
	CImgPlot p;
	VectorXd episodeNumbers_float(episodeNumbers.size());
	for (int i = 0; i < (int)episodeNumbers.size(); i++)
		episodeNumbers_float[i] = (double)episodeNumbers[i];
	p.setX(episodeNumbers_float);
	p.setY(meanReturns);
	p.setErrors(errorbarValues, errorbarValues);
	string labels = "";
	for (int i = 0; i < (int)algNames.size(); i++)
	{
		labels = labels + algNames[i];
		if (i != (int)algNames.size() - 1)
			labels = labels + (string)",";
	}
	p.setLabels(labels);
	p.setErrorBars(true);
	p.setXLabel("Episodes");
	p.setYLabel("Expected Return");
	p.setTitle(title);
	if (minY < maxY)
		p.setYRange(minY, maxY);
	string plotFileName = "/Users/danielsampetethiyagu/github/RL_CS687/cpp_source_v3/build/Clion/output/" + (string)title + ".bmp";
	p.save(plotFileName);
#ifdef _DISPLAY_GRAPHICS
	p.draw();
#endif
	if (!p.getWindow().isClosed())
	{
		cout << "Close the plot to continue..." << endl;
		while (true)
		{
			if (p.getWindow().isClosed())
				break;
			pause(10);
		}
		cout << "Plot closed." << endl;
	}
#endif

	if (writeToCSV)
	{
		// Write to file
		string fileName = "/Users/danielsampetethiyagu/github/RL_CS687/cpp_source_v3/build/Clion/output/" + (string)title + ".csv";
		ofstream out(fileName.c_str());
		// Print header line
		out << "Episode" << ',';
		for (int i = 0; i < (int)algNames.size(); i++)
			out << algNames[i] << ',';
		for (int i = 0; i < (int)algNames.size(); i++)
		{
			out << algNames[i] << "_ErrorBar";
			if (i != (int)algNames.size() - 1)
				out << ',';
		}
		out << endl;
		// Print data
		for (int i = 0; i < (int)meanReturns.cols(); i++)
		{
			out << episodeNumbers[i] << ',';
			for (int j = 0; j < (int)meanReturns.rows(); j++)
				out << meanReturns(j, i) << ',';
			for (int j = 0; j < (int)meanReturns.rows(); j++)
			{
				out << errorbarValues(j, i);
				if (j != (int)meanReturns.rows() - 1)
					out << ',';
			}
			out << endl;
		}
		out.close();
	}
}