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
		// � ������ � Visual Studio Comunity ����� ����������� �������� ����� � ������ ����
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


// � ��� ���������� ����� �������������� � json )) � ���������� ����������� � answers.json
void ConverterJSON::PutAnswer(std::vector<std::vector<std::pair<int, float>>> answer)
{
	json answers = { };		// ��� ��������� ������� ����� ���������� � answers.json
	for (int i = 0; i < answer.size(); i++)
	{
		auto& request = answer[i];
		std::string req_id = "request_";
		if (i >= 0 && i < 10) req_id += ("00" + std::to_string(i + 1));
		else if (i >= 10 && i <= 99) req_id += ("0" + std::to_string(i + 1));
		else if (i >= 99 && i <= 999) req_id += ("" + std::to_string(i + 1));

		// ��� ����������� ���� ������� ��������� ��� ��� � ����������� � ���� result (false ��� true)

		if (request.size() == 0) answers["answers"][req_id]["result"] = "false";
		else answers["answers"][req_id]["result"] = "true";

		// ���� ������� ������ ������ ���������
		// �� ������� ���� relative

		if (request.size() > 1)
		{
			for (auto& result_doc : request)
			{
				answers["answers"][req_id]["relevance"].push_back({ {"docid", result_doc.first} });
				answers["answers"][req_id]["relevance"].push_back({ {"rank", result_doc.second} });
				
				long int calc_result_rank = result_doc.second * 1000000;
				size_t doc_position = answers["answers"][req_id]["relevance"].size();
				json* edit_rank = &answers["answers"][req_id]["relevance"][doc_position - 1]["rank"];
				double add_rank = *edit_rank;
				// �������� � �0������ ���. ����� 1 ���� �� 1.0
				if (calc_result_rank > 999999) *edit_rank = static_cast<int>(*edit_rank);
				// �������� �� 6 ������ ����� �������
				else answers["answers"][req_id]["relevance"][doc_position - 1]["rank"] = std::trunc(add_rank * 1000000000) / 1000000000.0000000000f; ;
			}
		}
		// ���� ����� ���� �������� �� ������ ��������� � ������ � ��������� �������
		else if (request.size() == 1)
		{
			for (auto one_doc : request)
			{
				answers["answers"][req_id]["docid"] = one_doc.first;
				answers["answers"][req_id]["rank"] = one_doc.second;

			}
		}

			
	}
								
	std::ofstream input_answers("C:\\cpp\\EngineSearch\\EngineSearch\\configs\\answers.json", std::ios::out);
	if (input_answers.is_open()) input_answers << answers;
	input_answers.close();
}