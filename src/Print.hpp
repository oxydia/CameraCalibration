#ifndef _PRINT_HPP_
#define _PRINT_HPP_

#include "Image.hpp"
#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/Matrix.hpp>
#include <OpenKN/image/DrawCircle.hpp>
#include <OpenKN/image/ioJPG.hpp>
#include <OpenKN/image/DrawCircle.hpp>
#include <OpenKN/image/ImageRGB.hpp>
#include <cstdio>
#include <cstdlib>
#define JPEG_OUT_WIDTH 800
#define JPEG_OUT_HEIGHT 600

inline void printMatrix(const kn::Matrix<double> & m) {
	for(size_t r = 0; r < m.rows(); ++r) {
		std::cout << " |";
		for (size_t c = 0; c < m.columns(); ++c)
			std::cout << "\t" << m[r][c] << "\t|";
		std::cout << std::endl;
	}
}

inline void printVector(const kn::Vector<double> & v, bool in_line = false) {
	if(in_line) {
		std::cout << "(";
		for(size_t c = 0; c < v.size(); ++c)
			std::cout << "\t" << v[c] << ",\t";
		std::cout << ")" << std::endl;
	} else
		for(size_t c = 0; c < v.size(); ++c)
			std::cout << " |\t" << v[c] << "\t|" << std::endl;
}

// @FIXME
inline void printPointsMapJpeg(const std::vector<kn::Vectord> & outputPoints, std::vector<Image*> & imgs) {
	kn::ImageRGB<unsigned char> img(JPEG_OUT_WIDTH, JPEG_OUT_HEIGHT);
	img.fill(255);
	
	for(size_t i = 0; i < outputPoints.size(); ++i)
		kn::drawCircle(img, outputPoints[i][0] * 100 + JPEG_OUT_WIDTH / 2, outputPoints[i][2] * 100 + JPEG_OUT_HEIGHT / 2, 4, 128, 255, 0);

	for(size_t i = 0; i < imgs.size(); ++i)
		kn::drawCircle(img, imgs[i]->pCamera->position[0] * 100 + JPEG_OUT_WIDTH / 2, imgs[i]->pCamera->position[2] * 100 + JPEG_OUT_HEIGHT / 2, 4, 255, 0, 0);

	if (!kn::saveJPG(img, "./out/points_map.jpg", 100))
		std::cerr << "Saving impossible !" << std::endl;
}

#endif // _PRINT_HPP_
