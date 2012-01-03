#ifndef _MATHS_HPP_
#define _MATHS_HPP_

void nonLinearSystemSolver(
	kn::Vector<double> &a,
	const kn::Vector<double> &b,
	double (*costFunctionPtr)(const kn::Vector<double>&,
	const kn::Vector<double>&),
	const unsigned int nbMaxIterations)
{}

#endif // _MATHS_HPP_
