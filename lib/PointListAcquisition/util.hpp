/********************************/
/* author : Fabrice Boutarel    */
/********************************/
#ifndef __UTIL_HPP_
#define __UTIL_HPP_

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glx.h>

#define  BACKSPACE    0x0008
#define  ENTER        0x000D
#define  ESCAPE       0x001B
#define  SPACE        0x0020
#define  DELETE       0x007F

#ifndef GLUT_WHEEL_UP
#define GLUT_WHEEL_UP 3
#endif
#ifndef GLUT_WHEEL_DOWN
#define GLUT_WHEEL_DOWN 4
#endif
#define DEBUG true

#define DTOR(X) X*2.*M_PI/360. //degree to radian

//camera resolution
static const unsigned int stdWidth  = 640;
static const unsigned int stdHeight = 480;

/*Fin de définition de la caméra*/
static GLenum err;

#endif
