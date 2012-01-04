#include "Image.hpp"

Image::Image() :
	path(),pointlistPath(), image(), points()
{}

Image::~Image() {
	delete [] pCamera;
}

void Image::loadJPG() {
	kn::loadJPG(image, path);
}

void Image::loadPoints() {
	if(pointlistPath == "") {
		throw "no pointlist path !";
	}
	kn::loadVectorList(points, pointlistPath);
}

void Image::setCamera() {
	pCamera = new Camera((size_t)image.width(), (size_t)image.height());
}
