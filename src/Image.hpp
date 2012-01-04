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
#include <string>
#include <exception>

// Will stock our results and manage space.
struct Image {
	size_t id;
	std::string path;
	std::string pointlistPath;
	kn::ImageRGB8u image;
	std::vector< kn::Vector3d > points;
	kn::Vector<double> a;
	Camera* pCamera;
	Camera* pFakeCamera;
	kn::Matrix3x3d homography;
	
	void loadJPG();
	void loadPoints();
	void setCamera();
	
	kn::Matrix3x3d resolveHomography();
	
	kn::Vector<double> b();
	double f();
	
	Image();
	// Destructor
	~Image();
};

#endif // _IMAGE_HPP_
