#include "Maths.hpp"

void nonLinearSystemSolver(
	kn::Vector<double> &a,
	const kn::Vector<double> &b,
	double (*pF)(kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> & imgs),
	const size_t nbMaxIterations,
	const std::vector<Image*> & imgs)
{
	// derivative of f(a,b) (d a) // f returns a scalar so j is a one line matrix (a vector)
	kn::Vectord jacobianVector = _nonLinearSystemSolverJacobian(a, b, pF, imgs);
	// jBase is the jacobianVector converted into a matrix.
	kn::Matrixd j, jT = j;
	jT.transpose();
	kn::Matrixd jTj = jT * j;
	
	// alpha = valeur moyenne des elements de la diagonale de jTransposed * j;
	double alpha = 0.;
	for(size_t i = 0; i < a.size(); ++i)
		alpha += jTj[i][i];
	alpha *= pow(10, -3) / a.size();
	
	for(size_t i = 0; i < nbMaxIterations ; ++i) {
		if(i != 0) {
			// derivative of f(a,b) (d a) // f returns a scalar so j is a one line matrix (a vector)
			jacobianVector = _nonLinearSystemSolverJacobian(a, b, pF, imgs);
			j = kn::Matrixd(1, a.size(), jacobianVector, true);
			jT = j; jT.transpose();
		}
		size_t cpt = 0;
		bool accepted = false;
		do {
			//kn::Matrixd step_a_matrix = _nonLinearSystemSolverSVD(a, b, pF, j, imgs, alpha);
			kn::Matrix<double> identity(a.size(), a.size());
			identity.setIdentity();
			kn::Matrix<double> aSystem;
			kn::Vector<double> step_a, bSystem;
			aSystem = jT * j + alpha * identity;
			bSystem = - jacobianVector * f(a, b, imgs);
			solveSystemSVD(aSystem, step_a, bSystem);
			std::cout << "Step_a = " << std::endl;
			printVector(step_a);
			//for(size_t c = 0; c < step_a.size(); ++c)
			//	step_a[c] = step_a_matrix[c][0];
			kn::Vector<double> a_plus_step_a = a + step_a;
			if(fabs(pF(a_plus_step_a, b, imgs)) < fabs(pF(a, b, imgs))) {
				a = a_plus_step_a;
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

/*double _nonLinearSystemSolverSVD(
	kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*pF)(kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> &),
	kn::Matrix<double> & j,
	const std::vector<Image*> & imgs,
	const kn::Matrix<double> & alpha)
{
	return - pseudoInverseMatrixSVD(j) * f(a, b, imgs);
	
	kn::Vector<double> w, x, jSVDPseudoInverse;
	kn::Matrix<double> u = j, v;
	decompositionSVD(u, w, v);
	sortSingularValuesSVD(u, w, v);
	solveSVD(u, w, v, jSVDPseudoInverse, x);
	// jSVDPseudoInverse should be a 1*1 matrix
	assert(jSVDPseudoInverse.size() == 1);
	return (double)(-jSVDPseudoInverse[0] * pF(a, b, imgs));
}*/

kn::Vector<double> _nonLinearSystemSolverJacobian(
	kn::Vector<double> &a, const kn::Vector<double> &b,
	double (*pF)(kn::Vector<double>&, const kn::Vector<double>&, const std::vector<Image*> &),
	const std::vector<Image*> & imgs)
{
	kn::Vector<double> vectorJacobi(a.size());
	kn::Vector<double> aJ;
	// partial derivation
	for(size_t j = 0; j < a.size(); ++j) {
		aJ = a;
		double delta_aj = std::max(std::min(a[j]*std::pow(10, -4), std::pow(10, -6)), std::pow(10, -15));
		aJ[j] += delta_aj;
		vectorJacobi[j] = ( pF(aJ, b, imgs) - pF(a, b, imgs) ) / delta_aj;
	}
	// we return a matrix
	return vectorJacobi;
}

double f(kn::Vector<double> & a, const kn::Vector<double> & b, const std::vector<Image*> & imgs) {
	double res(0), proj1x1Y(0), proj2x2Y(0);
	kn::Vector3d point1;
	kn::Vector3d point2;
	if(imgs.size() != 2) {
		std::cerr << "You need to select only 2 images !" << std::endl;
		exit(1);
	}
	kn::Matrix3x3d rotationCam1 = imgs[0]->pCamera->computeHomographyFromRotation(kn::eulerAngles3x3d(a[0], a[1], a[2]));
	kn::Matrix3x3d rotationCam2 = imgs[1]->pCamera->computeHomographyFromRotation(kn::eulerAngles3x3d(a[3], a[4], a[5]));
	for(size_t i = 0; i < b[3]; ++i) {
		proj1x1Y = (rotationCam1 * kn::Vector3d(b[4+4*i],b[4+4*i+1],1.))[1];
		proj2x2Y = (rotationCam2 * kn::Vector3d(b[4+4*i+2],b[4+4*i+3],1.))[1];
		res += pow(proj1x1Y - proj2x2Y, 2);
	}
	return res;
}

// @FIXME
kn::Vector4d resolvePointTriangulation(const size_t iPoint, const std::vector<Image*> & imgs) {
	for(size_t i = 0 ; i < imgs.size() ; ++i) {
		// compute imgs[i].points[iPoint] together to find x, y, z
	}
	return kn::Vector4d();
}
