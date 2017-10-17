#include <stdafx.h>
#include <experimental/CMPSCI687/HW1.hpp>
#include <environments/Gridworld687.hpp>
#include <environments/Pendulum.hpp>
#include <agents/CrossEntropyMethod.hpp>

using namespace std;
using namespace Eigen;

void HW1()
{
	// RNG
	mt19937_64 generator(0);

	// Set maxThreads
	int maxThreads = 32;

	// Variables we will use
	int numTrials, population, numElite, episodesPerPolicy, maxEps, maxT;
	double gamma, sigma;
	vector<int> epsToDraw;

	// Where we store results
	vector<MatrixXd> returns;
	vector<string> algNames;

	while (true)
	{
		Environment * e = nullptr;

		// Ask which environment to run on
		switch (getSelection("environment", 4, "Gridworld-687", "Pendulum (deterministic start)", "Pendulum (stochastic start)", "exit"))
		{
		case 0: // Gridworld
			e = new Gridworld687;
			break;
		case 1: // Pendulum (deterministic start)
			e = new Pendulum(generator, true);
			break;
		case 2: // Pendulum (stochastic start)
			e = new Pendulum(generator, false);
			break;
		default: // exit
			exit(0);
		}

		// Ask for number of trials and hyperparameter values
		cout << "Enter number of trials (integer): ";
		cin >> numTrials;

		cout << "Enter population (CEM Hyperparameter, integer): ";
		cin >> population;

		cout << "Enter numElite (CEM Hyperparameter, integer): ";
		cin >> numElite;

		cout << "Enter episodesPerPolicy (CEM Hyperparameter, integer): ";
		cin >> episodesPerPolicy;

		cout << "Enter sigma (CEM Hyperparameter, real): ";
		cin >> sigma;

		// Get environment parameters
		Environment::Description envDesc = e->getDesc();
		maxEps = envDesc.suggestedMaxEps;
		maxT = envDesc.suggestedMaxT;
		gamma = envDesc.gamma;

		// Ask which episodes to draw
		epsToDraw.resize(0);
		if (yesNo("Draw some episodes?", cin, cout))
		{
			cout << "Episode numbers should be between 0 and " << maxEps - 1 << "." << endl;
			while (true)
			{
				int ep;
				cout << "Enter episode number to draw (-1 to stop): ";
				cin >> ep;
				if (ep == -1)
					break;
				epsToDraw.push_back(ep);
			}
		}

		// Create agent
		CrossEntropyMethod a(e->getInitialPolicy(), population, episodesPerPolicy, numElite, sigma, gamma, generator);

		// Get the results
		returns.resize(0);
		algNames.resize(0);
		cout << "Running experiment..." << endl;
		returns.push_back(runAgentEnvironment(&a, e, numTrials, maxEps, maxT, generator, maxThreads, epsToDraw));
		algNames.push_back("Cross-Entropy Method");

		// Plot
		cout << "Experiment complete. Plotting..." << endl;
		PlotLearningCurves(returns, algNames, e->getName(), envDesc.suggestedPlotMinPerformance, envDesc.suggestedPlotMaxPerformance);

		cout << "Done!" << endl << endl;
		cin.ignore(cin.rdbuf()->in_avail()); // Flush cin

		// Clean up memory
		delete e;
	}
}