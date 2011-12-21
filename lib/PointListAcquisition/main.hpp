/********************************/
/* author : Fabrice Boutarel    */
/********************************/


#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdio>
#include <cmath>

//openKN includes
#include <OpenKN/math/Vector.hpp>
#include <OpenKN/math/MathIO.hpp>
#include <OpenKN/image/ImageRGB.hpp>
#include <OpenKN/image/ioJPG.hpp>

#include "util.hpp"

static unsigned int windowwidth  = stdWidth;
static unsigned int windowheight = stdHeight;

static float windowratio = windowwidth/(float)windowheight;

std::string filename="";

GLuint lefttexid=0; 

float zoom = 1.;
//coordonnées de texture haut gauche.
float tx=0.0,ty=0.0;

//motion
bool motion=false;
int xMotion=0,yMotion=0;


kn::ImageRGB8u image;

//vector 3d
std::vector< kn::Vector3d > pList;
