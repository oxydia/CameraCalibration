#include "Maths.hpp"

void nonLinearSystemSolver(
	kn::Vector<double> &a,
	const kn::Vector<double> &b,
	double (*pF)(const kn::Vector<double>&, const kn::Vector<double>&),
	const size_t nbMaxIterations)
{
	// derivative of f(a,b) (d a) // f returns a scalar so j is a one line matrix (a vector)
	kn::Vector<double> jacobianVector = _nonLinearSystemSolverJacobian(a, b, pF);
	// jBase is the jacobianVector converted into a matrix.
	kn::Matrix<double> j, jBase(1, a.size(), jacobianVector, true);

	float alpha = jacobianVector*jacobianVector * pow(10, -3);
	for(size_t i = 0; i < nbMaxIterations ; ++i) {
		j = jBase;
		size_t cpt = 0;
		bool accepted = false;
		do {
			double step_a = _nonLinearSystemSolverSVD(a, b, pF, j);
			if(fabs(pF(a + step_a, b)) < fabs(pF(a, b))) {
				a+= step_a;
				alpha /= 10.;
				accepted = true;
			} else
				alpha *= 10.;

			++cpt;
			if(cpt > 100)
				accepted = true;
		} while (!accepted);
	}
}

double _nonLinearSystemSolverSVD(
	kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*costFunctionPtr)(const kn::Vector<double>&, const kn::Vector<double>&),
	kn::Matrix<double> & j)
{
	kn::Vector<double> w, x, jSVDPseudoInverse;
	kn::Matrix<double> u = j, v;
	decompositionSVD(u, w, v);
	sortSingularValuesSVD(u, w, v);
	solveSVD(u, w, v, jSVDPseudoInverse, x);
	// jSVDPseudoInverse should be a 1*1 matrix
	assert(jSVDPseudoInverse.size() == 1);
	return (double)(-jSVDPseudoInverse[0] * costFunctionPtr(a,b));
}

kn::Vector<double> _nonLinearSystemSolverJacobian(
	const kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*pF)(const kn::Vector<double>&, const kn::Vector<double>&))
{
	kn::Vector<double> vectorJacobi(a.size());
	// partial derivation 
	for(size_t j = 0; j < a.size(); ++j)
		vectorJacobi[j] = ( pF(a[j], b) - pF(a, b) ) / std::max(std::min(a[j]*std::pow(10, -4), std::pow(10, -6)), std::pow(10, -15));
	// we return a matrix
	return vectorJacobi;
}
