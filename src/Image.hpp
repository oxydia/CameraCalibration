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
	char* path;
	kn::ImageRGB8u image;
	std::vector< kn::Vector3d > points;
	Camera* camera;
	
	void loadJPG() {
		kn::loadJPG(image, path);
	}
	void loadPoints() {
		std::stringstream pointsFilename;
		pointsFilename << this->path << ".list";
		try {
			kn::loadVectorList(this->points, pointsFilename.str());
		}
		catch(...) {
			std::cerr << "Unable to load the points list in file " << pointsFilename.str().c_str() << "." << std::endl;
			std::cerr << "Aborting mission !" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	std::vector<double> getB() {
		std::vector<double> b;
		// @TODO : Make vector b.
		return b;
	}
	// making camera. The constructor of camera take it from there.
	void setCamera() {
		// Check if we better use a pointer or not (then how to construct ?).
		this->camera = new Camera(this->image.width(), this->image.height());
	}
	// Destructor
	~Image() {
		delete [] camera;
		delete [] path;
	}
};

#endif // _IMAGE_HPP_
