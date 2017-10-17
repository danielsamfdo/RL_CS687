#include <stdafx.h>
#include <policies/TabularSoftmax.hpp>

using namespace std;
using namespace Eigen;

// Constructor. Sigma scales how much exploration is included. Large values mean less exploration
TabularSoftmax::TabularSoftmax(const int & numObs, const int & numActs)
{
	this->numActs = numActs;
	params = VectorXd::Zero(numObs*numActs);
}

TabularSoftmax::~TabularSoftmax() {}

int TabularSoftmax::getNumParams() const {
	return (int)params.size();
}

Eigen::VectorXd TabularSoftmax::getParams() const
{
	return params;
}

void TabularSoftmax::setParams(const Eigen::VectorXd & newParams)
{
	this->params = newParams;
}

void TabularSoftmax::addToParams(const Eigen::VectorXd & dParams)
{
	params.array() += dParams.array();
}

void TabularSoftmax::getAction(Thing & buff, const Thing & o, mt19937_64 & generator) 
{
	buff.cont.resize(0);
	buff.disc = randp(generator, getActionProbabilities(o.disc));
}

double TabularSoftmax::pi(const Thing & o, const Thing & a) const
{
	return getActionProbabilities(o.disc)[a.disc];
}

void TabularSoftmax::dpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const
{
	buff = VectorXd::Zero(params.size());
	buff.segment(o.disc*numActs, numActs) = -getActionProbabilities(o.disc);
	double poa = -buff(o.disc*numActs + a.disc); // Remember pi(o,a)
	buff(o.disc*numActs + a.disc) += 1;
	buff *= poa;
}

void TabularSoftmax::dlnpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const
{
	buff = VectorXd::Zero(params.size());
	buff.segment(o.disc*numActs, numActs) = -getActionProbabilities(o.disc);
	buff(o.disc*numActs + a.disc) += 1;
}

TabularSoftmax * TabularSoftmax::clone(mt19937_64 & generator) const
{
	return new TabularSoftmax(*this);
}

VectorXd TabularSoftmax::getActionProbabilities(const int & o) const
{
	VectorXd result = params.segment(o*numActs, numActs).array().exp();
	result /= (double)result.sum();
	return result;
}