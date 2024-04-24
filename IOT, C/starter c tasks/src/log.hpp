#pragma once
#include <string>
#include <fstream>

class log{
	private:
	public:
		log();
		~log();

		bool create_log(std::string filename);
		bool next();
		std::ifstream LogFile;
		std::string message;
		std::string line();
		std::string error();
		std::string reformat();
};
