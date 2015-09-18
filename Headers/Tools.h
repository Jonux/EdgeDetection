#pragma once
#include "CImg.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <functional>

// All other files includes namespaces here
using namespace std;
using cimg_library::CImg;
using cimg_library::CImgDisplay;

typedef unsigned char uchar;
typedef unsigned int uint;


/*
* Defines useful tools 
*/
class Tools
{
public:

	// Filter to apply function to every cell in 2D array
	static CImg<uchar> filter(const CImg<uchar> &img, std::function<int(int, int)> func, const int padding = 1) {
		CImg<uchar> fimg(img.width(), img.height());
		for (int y = padding; y < img.height() - padding; y++) {
			for (int x = padding; x < img.width() - padding; x++) {
				fimg(x, y) = func(x, y);
			}
		}
		return fimg;
	}

	// Apply function to every pixel in the image
	static void checkPixels(const CImg<uchar> &img, std::function<void(int, int)> func, const int padding = 1) {
		for (int y = padding; y < img.height() - padding; y++) {
			for (int x = padding; x < img.width() - padding; x++) {
				func(x, y);
			}
		}
	}

	// Resize 2D array
	template<typename T>
	static void resize2DVector(std::vector<std::vector<T> > &v, const int w, const int h) {

		// Allocate and update 2D array
		// reserve will allocate the memory but will not resize your vector, which will have a logical size the same as it was before.
		// resize will modify the size of your vector and will fill any space with objects in their default state
		v.reserve(h);
		v.resize(h);
		for (int k = 0; k < h; ++k) {
			v[k].reserve(w);
			v[k].resize(w);
		}
	}

	// Measure time of a function
	template<typename TimeT = std::chrono::milliseconds>
	struct Measure {
		// Takes unlimited amount of arguments
		template<typename F, typename ...Args>
		static typename TimeT::rep execution(F func, Args&&... args) {
			auto start = std::chrono::system_clock::now();

			func(std::forward<Args>(args)...);

			auto duration = std::chrono::duration_cast<TimeT>(std::chrono::system_clock::now() - start);
			return duration.count();
		}
	};
};