#include <stdafx.h>

using namespace std;
using namespace Eigen;

Thing::Thing() {
}

Thing::Thing(const Thing & rhs)
{
	disc = rhs.disc;
	cont = rhs.cont;
}

Thing & Thing::operator=(const Thing & rhs)
{
	disc = rhs.disc;
	cont = rhs.cont;
	return *this;
}

Thing & Thing::operator=(const int & rhs)
{
	disc = rhs;
	cont.resize(0);
	return *this;
}

Thing & Thing::operator=(const Eigen::VectorXd & rhs)
{
	disc = 0;
	cont = rhs;
	return *this;
}

Thing::Thing(int disc) {
	this->disc = disc;
}

Thing::Thing(const Eigen::VectorXd & cont) {
	this->cont = cont;
}

Thing::Thing(int disc, const Eigen::VectorXd & cont) {
	this->disc = disc;
	this->cont = cont;
}

ostream & operator <<(ostream & out, const Thing & t)
{
	out << t.disc << '\t' << (int)t.cont.size() << t.cont << endl;
	return out;
}

istream & operator >>(istream  &in, Thing & t) {
	int contDim;
	in >> t.disc >> contDim;
	t.cont.resize(contDim);
	in >> t.cont;
	return in;
}

ThingDesc::ThingDesc()
{
	numDisc = 0;
	contDim = 0;
	contRange.first.resize(0);
	contRange.second.resize(0);
}

ThingDesc::ThingDesc(const ThingDesc & rhs)
{
	numDisc = rhs.numDisc;
	contDim = rhs.contDim;
	contRange = rhs.contRange;
}

ThingDesc & ThingDesc::operator=(const ThingDesc & rhs)
{
	numDisc = rhs.numDisc;
	contDim = rhs.contDim;
	contRange = rhs.contRange;
	return *this;
}

ThingDesc::ThingDesc(int numDisc)
{
	this->numDisc = numDisc;
	contDim = 0;
	contRange.first.resize(0);
	contRange.second.resize(0);
}

ThingDesc::ThingDesc(
	int numDisc,
	int contDim,
	pair<VectorXd, VectorXd> contRange)
{
	this->numDisc = numDisc;
	this->contDim = contDim;
	this->contRange = contRange;
}

ostream & operator <<(ostream &out, const ThingDesc &t)
{
	out << t.numDisc << '\t' << t.contDim << '\t';
	for (int i = 0; i < t.contDim; i++)
		out << t.contRange.first[i] << '\t' << t.contRange.second[i] << '\t';
	return out;
}

istream & operator >>(istream  &in, ThingDesc & t)
{
	in >> t.numDisc >> t.contDim;
	t.contRange.first.resize(t.contDim);
	t.contRange.second.resize(t.contDim);
	for (int i = 0; i < t.contDim; i++) {
		in >> t.contRange.first[i] >> t.contRange.second[i];
	}
	return in;
}