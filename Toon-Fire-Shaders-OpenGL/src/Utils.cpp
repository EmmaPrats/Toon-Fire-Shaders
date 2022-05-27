//Include our classes
#include "Utils.h"
#include <sstream>


std::vector<std::string> splitString(std::string aString, char separationCharacter){
	std::stringstream test(aString);
	std::string segment;
	std::vector<std::string> seglist;

	while(std::getline(test, segment, separationCharacter))
	{
	   seglist.push_back(segment);
	}
	return seglist;
}

