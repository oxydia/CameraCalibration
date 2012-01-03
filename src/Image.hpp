#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

// @TODO : Resolve compilations errors
#include "Camera.hpp"
#include <sstream>
#include <vector>
#include <OpenKN/image/ImageRGB.hpp>
#include <OpenKN/image/ioJPG.hpp>
#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/MathIO.hpp>
// @TODO : Need to add debug code
#include <cassert>
#include <exception>

// Will stock our results and manage space.
struct Image {
	size_t id;
	char* path;
	kn::ImageRGB8u image;
	std::vector< kn::Vector3d > points;
	Camera* pCamera;
	Camera* pFakeCamera;
	kn::Matrix3x3d homography;
	
	void loadJPG();
	void loadPoints();
	void setCamera();
	
	kn::Matrix3x3d resolveHomography();
	
	std::vector<double> getB();
	Image();
	// Destructor
	~Image();
};

#endif // _IMAGE_HPP_
