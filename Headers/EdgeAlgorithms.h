#pragma once

#include "CImg.h"
#include "Sobel.h"
#include "Canny.h"
#include "Tools.h"
#include "Gaussian.h"
#include "ArgumentParser.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>


/*
* EdgeAlgorithms class
* Handles calling of different algorithm methods
*/
class EdgeAlgorithms
{
private:
	unsigned int speedTestRounds;
	unsigned int width;
	unsigned int height;
	string outputFile;
	string inputFile;
	
	// Canny parameters
	int _gaussize;
	double _gaussigma;
	int _weakThreshold;
	int _strongThreshold;

protected:
	CImg<uchar> input_image;
	CImg<uchar> output_image;

public:

	// Set default values at constructor
	EdgeAlgorithms() : speedTestRounds(1), width(0), height(0), outputFile("output.bmp") {

		// Default mode is Canny
		edgeMode = EdgeMode::CANNY;

		// Default Canny parameters
		_gaussize = 5;
		_gaussigma = 0.5;
		_weakThreshold = 15;
		_strongThreshold = 30;
	}

	//Edge mode describes the commandline arguments executable progress
	enum EdgeMode {
		UNDEFINED,
		SOBEL,
		CANNY
	} edgeMode;

	// Convert mode to string, i.e. EdgeMode::SOBEL -> "Sobel"
	string edgeModeToString(const EdgeMode e) const;

	// set private parameters
	EdgeMode processArguments(int argc, char **argv);

	// Read Canny method arguments
	int readCannyParameters(int argc, char **argv);

	// Print given parameters
	void printInfo() const;

	// load image from file
	bool loadImage();
	
	// save image to outputfile
	bool saveImage() const;

	// return help-page
	string helpPage() const;

	// Shows the image on display
	void showImage(const CImg<uchar> &img, const string &title = "test");

	// Shows the output image
	void showImage() {
		showImage(output_image, "Result Image");
	}

	// call correct functions to create edges
	void perform() {
		int time_ms = 0;
		if (edgeMode == CANNY) {
			printBox("Canny Edge detection started!");
			time_ms = (int)Tools::Measure<>::execution([&]() {
				for (uint i = 0; i < speedTestRounds; i++) {
					Canny canny = Canny(_gaussize, _gaussigma, _weakThreshold, _strongThreshold);
					output_image = canny.perform(input_image);
				}
			});
		}
		else if (edgeMode == SOBEL) {
			printBox("Sobel Edge detection started!");
			time_ms = (int)Tools::Measure<>::execution([&]() { 
				vector<vector<double> > gradient_dir;
				for (uint i = 0; i < speedTestRounds; i++) {
					output_image = Sobel::sobelAlgorithm(input_image, gradient_dir, Sobel::EdgeStrengthMode::DIAGONAL);
				}
			});
		}
		else {
			printBox("UNKNOWN MODE !!!");
			return;
		}

		cout << "Calculation completed in " << time_ms << " milliseconds!" << endl << endl;
		printBox("Edge detection completed!");
	}

private:

	/*
	* Print box with text s.
	* s = input string, which lenght will be limited to (width - 4)
	* Mark = character to fill the box
	* Width = width of the element. Use odd numbers only, 
	* width i.e. 41, 43, ... [Width has to be greater than 4]
	*/
	void printBox(const string &s, const int width=43, const char mark='*') const {
		const string text = s.substr(0, max(width - 4, 0));
		const int len = text.length();
		cout << setw(width+2) << setfill(mark) << "\n";
		cout << mark << setw((int)ceil((width - len) / 2.0)) << " " << text << " " << setw((int)floor((width - len) / 2.0)-1) << mark << "\n";
		cout << setfill(mark) << setw(width+3) <<  "\n\n";
	}
};