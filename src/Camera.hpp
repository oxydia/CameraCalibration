#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "Print.hpp"
#include <cstdlib>
#include <cmath>
#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/Matrix3x3.hpp>
#include <OpenKN/math/InverseMatrix.hpp>
// @TODO : Need to add debug code
#include <cassert>
#include <iostream>
#include <exception>

struct Camera {
	size_t height;
	size_t width;
	float focale;
	kn::Vector3d position;
	kn::Matrix3x3d intrinsecParameters;
	kn::Matrix3x3d rotation;
	kn::Vector3d center;
	kn::Matrix<double> projection;
	
	virtual kn::Matrix3x3d computeIntrinsecParameters();
	virtual kn::Matrix3x3d homography();
	
	// We can add here functions for computing other Camera's attributes
	// Constructor & Destructor
	Camera(const size_t & h, const size_t & w);
	Camera(const Camera & other);
	virtual ~Camera();
	
};

#endif // _CAMERA_HPP_
