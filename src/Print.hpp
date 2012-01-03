#ifndef _PRINT_HPP_
#define _PRINT_HPP_

#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/Matrix3x3.hpp>

inline void printMatrix(const kn::Matrix3x3d & m) {
	std::cout << " |\t" << m[0][0] << "\t\t" << m[0][1] << "\t\t" << m[0][2] << "\t|" << std::endl;
	std::cout << " |\t" << m[1][0] << "\t\t" << m[1][1] << "\t\t" << m[1][2] << "\t|" << std::endl;
	std::cout << " |\t" << m[2][0] << "\t\t" << m[2][1] << "\t\t" << m[2][2] << "\t|" << std::endl;
}

inline void printVector(const kn::Vector3d & v, bool in_line = false) {
	if(in_line) {
		std::cout << " (\t" << v[0] << ",\t" << v[1] << ",\t" << v[2] << "\t)" << std::endl;
	} else {
		std::cout << " |\t" << v[0] << "\t|" << std::endl;
		std::cout << " |\t" << v[1] << "\t|" << std::endl;
		std::cout << " |\t" << v[2] << "\t|" << std::endl;
	}
}

#endif // _PRINT_HPP_
