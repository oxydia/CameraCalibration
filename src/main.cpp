#include "main.hpp"

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

const static int NB_MAX_ITERATIONS = 20;

// main
int main(int argc, char** argv) {
	// command : calibration -i p0.jpg p1.jpg -p p0.list p1.list
	cout << endl << "- Camera calibration. -" << endl << "Holy Razafinjoelina & Alexandre Mahé" << endl;
	cout << endl << "=============================" << endl << endl;
	
	// For stocking the images.
	vector<Image*> imgs;
	size_t iArg = 1;
	if(argc > 2 && !((string)argv[iArg]).compare("-i")) {
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
		// Not enough args, show the help.
		cout << "-= Help =-" << endl;
		cout << "This application is used to caliber multiple cameras using photographies." << endl;
		cout << "When launching this application, append the name of the application by -i and  the paths of two or more images." << endl;
		cout << "Ex: ./calibration -i path/photo1.jpg path/photo2.jpg" << endl << endl;
		cout << "Then append -p and the paths of the points lists associated with your images if you don't want to select your points with the integrated points selector." << endl;
		cout << "Ex: ./calibration -i path/photo1.jpg path/photo2.jpg -p path/points/photo1.list path/points/photo2.list" << endl;
		cout << "You will have to select some points on your images and close the selector." << endl;
		cout << "Commands for the selector :" << endl;
		cout << "\tf : Fullscreen" << endl;
		cout << "\tescape : Quit and save points in a file." << endl;
		exit(EXIT_SUCCESS);
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
		cout << "Press ESC when you are done selecting some points, identical from an image to another." << endl;
		
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
	
	size_t nbPoints = 0;
	// Now user configuration is done, we can compute data for every image.
	for(size_t i = 0; i < imgs.size(); ++i) {
		Image &img = *imgs[i];
		// The points are selected so we can load 'em
		img.loadPoints();
		// We make sure every image has the same number of points.
		if(nbPoints == 0)
			nbPoints = img.points.size();
		if(!nbPoints || img.points.size() != nbPoints) {
			cout << "not a good number of points" << endl;
			exit(EXIT_FAILURE);
		}
		// PRINT
		cout << "Image " << i+1 << " - Points vectors : " << endl;
		for(size_t vi = 0; vi < img.points.size(); ++vi)
			cout << img.points[vi] << endl;
		cout << endl;
		// Make camera
		img.setCamera();
		// Arbitrarly force the position
		img.pCamera->position[0] = (double)i;
		
		// Camera's intrinsec parameters
		// PRINT
		cout << "Image " << i+1 << " - Camera - Intrinsec parameters : " << endl << img.pCamera->intrinsecParameters() << endl;
	}
	
	cout << "-- Computing inversed transformations --" << endl;
	
	// will be filled up with rotations vectors
	kn::Vector<double> a(imgs.size() * 3);
	a.setZero();
	// filled up with cameras' parameters
	kn::Vector<double> b(4 + 2*imgs.size()*nbPoints);
	// Every image must have the same resolution, same center (x0, y0), same focale
	b[0] = imgs[0]->image.width();
	b[1] = imgs[0]->image.height();
	b[2] = imgs[0]->pCamera->focale;
	b[3] = nbPoints;
	for(size_t j = 0 ; j < nbPoints; ++j)
		for(size_t i = 0 ; i < imgs.size(); ++i)
			for(size_t c = 0; c < 2 ; ++c)
				b[4+j*(imgs.size()*2)+i*2+c] = imgs[i]->points[j][c];
				
	
	cout << endl << "Vector b : " << endl << b << endl;
	
	cout << "Non-linear system resolution :";
	// non linear resolution to find the best rotations
	nonLinearSystemSolver(a, b, &f, NB_MAX_ITERATIONS, imgs);
	cout << "done" << endl;
	
	cout << endl << "Vector a found : " << endl << a << endl;
	
	for(size_t i = 0; i < imgs.size(); ++i) {
		// Vector a[i*3] -> a[i*3+2] is the vector of the rotation that we need to compute transformation matrix.
		Image &img = *imgs[i];
		kn::Matrix3x3d rotation = kn::eulerAngles3x3d(a[i*2],a[i*2+1], a[i*2+2]);
		// PRINT
		cout << "Image " << i+1 << " - Camera - Found anti-transformation parameter : " << endl << rotation << endl;
		
		// We inverse this transformation (rotation) by transposing it and set it to the camera
		img.pCamera->rotation = rotation.transpose();
		// PRINT
		cout << "Image " << i+1 << " - Camera - Transformation (rotation) parameter : " << endl << img.pCamera->rotation << endl;
		
		// Reset center.
		kn::Vector3d oldPosition;
		oldPosition.setZero();
		oldPosition[0] = i;
		img.pCamera->center = - img.pCamera->rotation * oldPosition;
		
		// Find the actual homography of the camera.
		// PRINT
		cout << "Image " << i+1 << " - Camera - Actual homography : " << endl << img.pCamera->homography()  << endl;
		
		// Find the actual projection of the camera.
		img.pCamera->computeProjection();
		// PRINT
		cout << "Image " << i+1 << " - Camera - Projection : " << endl << img.pCamera->projection << endl;
	}
	
	// Triangulation
	vector< kn::Vector<double> > outputVectors(nbPoints);
	// For each point
	cout << "Triangulation 3D - Points vectors : " << endl;
	for(size_t i = 0; i < nbPoints; ++i) {
		outputVectors[i] = resolvePointTriangulation(i, imgs);
		// PRINT
		cout << "Point " << i << " : " << outputVectors[i] << endl;
	}
	cout << endl;
	
	// Export a list of points.
	kn::saveVectorList(outputVectors, "./out/3dpoints.list");
	
	// Export an image
	printPointsMapJpeg(outputVectors, imgs);
	
	// delete imgs
	while(!imgs.empty()) {
        delete imgs.back();
        imgs.pop_back();
    }
    
	cout << "That's done." << endl << "You will find a list of 3d points and a picture of your cameras in the directory out." << endl;
}

void printPointsMapJpeg(const std::vector<kn::Vectord> & outputPoints, std::vector<Image*> & imgs) {
	kn::ImageRGB<unsigned char> img(JPEG_OUT_WIDTH, JPEG_OUT_HEIGHT);
	img.fill(255);
	
	// @FIXME
	for(size_t i = 0; i < outputPoints.size(); ++i)
		kn::drawCircle(img, outputPoints[i][0] * 100 + JPEG_OUT_WIDTH / 2, outputPoints[i][2] * 100 + JPEG_OUT_HEIGHT / 2, 4, 0, 0, 255);

	for(size_t i = 0; i < imgs.size(); ++i)
		kn::drawCircle(img, imgs[i]->pCamera->center[0] * 100 + JPEG_OUT_WIDTH / 2, imgs[i]->pCamera->center[2] * 100 + JPEG_OUT_HEIGHT / 2, 4, 255, 0, 0);

	if (!kn::saveJPG(img, "./out/points_map.jpg", 100))
		std::cerr << "Saving impossible !" << std::endl;
}
