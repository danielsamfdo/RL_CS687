#include <stdafx.h>

using namespace std;
using namespace Eigen;

// This test objective is static so that it is only visible within this file.
static double objective_function_test_CMAES(const VectorXd & x, void * data[], mt19937_64 & gen)
{
	return x.dot(x);
}

void runAllTests() {
	testMatlabEngine();
	testCMAES();
	testEigenExtensions();
	testGradientDescent();
}

// Simple test of CMAES
void testCMAES()
{
	cout << "Beginning test of CMA-ES. This may take a while." << endl;
	VectorXd initialMean(3);
	initialMean.setConstant(1);
	double initialSigma = 1;
	int numIterations = 5000;
	VectorXd cmaesResult = CMAES(
		initialMean,
		initialSigma,
		numIterations,
		objective_function_test_CMAES,
		NULL,
		true,
		rand(),
		2);

	cout << "CMA-ES found minimum of x.dot(x) at = " << cmaesResult.transpose()
		<< endl << "Press enter to continue." << endl;

	// Force a getchar
	forceGetchar();
}

// Test Eigen extensions
void testEigenExtensions() 
{
	// Test loading with random numbers
	cout << "Beginning test of Eigen extensions." << endl;
	VectorXd v(5);
	mt19937 generator;
	v.uniformRealRand(0.0, 1.0, generator);
	cout << "Random vector: " << v.transpose() << endl;

	v.uniformIntRand(1, 3, generator);
	cout << "Random int vector: " << v.transpose() << endl;

	// Test sorting
	cout << "Sorted (ascending): " << v.sort().transpose() << endl;
	cout << "Unsorted again: " << v.transpose() << endl;
	cout << "Sorted (descending): " << v.sort(false).transpose() << endl;
	cout << "Unsorted again: " << v.transpose() << endl;
	v.sortInPlace();
	cout << "Sorted (ascending): " << v.transpose() << endl;
	v.sortInPlace(false);
	cout << "Sorted (descending): " << v.transpose() << endl;

	VectorXi w(5);
	w.uniformIntRand(1, 3, generator);
	cout << "New random int vector: " << w.transpose() << endl;

	// Test isFinite
	cout << "The following should be 1: " << v.isFinite() << endl;
	v.setConstant(NAN);
	cout << "The following should be nan: " << v.transpose() << endl;
	cout << "The following should be 0: " << v.isFinite() << endl;
	v.setConstant(1);
	v = v*pow(100, 1000);
	cout << "This following should be INF: " << v.transpose() << endl;
	cout << "The following should be 0: " << v.isFinite() << endl;

	cout << "Eigen extension tests complete. Hit enter to continue." << endl;
	forceGetchar();
}

double testGradientDescent_f(const VectorXd & x, void* d[]) {
	return 0.5*x.dot(x);
}
VectorXd testGradientDescent_grad(const VectorXd & x, void*d[]) {
	return x;
}
void testGradientDescent() {
	cout << "Minimizing x'*x using gradient descent..." << endl;
	VectorXd initialSolution(5);
	for (int i = 0; i < 5; i++)
		initialSolution[i] = (double)i;
	VectorXd soln = gradientDescent(testGradientDescent_f, testGradientDescent_grad, initialSolution, nullptr);
	cout << "soln = " << soln.transpose() << endl;
}

void testMatlabEngine()
{
	MatlabEngine::initMatlab();
	VectorXd x(100), y(100);
	for (int i = 0; i < 100; i++) {
		x[i] = i;
		y[i] = sqrt(i);
	}
	MatlabEngine::scatter(x, y);

	cout << "If Matlab Engine link is working, you should see a scatter plot." << endl;
	cout << "Press enter to close the Matlab engine and continue." << endl;
	forceGetchar();
	MatlabEngine::closeMatlab();
}