#include <stdafx.h>

using namespace std;
using namespace Eigen;

VectorXd gradientDescent(
	double(*f)(const VectorXd &, void*[]),	// Function to be minimized
	VectorXd(*grad)(const VectorXd &, void*[]), // gradient of function
	VectorXd & initialSolution, // Initial solution
	void* d[], // Extra information passed to function (e.g., data set)
	const int & maxSteps, // Maximum steps before termination
	const double & maxStepSize) // Maximum step size - this is divided by 2 100 times, so starting with 100 can still allow for small steps.
{
	// Delcare variables, set x = initial solution
	int j, step;
	double stepSize, curVal, newVal;
	VectorXd g, x = initialSolution, newX;

	// Evaluate the function at initial point
	curVal = f(x, d);

	// Loop over iterations
	for (step = 0; step < maxSteps; step++) {
		// Compute the gradient
		g = grad(x, d);

		// Use backtracking search to pick the step size
		stepSize = maxStepSize;
		for (j = 0; j < 1000; j++) {
			newVal = f(x - stepSize*g, d);
			if (newVal < curVal)
				break;
			stepSize = stepSize / 2;
		}
		if (j == 1000)
			break; // We couldn't find a small enough step size to get an improvement.

		// Take the step
		x = x - stepSize*g;

		// Get the value of the function after the step
		curVal = newVal;
	}

	// Return the solution
	return x;
}