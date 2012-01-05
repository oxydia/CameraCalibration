#ifndef _MATHS_HPP_
#define _MATHS_HPP_

#include "Image.hpp"
#include <OpenKN/math/Matrix3x3.hpp>
#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/Solver.hpp>
#include <OpenKN/math/InverseMatrix.hpp>
#include <cstdlib>
#include <cassert>

void nonLinearSystemSolver(
	kn::Vector<double> &a,
	const kn::Vector<double> &b,
	double (*pF)(kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> & imgs),
	const size_t nbMaxIterations,
	const std::vector<Image*> & imgs);

/*double _nonLinearSystemSolverSVD(
	kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*pF)(kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> &),
	kn::Matrix<double> & j,
	const std::vector<Image*> & imgs,
	const kn::Matrix<double> alpha);
*/

kn::Vector<double> _nonLinearSystemSolverJacobian(
	kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*pF)(kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> &),
	const std::vector<Image*> & imgs);

double f(kn::Vector<double> & a, const kn::Vector<double> & b, const std::vector<Image*> & imgs);

kn::Vector4d resolvePointTriangulation(const size_t iPoint, const std::vector<Image*> & imgs);

#endif // _MATHS_HPP_
