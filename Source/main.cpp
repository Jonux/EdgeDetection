#include "EdgeAlgorithms.h"

int main(int argc, char **argv)
{
	EdgeAlgorithms program = EdgeAlgorithms();

	try {
		if (program.processArguments(argc, argv)) {
			program.printInfo();
			program.loadImage();
			program.perform();
			program.saveImage();
			program.showImage();
		}
	}
	catch (exception e) {
		// Rethrow all occurred exceptions in the EdgeAlgorithms-class
		throw e;
	}

#ifdef _WIN32
	system("pause"); // Prevent Windows to close console automatically
#endif

	return 0;
}


