//
// Created by user on 13.02.2024.
//

#include "../include/ConverterJSON.h"
#include "nlohmann/json.hpp"



std::vector<std::string> ConverterJSON::GetTextDocument()
{
	std::string word;
	std::string tmp_str;
	std::vector<std::string> words_files; // ������ ������� ����� ����������
	// ��������� �� ������� ���� ������ �� �������
	std::ifstream config_prj(this->dir_prj + this->src_config); 
	json files = json::parse(config_prj);
	config_prj.close();

	for (std::string item_file : files["files"])
	{
		// �.�. ����������� ������� � Clion � ���������� ��� mingw �� � ������ ������ �������
		// � Visual Studio Comunity ����� ����������� �������� ����� � ������ ����
		// ������ ���� / �� \\

		for (auto& item_ch : item_file)
		{
			if (item_ch == '/') item_ch = '\\';
		}

		// ���������� ���� ��� ����� � ����� resources
		std::string link_file = this->dir_prj + item_file;
		std::ifstream read_file_res(link_file);

		// ��������� ������ �� ������ � ��������� ������ ����
		tmp_str = "";
		while (read_file_res >> word)
		{
			if (tmp_str.size() == 0) tmp_str += word;
			else tmp_str += (" " + word);
		}
		words_files.push_back(tmp_str);
		read_file_res.close();
	}

	return words_files;
}

int ConverterJSON::GetResponseLimit()
{
	// ��������� �� ������� ������������ ���-�� ��������
	std::ifstream config_prj(this->dir_prj + this->src_config);
	json count_response = json::parse(config_prj);
	return count_response["config"]["max_response"];
}

std::vector<std::string> ConverterJSON::GetRequest()
{
	// ��������� ������� �� ����� �������
	std::ifstream requests_prj(this->dir_prj + this->src_request);
	json requests = json::parse(requests_prj);
	return requests["requests"];
}

void ConverterJSON::putAnswer(std::vector<std::vector<std::pair<int, float>>> answer)
{

}