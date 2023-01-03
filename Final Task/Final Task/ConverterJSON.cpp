#include "Header.h"

ConverterJSON::ConverterJSON() = default;

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
	if (read_good)
	{
		return obj_config.files;
	}
	else
	{
		json_read();
		return obj_config.files;
	}
}

int ConverterJSON::GetResponsesLimit()
{	
	if (read_good)
	{
		return obj_config.max_responses;
	}
	else
	{
		json_read();
		return obj_config.max_responses;
	}
}

std::vector<std::string> ConverterJSON::GetRequests()
{
	js requests;

	std::ifstream file("Json\\requests.json");

	if (!file)
	{
		cout << "File NO is open\n";
		return {};
	}

	file >> requests;

	file.close();

	requests["requests"].get_to<std::vector<std::string>>(requests_vec);

	return requests_vec;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
	if (!answers.empty())
	{
		cout << "The vector is empty" << endl;
		return;
	}
	
	std::ofstream file("Json\\answers.json", std::ios::out);

	if (!file)
	{
		cout << "File NO is open\n";
		return;
	}

	js answer_js;
	int count = 0;

	for (auto request : answers)
	{
		answer_js["answers"]["request" + std::to_string(count)]["result"] = !request.empty();

		if (request.size() == 1)
		{
			answer_js["answers"]["request" + std::to_string(count)]["docid"] = request[0].first;
			answer_js["answers"]["request" + std::to_string(count)]["rank"] = request[0].second;
		}
		else
		{
			auto obj_arr = js::array();
			int request_id = 0;

			for (auto relevance : request)
			{
				++request_id;
				if (request_id > obj_config.max_responses)
				{
					break;
				}

				auto obj_js = js::object();
				obj_js["docid"] = relevance.first;
				obj_js["rank"] = relevance.second;
				obj_arr.push_back(obj_js);
			}
			answer_js["answers"]["request" + std::to_string(count)]["relevance"] = obj_arr;
		}
		++count;
	}
	file << answer_js;
	file.close();
}

void ConverterJSON::json_read()
{
	js config;
	std::ifstream file("Json\\config.json");

	if (!file)
	{
		cout << "File NO is open\n";
		return;
	}
	file >> config;
	file.close();

	obj_config.name = config["config"]["name"].get<std::string>();
	obj_config.version = config["config"]["version"].get<double>();
	obj_config.max_responses = config["config"]["max_responses"].get<int>();
	config["files"].get_to<std::vector<std::string>>(obj_config.files);

	read_good = true;
}