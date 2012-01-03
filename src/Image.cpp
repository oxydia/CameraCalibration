#include "Image.hpp"

using namespace std;

Image::Image() :
	path(NULL), image(), points()
{}

Image::~Image() {
	delete [] path;
	delete [] pCamera;
	delete [] pFakeCamera;
}

void Image::loadJPG() {
	kn::loadJPG(image, path);
}

void Image::loadPoints() {
	std::stringstream pointsFilename;
	pointsFilename << path << ".list";
	kn::loadVectorList(points, pointsFilename.str());
}

void Image::setCamera() {
	pCamera = new Camera((size_t)image.width(), (size_t)image.height());
}

kn::Matrix3x3d Image::resolveHomography() {
	//for(
	kn::Matrix3x3d rotX, rotY, rotZ;
	rotX.setIdentity();
	rotY.setIdentity();
	rotZ.setIdentity();
	return pCamera->computeHomography(rotX * rotY * rotZ);
}

/*vector<double> Image::getA() {
	
	return;
}*/


vector<double> Image::getB() {
	if(points.size() == 0) {
		throw "no points";
		exit(1);
	}
	vector<double> b;
	b.push_back(image.width());
	b.push_back(image.height());
	b.push_back(pCamera->focale);
	b.push_back(points.size());
	for(size_t i = 0; i < points.size(); ++i)
		for(size_t c = 0; c < 3 ; ++i)
			b.push_back(points[i][c]);
	// @TODO : Make vector b.
	return b;
}
