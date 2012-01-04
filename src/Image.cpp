#include "Image.hpp"

Image::Image() :
	path(),pointlistPath(), image(), points()
{}

Image::~Image() {
	delete [] pCamera;
	delete [] pFakeCamera;
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

kn::Matrix3x3d Image::resolveHomography() {
	//for(
	kn::Matrix3x3d rotX, rotY, rotZ;
	rotX.setIdentity();
	rotY.setIdentity();
	rotZ.setIdentity();
	return pCamera->computeHomography(rotX * rotY * rotZ);
}

double Image::f() {
	return 1.;
}


kn::Vector<double> Image::b() {
	if(points.size() == 0) {
		throw "no points";
		exit(1);
	}
	kn::Vector<double> b(4 + 3*points.size());
	b[0] = image.width();
	b[1] = image.height();
	b[2] = pCamera->focale;
	b[3] = points.size();
	for(size_t i = 0 ; i < points.size(); ++i)
		for(size_t c = 0; c < 3 ; ++c)
			b[4+i*3+c] = points[i][c];
	// @TODO : Make vector b.
	return b;
}
