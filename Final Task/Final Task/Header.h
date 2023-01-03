#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <map>

#include <set>
#include <sstream>
#include <cctype>
#include <algorithm>

#include "Struct_Json.h"
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

using std::cout;
using std::cin;
using std::endl;

using js = nlohmann::json;

namespace fs = std::filesystem;

extern std::vector<std::string> res_files;

//функция создаёт json файлы
void json_file();

void write_answers_json(const std::vector<std::vector<RelativeIndex>>& requests_results);