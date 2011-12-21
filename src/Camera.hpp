#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_


#include <cstdlib>
#include <cmath>
#include <OpenKN/math/Vector.hpp>
// @TODO : Need to figure out how to use Matrix3x3d
#include <OpenKN/math/Matrix.hpp>
// @TODO : Need to add debug code
#include <cassert>
#include <exception>

class Camera {
private :
	const size_t m_Height;
	const size_t m_Width;
	const double m_Focale;
	kn::Vector<double> m_Center;
	//kn::Matrix<double, 3, 3> m_IntrasecParameters;
	//Matrix3x3d m_ExtrasecParameters;
	//Matrix3x3d m_Rotation;
	
public :
	Camera(size_t height, size_t width)
		: 	m_Height(height), m_Width(width), m_Focale(sqrt(pow(m_Width, 2) + pow(m_Height, 2))),
			m_Center(double(m_Width/2.), double(m_Height/2.), 1.)
		{
		
	}
	~Camera() {}
	
	double focale() const {
		return m_Focale;
	}
	
	size_t height() const {
		return m_Height;
	}
	
	size_t width() const {
		return m_Width;
	}
	
	kn::Vector<double> center() const {
		return m_Center;
	}
};

#endif // _CAMERA_HPP_
