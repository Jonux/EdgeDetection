#pragma once
#include "CImg.h"
#include "Tools.h"
#include <vector>

/*
* Apply Sobel Edge detection method to the image
* Method uses 3x3 matrixes to calculate derivates
*/
class Sobel
{
	//Sobel Matrices Horizontal
	static int const GX[4][4]; 

	//Sobel Matrices Vertical
	static int const GY[4][4]; 

public:

	// Possible Edge strength modes, Diagonal or Block wise
	enum EdgeStrengthMode { UNDEF, DIAGONAL, BLOCK };

	// Perform Sobel algorithm to the image
	// function creates gradient_dir, which includes all gradient directions in radians. [i.e RIGHT = 0 rad, UP = PI/4 rads]
	// EdgeStrengthMode is by default DIAGONAL (optimal), but to fast up calculations BLOCK-mode can be used [approximates the results]
	static CImg<uchar> sobelAlgorithm(const CImg<uchar> &image, vector<vector<double> > &gradient_dir, const EdgeStrengthMode strMode = EdgeStrengthMode::DIAGONAL);

};
