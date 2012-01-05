#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include "Camera.hpp"
#include <sstream>
#include <vector>
#include <OpenKN/image/ImageRGB.hpp>
#include <OpenKN/image/ioJPG.hpp>
#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/Matrix3x3.hpp>
#include <OpenKN/math/MathIO.hpp>
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
	Camera* pCamera;
	
	void loadJPG();
	void loadPoints();
	void setCamera();
	
	kn::Matrix3x3d resolveRotationEuler(kn::Vector3d angle);
	
	Image();
	// Destructor
	~Image();
};

#endif // _IMAGE_HPP_
