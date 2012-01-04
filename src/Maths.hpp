#ifndef _MATHS_HPP_
#define _MATHS_HPP_

#include <OpenKN/math/Matrix3x3.hpp>
#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/SVD.hpp>
#include <cstdlib>
#include <cassert>

void nonLinearSystemSolver(
	kn::Vector<double> &a,
	const kn::Vector<double> &b,
	double (*costFunctionPtr)(const kn::Vector<double>&,
	const kn::Vector<double>&),
	const size_t nbMaxIterations);

double _nonLinearSystemSolverSVD(
	kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*costFunctionPtr)(const kn::Vector<double>&, const kn::Vector<double>&),
	kn::Matrix<double> & j);

kn::Vector<double> _nonLinearSystemSolverJacobian(
	const kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*costFunctionPtr)(const kn::Vector<double>&, const kn::Vector<double>&));

#endif // _MATHS_HPP_
