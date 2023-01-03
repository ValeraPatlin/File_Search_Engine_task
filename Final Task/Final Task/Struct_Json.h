#pragma once
#include "Header.h"

struct Struct_Json_config
{
	std::string name = "file search";
	double version = 0.1;
	int max_responses = 5;
	std::vector < std::string> files;
};
