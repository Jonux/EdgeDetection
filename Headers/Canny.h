#pragma once
#include "CImg.h"
#include "Tools.h"
#include "Sobel.h"
#include "Gaussian.h"

#include <cmath>
#include <vector>
#include <random>
#include <functional>

#define M_PI 3.1415926535897931384

/*
* Canny class defines Canny-edgedetection method.
*/
class Canny
{
	int _gaussize;
	double _gaussigma;
	int _weakThreshold;
	int _strongThreshold;

	vector<vector<double> > gradient_dir;

public:

	// Default values
	Canny() : _gaussize(5), _gaussigma(0.5), _weakThreshold(20), _strongThreshold(35) {}

	
	// Canny recommended a upper:lower ratio between 2:1 and 3:1.
	// As gaussiam matrix is often used 5x5 and sigma between 0.2 - 2.0. 
	// Lower sigma = sharper image
	Canny(int size, double sig, int wt, int ht) : _gaussize(size), _gaussigma(sig), _weakThreshold(wt), _strongThreshold(ht) { }


	/*
	* Performs the image edgedetection with Canny detector method.
	*/
	CImg<uchar> perform(const CImg<uchar> &img)
	{
		// 1. Filter out noise
		Gaussian gaussian = Gaussian(_gaussize, _gaussigma);
		CImg<uchar> smooth_img = gaussian.filter_image(img);

		// 2.  intensity gradient of the image
		CImg<uchar> sobel_img = create_intensity_gradient(smooth_img);

		// 3. non-maximum suppression
		CImg<uchar> supp_img = nonMaximumSuppression(sobel_img);

		// 4. Thresholding
		CImg<uchar> final_img = threshold_image(supp_img, _weakThreshold, _strongThreshold);

		return final_img;
	}

	/*
	* Function makes the image 2-colored.
	* - Removes points which are lower than weakThreshold.
	* - Rounds points to grayValue which are over or equal to strongThreshold.
	* - Removes points which are over weakThreshold but under strongThreshold if they don't have neighbour which is strong pixel.
	*/
	CImg<uchar> threshold_image(const CImg<uchar> &img, const int weakThreshold, const int strongThreshold, const uchar grayValue=255) {
		return Tools::filter(img, [&](int x, int y) -> int {

			if (img(x, y) >= strongThreshold) {
				return grayValue;
			}
			else if (img(x, y) >= weakThreshold) {
				const int plusX[] = { 1,1,1,0,0,-1,-1,-1 };
				const int plusY[] = { 0,-1,1,-1,1,-1,0,1 };
				for (int i = 0; i < 8; i++) {
					const int px = plusX[i] + x;
					const int py = plusY[i] + y;
					if (px < 0 || py < 0 || px >= img.width() || py >= img.height()) continue;
					if (img(px, py) >= strongThreshold) {
						return grayValue;
					}
				}
			}
			return 0;
		});
	}


	/*
	* nonMaximumSuppression
	* Function removes pixels which are weaker than the gradiet shown neighbours.
	* i.e. if gradient is up, and the UP or Down pixel is stronger than middle, middle pixel gets removed.
	*/
	CImg<uchar> nonMaximumSuppression(const CImg<uchar> &img) {
		CImg<uchar> fimg = Tools::filter(img, [&](int x, int y) -> int {
			const double ang = roundAngleTo(gradient_dir[y][x]);

			const uchar front = (int)getNeighbourPixel(img, x, y, ang);
			const uchar back = (int)getNeighbourPixel(img, x, y, -ang);

			if (front > img(x, y) || back > img(x, y)) {
				return 0;
			}

			return img(x, y);
		});
		
		return fimg;
	}

	/*
	* Intensity gradient
	* Calculates the stregth of changes in the picture and the direction of the gradients.
	*/
	CImg<uchar> create_intensity_gradient(const CImg<uchar> &img) {
		CImg<uchar> sobel_img = Sobel::sobelAlgorithm(img, gradient_dir, Sobel::EdgeStrengthMode::DIAGONAL);
		return sobel_img;
	}

private:

	// Function gets the neighbour from the given direction. 
	// Ang = 0 is Right, Pi/4 is up, etc.
	uchar getNeighbourPixel(const CImg<uchar> &img, const int x, const int y, const double ang) const {
		const int nx = x + (int)round(cos(ang));
		const int ny = y - (int)round(sin(ang));

		// neighbour out of area, return current pixel
		if (nx < 0 || ny < 0 || nx >= img.width() || ny >= img.height()) {
			return img(x, y);
		}
		return img(nx, ny);
	}

	// Rounds angle to 45 deg angles
	// May change angle direction to opposite
	double roundAngleTo(const double ang) const {
		const double a = fabs(ang);
		if (a < M_PI / 8.0) {
			return 0;
		}
		else if (a < M_PI * 3.0 / 8.0) {
			return M_PI / 4.0;
		}
		else if (a < M_PI * 5.0 / 8.0) {
			return M_PI / 2.0;
		}
		else if (a < M_PI * 7.0 / 8.0) {
			return 3.0 * M_PI / 4.0;
		}
		return 0;
	}
};