#pragma once
#include <string>
#include <algorithm>

/*
* Static ArgumentParser class
* Used to parse commandline arguments.
*/
class ArgumentParser
{
public:

	/*
	* Get commandline Option
	* Returns the location of the match, 0 otherwise.
	*/
	static char* getCmdOption(char **begin, char **end, const std::string &option) {
		char **itr = std::find(begin, end, option);
		if (itr != end && ++itr != end) {
			return *itr;
		}
		return 0;
	}

	/*
	* cmdOptionExists
	* Returns true if the option does exists, else false. 
	*/
	static bool cmdOptionExists(char** begin, char** end, const std::string& option) {
		return std::find(begin, end, option) != end;
	}

	/*
	* Read (signed) number argument to a variable.
	* Return number of read arguments (value between 0 - 2)
	*/
	template <typename T = double>
	static int readNumberArgument(char** begin, char** end, const std::string& option, T &variable) {
		if (cmdOptionExists(begin, end, option)) {
			string gsize = getCmdOption(begin, end, option);
			if (!gsize.empty()) {
				try {
					// TODO: Take care of negative numbers
					variable = (T)stod(gsize);
					return 2;
				}
				catch (exception e) {
					std::cerr << "PARSE ERROR: parameter value missing: " << option << endl;
				}
			}
			return 1;
		}
		return 0;
	}

};