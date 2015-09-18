#include "EdgeAlgorithms.h"



bool EdgeAlgorithms::saveImage() const {
	try {
		output_image.save_bmp(outputFile.c_str());
	}
	catch (exception e) {
		std::cerr << "Error writting the file:" << outputFile << std::endl;
		throw e;
	}
	return true;
}

bool EdgeAlgorithms::loadImage() {
	try {
		input_image.load(inputFile.c_str());
		width = input_image.width();
		height = input_image.height();
	}
	catch (exception e) {
		std::cerr << "INVALID FILE\n";
		throw e;
	}

	return true;
}

string EdgeAlgorithms::edgeModeToString(const EdgeMode e) const {
	if (e == EdgeMode::CANNY) return "Canny";
	if (e == EdgeMode::SOBEL) return "Sobel";
	return "Unknown";
}


void EdgeAlgorithms::showImage(const CImg<uchar> &img, const string &title) {
	const int disp_mode = 1; // Use Low level universal X11 display library

	CImgDisplay disp(img.width(), img.height(), title.c_str(), disp_mode);
	disp.render(img);

	while (!disp.is_closed()) {
		disp.wait();
	}
}


void EdgeAlgorithms::printInfo() const {

	cout << "********************************" << endl
		<< "*    EDGE DETECTION STARTED    *" << endl
		<< "********************************" << endl
		<< "Mode selected: " << edgeModeToString(edgeMode) << endl
		<< "Test rounds:   " << speedTestRounds << endl
		<< "Output file:   " << outputFile << endl
		<< "Input file:    " << inputFile << endl << endl;

	if (edgeMode == EdgeMode::CANNY) {
		cout << "********************************" << endl
			 << "** CANNY PARAMETERS:   " << endl
			 << "* Gaussian matrix size " << _gaussize << endl
			 << "* Gaussian sigma:      " << _gaussigma << endl
			 << "* Weak threshold:      " << _weakThreshold << endl
			 << "* Strong threshold:    " << _strongThreshold << endl
		<< "********************************" << endl << endl;
	}
}

/*
* Parse Canny edgedetection commandline parameters
*/
int EdgeAlgorithms::readCannyParameters(int argc, char **argv) {
	int arguments = 0;

	typedef ArgumentParser AP;
	arguments += AP::readNumberArgument<int>(argv, argv + argc, "--gaussize", _gaussize);
	arguments += AP::readNumberArgument<double>(argv, argv + argc, "--sigma", _gaussigma);
	arguments += AP::readNumberArgument<int>(argv, argv + argc, "--wt", _weakThreshold);
	arguments += AP::readNumberArgument<int>(argv, argv + argc, "--st", _strongThreshold);

	return arguments;
}

/*
* Parse commandline arguments
*/
EdgeAlgorithms::EdgeMode EdgeAlgorithms::processArguments(int argc, char **argv) {

	int arguments = 1;
	if (ArgumentParser::cmdOptionExists(argv, argv + argc, "--help")) {
		cout << helpPage() << endl;
		return EdgeMode::UNDEFINED;
	}

	if (ArgumentParser::cmdOptionExists(argv, argv + argc, "--mode")) {
		// Read mode and change it to lower case
		string mode = ArgumentParser::getCmdOption(argv, argv + argc, "--mode");
		std::transform(mode.begin(), mode.end(), mode.begin(), ::tolower);

		if (mode == "canny") {
			edgeMode = EdgeMode::CANNY;
			arguments += readCannyParameters(argc, argv);
		}
		else if (mode == "sobel") {
			edgeMode = EdgeMode::SOBEL;
		}
		else {
			cout << "Invalid Mode!\nOptions are: Sobel, Canny" << endl;
			return EdgeMode::UNDEFINED;
		}
		arguments += 2;
	}

	if (ArgumentParser::cmdOptionExists(argv, argv + argc, "--speedtest")) {
		const string rounds = ArgumentParser::getCmdOption(argv, argv + argc, "--speedtest");
		unsigned long n = stoul(rounds);
		if (!n || !rounds.length()) {
			cout << "Invalid speed rounds!\nGive positive number. i.e. --speedtest 10" << endl;
			return EdgeMode::UNDEFINED;
		}
		speedTestRounds = n;
		arguments += 2;
	}

	if (ArgumentParser::cmdOptionExists(argv, argv + argc, "--output")) {
		outputFile = ArgumentParser::getCmdOption(argv, argv + argc, "--output");
		if (!outputFile.length()) {
			cout << "Invalid output!\nGive output file. i.e. --output example.bmp" << endl;
			return EdgeMode::UNDEFINED;
		}
		arguments += 2;
	}

	// input file as last argument
	if (argc < arguments + 1) {
		cout << "Invalid input!\nGive input file. i.e. ./edge test.bmp" << endl;
		return EdgeMode::UNDEFINED;
	}
	inputFile = argv[argc - 1];

	return edgeMode;
}

string EdgeAlgorithms::helpPage() const {
	const string help = ""
		"Program usage: ./program <parameters> input_file\n\n"

		"(Required) Arguments\n"
		"input_file : i.e. file_name.bmp\n\n"

		"(Optional) Arguments\n"
		" --mode : Which algorithm are we using? Options are: Sobel, Canny\n"
		" --speedtest n: Run funktion n[1-1000] times and show cpu time\n"
		" --output : Output file name, i.e. output.bmp\n\n"

		"* Canny Mode's (optional) parameters\n"
		" --gaussize : Gaussian matrix size[1 - img_size], i.e. 5\n"
		" --sigma : Gassian sigma[0.001 - 10.0], i.e. 1.5\n"
		" --wt : Weak threshold[0 - 255], i.e. 10\n"
		" --st : Strong threshold[0 - 255], i.e. 20\n\n"

		"(Other) Arguments\n"
		" --help : Help page\n\n"

		"// Sobel Examples\n"
		"./program --mode sobel input.bmp\n"
		"./program --mode sobel --output test.bmp input.bmp\n"
		"./program --mode sobel --speedtest 5 --output test.bmp input.bmp\n\n"

		"// Canny Examples\n"
		"./program --mode canny --sigma 2.0 --wt 10 --st 20 input.bmp\n"
		"./program --mode canny --speedtest 5 --output test.bmp input.bmp\n"
		"./program --mode canny --gaussize 5 --sigma 2.0 --wt 10 --st 20 input.bmp\n"
		"./program --speedtest 10 --output alltest.bmp --mode canny --gaussize 5 --sigma 2.0 --wt 10 --st 20 input.bmp\n\n";
	return help;
}
