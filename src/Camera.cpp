#include "Camera.hpp"

using namespace std;
using namespace kn;

Camera::Camera(const size_t & h, const size_t & w) :
	height(h), width(w), focale(sqrt(h*h+w*w)),
	position(),
	intrinsecParameters(),
	rotation(), center(),
	projection(3,4)
{}

Camera::Camera(const Camera & other) :
	height(other.height), width(other.width), focale(other.focale),
	position(other.position), intrinsecParameters(other.intrinsecParameters), rotation(other.rotation),
	center(other.center), projection(other.projection)
{}

Camera::~Camera() {}

Matrix3x3d Camera::computeIntrinsecParameters() {
	intrinsecParameters[0][0] = focale;
	intrinsecParameters[1][1] = focale;
	intrinsecParameters[2][2] = 1.;
	intrinsecParameters[0][2] = height/2.;
	intrinsecParameters[1][2] = width/2.;
	return intrinsecParameters;
}

Matrix3x3d Camera::computeHomography(const Matrix3x3d & rotation) {
	Matrix3x3d homography = intrinsecParameters;
	homography *= rotation;
	homography *= inverseMatrixGaussianElimination(intrinsecParameters);
	return homography;
}

/*
Camera::_setExtrinsecParameters() {
	_setRotation();
	_setCenter();
	Vector3d minus_rc = - R*c;
	Matrix<double> p(3,4);
	p[];
	p *= m_K; 
}

Camera::_setRotation() {
	
}

Camera::_setCenter() {
	
}
*/


