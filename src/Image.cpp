#include "Image.hpp"

Image::Image() :
	id(), path(NULL), image(), points(), camera(NULL)
{}

Image::~Image() {
	delete [] camera;
	delete [] path;
}

void Image::loadJPG() {
	kn::loadJPG(image, path);
}

void Image::loadPoints() {
	std::stringstream pointsFilename;
	pointsFilename << path << ".list";
	try {
		kn::loadVectorList(points, pointsFilename.str());
	}
	catch(...) {
		std::cerr << "Unable to load the points list in file " << pointsFilename.str().c_str() << "." << std::endl;
		std::cerr << "Aborting." << std::endl;
		exit(EXIT_FAILURE);
	}
}

std::vector<double> Image::getB() {
	std::vector<double> b;
	// @TODO : Make vector b.
	return b;
}

// making camera. The constructor of camera take it from there.
void Image::setCamera() {
	// Check if we better use a pointer or not (then how to construct ?).
	camera = new Camera(id, (size_t)image.width(), (size_t)image.height());
}
