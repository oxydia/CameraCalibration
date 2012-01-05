#ifndef _PRINT_HPP_
#define _PRINT_HPP_

#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/Matrix3x3.hpp>

inline void printMatrix(const kn::Matrix<double> & m) {
	for(size_t r = 0; r < m.rows(); ++r) {
		std::cout << " |";
		for (size_t c = 0; c < m.columns(); ++c)
			std::cout << "\t" << m[r][c] << "\t|";
		std::cout << std::endl;
	}
}

inline void printVector(const kn::Vector<double> & v, bool in_line = false) {
	// @TODO : debug
	if(in_line) {
		std::cout << "(";
		for(size_t c = 0; c < v.size(); ++c)
			std::cout << "\t" << v[c] << ",\t";
		std::cout << ")" << std::endl;
	} else
		for(size_t c = 0; c < v.size(); ++c)
			std::cout << " |\t" << v[c] << "\t|" << std::endl;
}

#endif // _PRINT_HPP_
