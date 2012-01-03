#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "Print.hpp"
#include <cstdlib>
#include <cmath>
#include <OpenKN/math/Vector.hpp>
// @TODO : Need to figure out how to use Matrix3x3d
#include <OpenKN/math/Matrix3x3.hpp>
// @TODO : Need to add debug code
#include <cassert>
#include <iostream>
#include <exception>

class Camera {
private :
	const size_t m_Id;
	const size_t m_Height;
	const size_t m_Width;
	const float m_Focale;
	kn::Matrix3x3d m_IntrinsecParameters;
	kn::Matrix3x3d m_Rotation;
	kn::Vector3d m_Center;
	kn::Matrix<double> m_Projection;
	
	void _setIntrinsecParameters();

public :
	Camera(size_t id, size_t height, size_t width);
	~Camera();
	
	void printIntrinsecParameters() const;
	
	// Getters
	inline const float focale() const {
		return m_Focale;
	}
	inline const size_t height() const {
		return m_Height;
	}
	inline const size_t width() const {
		return m_Width;
	}
	inline const kn::Matrix3x3d intrinsecParameters() const {
		return m_IntrinsecParameters;
	}
	inline const kn::Vector3d center() const {
		return m_Center;
	}
	
};

#endif // _CAMERA_HPP_
