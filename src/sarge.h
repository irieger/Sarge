/*
	sarge.h - Header file for the Sarge command line argument parser project.

	Revision 0

	Notes:
			-

	2019/03/16, Maya Posch

*/


#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>


// Function via https://stackoverflow.com/questions/1243428/convert-string-to-int-with-bool-fail-in-c
template <typename T>
T lexical_cast(const std::string& s)
{
    std::stringstream ss(s);

    T result;
    if ((ss >> result).fail() || !(ss >> std::ws).eof())
    {
        throw std::bad_cast();
    }

    return result;
}



struct Argument {
	Argument() : hasValue(false), parsed(false) { /* */ }
	std::string arg_short;
	std::string arg_long;
	std::string description;
	bool hasValue;
	std::string value;
	bool parsed;
};



class Sarge {
	std::vector<std::unique_ptr<Argument> > args;
	std::map<std::string, Argument*> argNames;
	bool parsed = false;
	int flagCounter = 0;
	std::string execName;
	std::string description;
	std::string usage;

public:
	void setArgument(std::string arg_short, std::string arg_long, std::string desc, bool hasVal);
	void setArguments(std::vector<Argument> args);
	void setDescription(std::string desc) { this->description = desc; }
	void setUsage(std::string use) { this->usage = use; }
	bool parseArguments(int argc, char** argv);

	bool getFlag(std::string arg_flag, std::string &arg_value);

	template<typename T>
	bool getFlag(std::string arg_flag, T &arg_value) {
		if (!parsed) { return false; }
		std::map<std::string, Argument*>::const_iterator it = argNames.find(arg_flag);
		if (it == argNames.end()) { return false; }
		if (!it->second->parsed) { return false; }
		if (it->second->hasValue) {
			arg_value = lexical_cast<int32_t>(it->second->value);
		}
		return true;
	}

	bool exists(std::string arg_flag);
	void printHelp();
	int flagCount() { return flagCounter; }
	std::string executableName() { return execName; }
};
