#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

// @TODO : Resolve compilations errors
#include "Camera.hpp"
#include <sstream>
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
	Camera* camera;
	
	void loadJPG();
	void loadPoints();
	std::vector<double> getB();
	// making camera. The constructor of camera take it from there.
	void setCamera();
	Image();
	// Destructor
	~Image();
};

#endif // _IMAGE_HPP_
