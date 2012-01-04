#include "Maths.hpp"

void nonLinearSystemSolver(
	kn::Vector<double> &a,
	const kn::Vector<double> &b,
	double (*pF)(const kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> & imgs),
	const size_t nbMaxIterations,
	const std::vector<Image*> & imgs)
{
	// derivative of f(a,b) (d a) // f returns a scalar so j is a one line matrix (a vector)
	kn::Vector<double> jacobianVector = _nonLinearSystemSolverJacobian(a, b, pF, imgs);
	// jBase is the jacobianVector converted into a matrix.
	kn::Matrix<double> j, jBase(1, a.size(), jacobianVector, true);

	float alpha = jacobianVector*jacobianVector * pow(10, -3);
	for(size_t i = 0; i < nbMaxIterations ; ++i) {
		j = jBase;
		size_t cpt = 0;
		bool accepted = false;
		do {
			double step_a = _nonLinearSystemSolverSVD(a, b, pF, j, imgs);
			if(fabs(pF(a + step_a, b, imgs)) < fabs(pF(a, b, imgs))) {
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
	double (*pF)(const kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> &),
	kn::Matrix<double> & j,
	const std::vector<Image*> & imgs)
{
	kn::Vector<double> w, x, jSVDPseudoInverse;
	kn::Matrix<double> u = j, v;
	decompositionSVD(u, w, v);
	sortSingularValuesSVD(u, w, v);
	solveSVD(u, w, v, jSVDPseudoInverse, x);
	// jSVDPseudoInverse should be a 1*1 matrix
	assert(jSVDPseudoInverse.size() == 1);
	return (double)(-jSVDPseudoInverse[0] * pF(a, b, imgs));
}

kn::Vector<double> _nonLinearSystemSolverJacobian(
	const kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*pF)(const kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> &),
	const std::vector<Image*> & imgs)
{
	kn::Vector<double> vectorJacobi(a.size());
	// partial derivation 
	for(size_t j = 0; j < a.size(); ++j)
		vectorJacobi[j] = ( pF(a[j], b, imgs) - pF(a, b, imgs) ) / std::max(std::min(a[j]*std::pow(10, -4), std::pow(10, -6)), std::pow(10, -15));
	// we return a matrix
	return vectorJacobi;
}

double f(kn::Vector<double> & a, const kn::Vector<double> & b, const std::vector<Image*> & imgs) {
	double res(0), proj1x1Y, proj2x2Y;
	kn::Vector3d point1;
	kn::Vector3d point2;
	if(imgs.size() != 2) {
		//  @TODO : accept more images ?
		throw "You need to select only 2 images !";
		exit(1);
	}
	kn::Matrix3x3d rotationCam1 = imgs[0]->pCamera->computeHomographyFromRotation(kn::eulerAngles3x3d(a[0], a[1], a[2]));
	kn::Matrix3x3d rotationCam2 = imgs[1]->pCamera->computeHomographyFromRotation(kn::eulerAngles3x3d(a[3], a[4], a[5]));
	for(size_t i = 0; i < b[3]; ++i) {
		// @FIXME : Review coordinates of points in b
		proj1x1Y = (rotationCam1 * kn::Vector3d(b[4+4*i],b[4+4*i+1],1.))[1];
		proj2x2Y = (rotationCam2 * kn::Vector3d(b[4+4*i+2],b[4+4*i+3],1.))[1];
		//  @TODO : accept more images ?
		res += pow(proj1x1Y - proj2x2Y, 2);
	}
	return res;
}

// @FIXME
kn::Vector3d resolvePointTriangulation(const size_t iPoint, const std::vector<Image*> & imgs) {
	for(size_t i = 0 ; i < imgs.size() ; ++i) {
		// compute imgs[i].points[iPoint] together to find x, y, z
	}
	return kn::Vector3d();
}
