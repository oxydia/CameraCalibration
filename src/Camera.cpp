#include "Camera.hpp"

using namespace std;
using namespace kn;

Camera::Camera(const size_t & h, const size_t & w) :
	height(h), width(w), focale(sqrt(h*h+w*w)),
	position(), rotation(),
	center(height/2., width/2., 1.),
	projection(3,4)
{
	rotation.setIdentity();
	computeProjection();
}

Camera::Camera(const Camera & other) :
	height(other.height), width(other.width), focale(other.focale),
	position(other.position), rotation(other.rotation),
	center(other.center), projection(other.projection)
{}

Camera::~Camera() {}

Matrix3x3d Camera::intrinsecParameters() {
	Matrix3x3d iP;
	iP[0][0] = focale;
	iP[1][1] = focale;
	iP[0][2] = center[0];
	iP[1][2] = center[1];
	iP[2][2] = center[2];
	return iP;
}

Matrix<double> Camera::extrinsecParameters() {
	Matrix<double> eP(3,4);
	for(size_t i = 0; i < 3; ++i)
		for(size_t j=0; j < 3; ++j)
			eP[i][j] = rotation[i][j];
	Vector3d newCenter = - rotation * position;
	eP[0][3] = newCenter[0];
	eP[1][3] = newCenter[1];
	eP[2][3] = newCenter[2];
	return eP;
}
	
Matrix3x3d Camera::homography() {
	return computeHomographyFromRotation(rotation);
}

Matrix3x3d Camera::computeHomographyFromRotation(const Matrix3x3d & rot) {
	Matrix3x3d iP = intrinsecParameters();
	Matrix3x3d homography = iP;
	homography *= rot;
	homography *= inverseMatrixGaussianElimination(iP);
	return homography;
}

kn::Vector3d Camera::project(const Vector4d & point3d) {
	return projection * point3d;
}

void Camera::computeProjection() {
	projection = intrinsecParameters() * extrinsecParameters();
}
