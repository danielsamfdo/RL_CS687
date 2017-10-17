#pragma once

#include <stdafx.h>

class TabularSoftmax : public Policy {
public:
	// Constructors and assignment operator
	TabularSoftmax(const int & numObs, const int & numActs);
	~TabularSoftmax() override;

	int getNumParams() const;
	Eigen::VectorXd getParams() const;
	void setParams(const Eigen::VectorXd & newParams);
	void addToParams(const Eigen::VectorXd & dParams);

	void getAction(Thing & buff, const Thing & o, std::mt19937_64 & generator) override;
	double pi(const Thing & o, const Thing & a) const override;
	void dpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const override;
	void dlnpi(Eigen::VectorXd & buff, const Thing & o, const Thing & a) const override;
	TabularSoftmax * clone(std::mt19937_64 & generator) const override;

private:
	int numActs;

	Eigen::VectorXd params;

	Eigen::VectorXd getActionProbabilities(const int & o) const;
};