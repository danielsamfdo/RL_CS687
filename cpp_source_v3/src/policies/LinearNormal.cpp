#include <stdafx.h>
#include <policies/LinearNormal.hpp>

using namespace std;
using namespace Eigen;

// Constructor. Sigma scales how much exploration is included. Large values mean less exploration
LinearNormal::LinearNormal(
	const ThingDesc & oDesc,
	const bool & tuneSigma,					// Is sigma a tunable parameter?
	const double & sigma,					// Initial standard deviation
	const Basis * b)
{
	this->oDesc = oDesc;
	this->tuneSigma = tuneSigma;
	
	this->b = b->clone();
	numFeatures = b->getNumOutputs();
	if (tuneSigma)
	{
		params = VectorXd::Zero(numFeatures + 1);
		params[numFeatures] = sigma;
		this->sigma = -INFINITY;	// Shouldn't ever use it.
	}
	else
	{
		params = VectorXd::Zero(numFeatures);
		this->sigma = sigma;
	}

	d = normal_distribution<double>(0, 1);
}

LinearNormal::LinearNormal(const LinearNormal & rhs)
{
	oDesc = rhs.oDesc;
	tuneSigma = rhs.tuneSigma;
	sigma = rhs.sigma;
	b = rhs.b->clone();
	numFeatures = rhs.numFeatures;
	params = rhs.params;
	d = rhs.d;
}

LinearNormal & LinearNormal::operator=(const LinearNormal & rhs)
{
	delete b;

	oDesc = rhs.oDesc;
	tuneSigma = rhs.tuneSigma;
	sigma = rhs.sigma;
	b = rhs.b->clone();
	numFeatures = rhs.numFeatures;
	params = rhs.params;
	d = rhs.d;

	return *this;
}

LinearNormal::~LinearNormal()
{
	delete b;
}

int LinearNormal::getNumParams() const {
	return (int)params.size();
}

Eigen::VectorXd LinearNormal::getParams() const
{
	return params;
}

void LinearNormal::setParams(const Eigen::VectorXd & newParams)
{
	this->params = newParams;
}

void LinearNormal::addToParams(const Eigen::VectorXd & dParams)
{
	params.array() += dParams.array();
}

void LinearNormal::getAction(Thing & buff, const Thing & o, mt19937_64 & generator) 
{
	buff.disc = -1;
	buff.cont.resize(1);
	VectorXd features;
	b->basify(features, o.cont);

	if (tuneSigma)
	{
		// Get mean
		buff.cont[0] = params.segment(0, numFeatures).dot(features);
		// Add noise
		buff.cont[0] += params[numFeatures] * d(generator);
	}
	else
	{
		// Get mean
		buff.cont[0] = params.dot(features);
		// Add noise
		buff.cont[0] += sigma * d(generator);
	}		
}

double LinearNormal::pi(const Thing & o, const Thing & a) const
{
	VectorXd features;
	b->basify(features, o.cont);
	if (tuneSigma)
	{
		double mu = params.segment(0, numFeatures).dot(features);
		return normpdf(a.cont[0], mu, params[numFeatures]);
	}
	else
	{
		double mu = params.dot(features);
		return normpdf(a.cont[0], mu, sigma);
	}
}

void LinearNormal::dpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const
{
	VectorXd features;
	b->basify(features, o.cont);
	buff.resize(params.size());
	if (tuneSigma)
	{
		// Gradient w.r.t. theta
		double mu = params.dot(features.segment(0, numFeatures));
		double pi = normpdf(a.cont[0], mu, params[numFeatures]);
		double scalarTerm = pi*(a.cont[0] - mu) / (params[numFeatures] * params[numFeatures]);
		buff.resize(params.size());
		buff.segment(0, numFeatures) = scalarTerm*features;

		// Gradient w.r.t. sigma
		double temp1 = (a.cont[0] - mu) / (params[numFeatures] * params[numFeatures] * params[numFeatures]);
		double temp2 = 1.0 / params[numFeatures];
		buff[numFeatures] = pi*(temp1 - temp2);
	}
	else
	{
		double mu = params.dot(features);
		double pi = normpdf(a.cont[0], mu, sigma);
		double scalarTerm = pi*(a.cont[0] - mu) / (sigma*sigma);
		buff = scalarTerm*features;
	}
}

void LinearNormal::dlnpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const
{
	VectorXd features;
	b->basify(features, o.cont);
	buff.resize(params.size());
	if (tuneSigma)
	{
		// Gradient w.r.t. theta
		double mu = params.dot(features.segment(0, numFeatures));
		double scalarTerm = (a.cont[0] - mu) / (params[numFeatures] * params[numFeatures]);
		buff.resize(params.size());
		buff.segment(0, numFeatures) = scalarTerm*features;

		// Gradient w.r.t. sigma
		double temp1 = (a.cont[0] - mu) / (params[numFeatures] * params[numFeatures] * params[numFeatures]);
		double temp2 = 1.0 / params[numFeatures];
		buff[numFeatures] = (temp1 - temp2);
	}
	else
	{
		double mu = params.dot(features);
		double scalarTerm = (a.cont[0] - mu) / (sigma*sigma);
		buff = scalarTerm*features;
	}
}

LinearNormal * LinearNormal::clone(mt19937_64 & generator) const
{
	return new LinearNormal(*this);
}