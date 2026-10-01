#include "main.h"

#include "preprocess.h"

int main (int argc, char* argv[]) {
	PreProcessor sample;

	if (argc < 2) {
		std::cout << "preprocessor <filename> OPTIONAL=<outputfilename>" << std::endl;
	}
	else if (argc == 2) {
		//process file as input and output the same filename, appended with PP
		std::string outputFileName = argv[1];
		outputFileName = outputFileName.substr(0, outputFileName.find_last_of('.'));
		outputFileName += "PP.txt"; //PreProcessed.txt
		sample.processFile(argv[1], outputFileName);
	}
	else if (argc == 3) {
		sample.processFile(argv[1], argv[2]);
	}
	else {
		std::cout << "preprocessor <filename> OPTIONAL=<outputfilename>" << std::endl;
	}


	return 0;
}
