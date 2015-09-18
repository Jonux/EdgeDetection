#pragma once
#include "CImg.h"
#include "Tools.h"
#include <vector>
#include <random>
#include <functional>

#define M_PI 3.1415926535897931384

/*
* Class to smooth the image
* Class uses Gaussian (Normal distribution) to smooth the image
*/
class Gaussian
{
	vector<vector<double> > gaussianArr;

public:

	Gaussian() { setMaskSize(5, 1.0);  }
	Gaussian(const int s, const double sigma) { setMaskSize(s, sigma); }

	// Normal distribution, probability density function, Norm(PDF)
	// TODO: Calculation operations could be optimised
	inline double normal_pdf(const double& x, const double& mu, const double& sigma) const {
		return 1.0 / sqrt(2 * M_PI) / sigma * exp(-((x - mu) * (x - mu)) / (sigma * sigma));
	}

	// Create Gaussian 2d mask
	// TODO: use log-distribution. More accurate results and more efficient operations to computer
	void setMaskSize(const int size, const double sigma) {
		Tools::resize2DVector(gaussianArr, size, size);

		// Calculate new mask
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {

				const double nX = normal_pdf(x, size / 2.0, sigma);
				const double nY = nX;	// symmetrical distribution

				// 2D normal_distribution can be constructed from 1D distributions:
				gaussianArr[y][x] = nX * nY;
			}
		}
	}


	/*
	* Apply the filterArr to the image
	* Border areas won't be modified, so if the mask is large check boders separently.
	*/
	CImg<uchar> filter_image(const CImg<uchar> &img, const vector<vector<double> > &filterArr) const {
		const int fsize = filterArr.size();
		const int fs = (int)std::floor(fsize / 2.0);

		return Tools::filter(img, [&](int x, int y) -> int {
			double sum = 0;
			for (int j = -fs; j < std::ceil(fsize / 2.0); j++) {
				for (int i = -fs; i < std::ceil(fsize / 2.0); i++) {
					if (x + i >= img.width() || y + j >= img.height() || x + i < 0 || y + j < 0) continue;
					sum += filterArr[j + fs][i + fs] * (double)img(x + i, y + j);
				}
			}
			return (int)sum;
		}, fs);
	}

	/*
	* Smooth the image with Gaussian mask
	*/
	CImg<uchar> filter_image(const CImg<uchar> &img) const {
		return filter_image(img, gaussianArr);
	}
};

