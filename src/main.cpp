#include "Camera.hpp"
#include "Image.hpp"
#include "Print.hpp"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>
// @TODO : Need to add debug code
#include <cassert>
#include <exception>
#define POINTLISTEXPORTER "lib/PointListAcquisition/pointListExporter"

using namespace std;

// executing commands from da shell
int runCommand(const char *strCommand)
{
	int iForkId, iStatus;
	iForkId = vfork();
	if (iForkId == 0) {
		iStatus = execl("/bin/sh","sh","-c", strCommand, (char*) NULL);
		exit(iStatus);
	} else
		iStatus = (iForkId > 0) ? 0 : -1;
	return(iStatus);
}

// main
int main(int argc, char** argv) {
	// command : cameraCalibration -i p0.jpg p1.jpg -p p0.list p1.list
	cout << endl << "- Camera calibration. -" << endl << "Holy Razafinjoelina & Alexandre Mahé" << endl;
	cout << endl << "=============================" << endl << endl;
	
	if(argc <= 1) {
		// Not enough args, show the help.
		cout << "-= Help =-" << endl;
		cout << "This application is used to caliber multiple cameras using photographies." << endl;
		cout << "When launching this application, append the name of the application by -i and  the paths of two or more images." << endl;
		cout << "Ex: ./cameraCalibration -i path/photo1.jpg path/photo2.jpg" << endl << endl;
		cout << "Then append -p and the paths of the points lists associated with your images if you don't want to select your points with the integrated points selector." << endl;
		cout << "Ex: ./cameraCalibration -i path/photo1.jpg path/photo2.jpg -p path/points/photo1.list path/points/photo1.list" << endl;
		cout << "You will ask to select 6 points on your images and close the selector." << endl;
		cout << "Commands for the selector :" << endl;
		cout << "\tf : Fullscreen" << endl;
		cout << "\tescape : Quit and save points in a file." << endl;
		exit(EXIT_SUCCESS);
	}
	// else
	// For stocking the images.
	std::vector<Image*> imgs;
	size_t iArg = 1;
	if(!((string)argv[iArg]).compare("-i")) {
		++iArg;
		const int j(iArg);
		while(argv[iArg] && ((string)argv[iArg]).compare("-p")) {
			imgs.push_back(new Image());
			imgs[iArg-j]->path = (string)argv[iArg];
			try {
				imgs[iArg-j]->loadJPG();
			} catch(...) {
				cerr << "Unable to load the file at " << imgs[iArg-j]->path << endl;
				cerr << "Aborting." << endl;
				exit(EXIT_FAILURE);
			}
			++iArg;
		}
	} else {
		throw "no args !";
		exit(EXIT_FAILURE);
	}
	
	// If pointlists paths are set in the application command line
	if(!!argv[iArg] && !((string)argv[iArg]).compare("-p")) {
		++iArg;
		const int j(iArg);
		while(argv[iArg]) {
			imgs[iArg-j]->pointlistPath = (string)argv[iArg];
			++iArg;
		}
		if(iArg-j != imgs.size())
			throw "Not enough points files";
	}
	// If not
	else {
		// For executing commands in shell.
		int iNumProc = 0, iChildiStatus = 0, iStatus = 0, iDeadId = 0, iExitFlag = 0;
		for(size_t i = 0; i < imgs.size(); ++i) {
			stringstream imgCommand;
			// assert on a property of the image (like dimensions)
			imgCommand << POINTLISTEXPORTER << " " << imgs[i]->path;
			iStatus = runCommand(imgCommand.str().c_str());
			if (!iStatus)
				iNumProc++;
		}
		cout << "Press ESC when you are done selecting the 6 points." << endl;
		
		// Wait till the commands complete
		while (iNumProc && !iExitFlag)
		{
			iDeadId = waitpid(-1, &iChildiStatus, WNOHANG);
			if (iDeadId < 0)
				iExitFlag = 1;
			else if (iDeadId > 0)
				--iNumProc;
			else
				sleep(3);
		}
		
		for(size_t i = 0; i < imgs.size(); ++i) {		
			std::stringstream pointsFilename;
			pointsFilename << imgs[i]->path << ".list";
			imgs[i]->pointlistPath = pointsFilename.str();
		}
	}
	
	cout << "Computing results..." << endl;
	
	// Now user configuration is done, we can compute data for every image.
	for(size_t i = 0; i < imgs.size(); ++i) {
		Image &img = *imgs[i];
		
		// The points are selected so we can load 'em
		img.loadPoints();
		// PRINT
		cout << "Image " << i+1 << " - Points vectors : " << endl;
		for(size_t vi = 0; vi < img.points.size(); ++vi)
			printVector(img.points[vi], true);
		cout << endl;
		
		// Make camera
		img.setCamera();
		// Arbitrarly force the position
		img.pCamera->position[0] = (double)i;
		
		// Camera's intrinsec parameters
		img.pCamera->computeIntrinsecParameters();
		// PRINT
		cout << "Image " << i+1 << " - Camera - Intrinsec parameters : " << endl;
		printMatrix(img.pCamera->intrinsecParameters);
		cout << endl;
		
		// The homography is used for finding a rotation
		img.homography = img.resolveHomography();
		// PRINT
		cout << "Image " << i+1 << " - Theorical Homography : " << endl;
		printMatrix(img.homography);
		cout << endl;
		
		// Make a fake camera for describing camera projection ?
		
		// @TODO : Insert strange parameters and compute 'em.
	}
	
	// Now we need to create fake cameras.
	
	// Build an image
	
	cout << "That's done." << endl;
	
	// Print coordinates	
}
