//
// Created by user on 13.02.2024.
//

#include "../src/ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocument()
{
	std::vector<std::string> data_files;
	std::ifstream config_prj(this->src_config);
	json files = json::parse(config_prj);
	for (auto item_file : files["files"])
	{
		data_files.push_back(item_file);
	}

	config_prj.close();
	return data_files;
}

int ConverterJSON::GetResponseLimit()
{
	std::ifstream config_prj(this->src_config);
	json count_response = json::parse(config_prj);
	return count_response["config"]["max_response"];
}

std::vector<std::string> ConverterJSON::GetRequest()
{
	std::ifstream requests_prj(this->src_request);
	json requests = json::parse(requests_prj);
	return requests["requests"];
}

void ConverterJSON::putAnswer(std::vector<std::vector<std::pair<int, float>>> answer)
{

}