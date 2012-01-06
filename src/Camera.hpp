#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <cstdlib>
#include <cmath>
#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/Matrix3x3.hpp>
#include <OpenKN/math/InverseMatrix.hpp>
#include <OpenKN/math/EulerAngles.hpp>
#include <cassert>
#include <iostream>
#include <exception>

struct Camera {
	size_t height;
	size_t width;
	float focale;
	kn::Vector3d position;
	kn::Matrix3x3d rotation;
	kn::Vector3d center;
	kn::Matrix<double> projection;
	
	virtual kn::Matrix3x3d intrinsecParameters();
	virtual kn::Matrix<double> extrinsecParameters();
	virtual kn::Matrix3x3d homography();
	virtual kn::Matrix3x3d computeHomographyFromRotation(const kn::Matrix3x3d & rot);
	// transfom the coordinates of a point with the projection of the camera
	virtual kn::Vector3d project(const kn::Vector4d & point3d);
	virtual void computeProjection();
	
	// We can add here functions for computing other Camera's attributes
	// Constructor & Destructor
	Camera(const size_t & h, const size_t & w);
	Camera(const Camera & other);
	virtual ~Camera();
	
};

#endif // _CAMERA_HPP_
