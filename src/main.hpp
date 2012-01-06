#include "Camera.hpp"
#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include "Image.hpp"
#include "Maths.hpp"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
#include <cassert>
#include <exception>
#include <OpenKN/image/DrawCircle.hpp>

#define JPEG_OUT_WIDTH 800
#define JPEG_OUT_HEIGHT 600
#define POINTLISTEXPORTER "lib/PointListAcquisition/pointListExporter"

int runCommand(const char *strCommand);
int main(int argc, char** argv);
void printPointsMapJpeg(const std::vector<kn::Vectord> & outputPoints, std::vector<Image*> & imgs);

#endif // _MAIN_HPP_
