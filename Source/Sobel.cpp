#include "Sobel.h"
#include "Tools.h"
#include <algorithm>
#include <cmath>

#define MY_PI 3.14159265358979323

// Sobel Matrices Horizontal
int const Sobel::GX[4][4] = { 
	{ 1, 0, -1, 0 },
	{ 2, 0, -2, 0 },
	{ 1, 0, -1, 0 },
	{ 0, 0,  0, 0 }
};

// Sobel Matrices Vertical
int const Sobel::GY[4][4] = {
	{ 1,  2,  1, 0 },
	{ 0,  0,  0, 0 },
	{-1, -2, -1, 0 },
	{ 0,  0,  0, 0 }
};


CImg<uchar> Sobel::sobelAlgorithm(const CImg<uchar> &image, vector<vector<double> > &gradient_dir, const EdgeStrengthMode strMode) {
	const int width = image.width();
	const int height = image.height();

	Tools::resize2DVector<double>(gradient_dir, width + 1, height + 1);

	// Edge detection using Sobel Algorithm
	return Tools::filter(image, [&](int x, int y) -> int {
		int sumX = 0, sumY = 0;

		// Convolution for X
		for (int j = -1; j<3; j++) {
			for (int i = -1; i<3; i++) {
				if (x + i >= width || y + j >= height || x + i < 0 || y + i < 0) continue;
				sumX = sumX + GX[j + 1][i + 1] * (int)image(x + i, y + j);
			}
		}
		// Convolution for Y
		for (int j = -1; j< 3; j++) {
			for (int i = -1; i< 3; i++) {
				if (x + i >= width || y + j >= height || x + i < 0 || y + i < 0) continue;
				sumY = sumY + GY[j + 1][i + 1] * (int)image(x + i, y + j);
			}
		}

		// Edge strength
		int sum;
		if (strMode == EdgeStrengthMode::DIAGONAL) {
			sum = (int)(sqrt(pow((double)sumX, 2) + pow((double)sumY, 2)) + 0.5);
		}
		else { 
			sum = sumX + sumY; // Approximate distance
		}

		// Gradient direction in radians
		gradient_dir[y][x] = (sumX == 0) ? MY_PI : atan2(sumY, sumX);

		return min(max(sum, 0), 255);
	}, 1);
}