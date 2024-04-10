//
// Created by user on 13.02.2024.
//

#include "../include/ConverterJSON.h"
#include "nlohmann/json.hpp"



std::vector<std::string> ConverterJSON::GetTextDocument()
{
	std::string word;
	std::string tmp_str;
	std::vector<std::string> words_files; // список который будем возвращать
	// считываем из конфига пути файлов со словами
	json files;
	std::ifstream config_prj(this->dir_prj + this->src_config); 
	if (config_prj.is_open())
	{
		files = json::parse(config_prj);
		config_prj.close();
		for (std::string current_file : files["files"])
		{

			// составляем путь для файла в папке resources
			std::ifstream read_file_res(this->dir_prj + this->src_request);
			if (read_file_res.is_open())
			{
				// считываем данные из файлов и заполняем список слов
				tmp_str = "";
				while (read_file_res >> word)
				{
					if (tmp_str.size() == 0) tmp_str += word;
					else tmp_str += (" " + word);
				}
				words_files.push_back(tmp_str);
				read_file_res.close();
			}
			else
				throw ExeptNotFoundFile(this->src_request, this->dir_prj, 77, "ConfigJson");

			
		}
		return words_files;
	}
	else
		throw ExeptNotFoundFile(this->src_config, this->dir_prj, 84, "ConfigJson");
	return words_files;
}

int ConverterJSON::GetResponseLimit()
{
	json count_response;
	// считываем из конфига максимальное кол-во запросов
	std::ifstream config_prj(this->dir_prj + this->src_config);
	if (config_prj.is_open())
	{
		count_response = json::parse(config_prj);
		return count_response["config"]["max_response"];
	}
	else
		throw ExeptNotFoundFile(this->src_config, this->dir_prj, 99, "ConfigJson");
	return 0;
}

std::vector<std::string> ConverterJSON::GetRequest()
{
	std::vector<std::string> requests;
	// считываем запросы из файла реквест
	std::ifstream requests_prj(this->dir_prj + this->src_request);
	if (requests_prj.is_open())
	{
		for (std::string current_req : json::parse(requests_prj)["requests"])
			requests.push_back(current_req);
	}
	else
		throw ExeptNotFoundFile(this->src_request, this->dir_prj, 114, "ConfigJson");

	return requests;
}


// а тут происходит магия перевоплащения в json )) и сохранение результатов в answers.json
void ConverterJSON::PutAnswer(std::vector<std::vector<std::pair<int, float>>> answer)
{
	json answers = { };		// это результат который будем записывать в answers.json
	for (int i = 0; i < answer.size(); i++)
	{
		auto& request = answer[i];
		std::string req_id = "request_";
		if (i >= 0 && i < 10) req_id += ("00" + std::to_string(i + 1));
		else if (i >= 10 && i <= 99) req_id += ("0" + std::to_string(i + 1));
		else if (i >= 99 && i <= 999) req_id += ("" + std::to_string(i + 1));

		// тут определеяем если найдены документы или нет и записывапем в поле result (false или true)

		if (request.size() == 0) answers["answers"][req_id]["result"] = "false";
		else answers["answers"][req_id]["result"] = "true";

		// если найдено больше одного документа
		// то создаем лист relative

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
				// приводим в б0жеский вид. чтобы 1 была не 1.0
				if (calc_result_rank > 999999) *edit_rank = static_cast<int>(*edit_rank);
				// обрезаем до 6 знаков после запятой
				else answers["answers"][req_id]["relevance"][doc_position - 1]["rank"] = std::trunc(add_rank * 1000000000) / 1000000000.0000000000f; ;
			}
		}
		// если всего один документ то просто добавляем в массив с названием запроса
		else if (request.size() == 1)
		{
			for (auto one_doc : request)
			{
				answers["answers"][req_id]["docid"] = one_doc.first;
				answers["answers"][req_id]["rank"] = one_doc.second;

			}
		}

			
	}
								
	std::ofstream input_answers(this->dir_prj + src_answers, std::ios::out);
	if (input_answers.is_open()) input_answers << answers;
	else
		throw ExeptNotFoundFile(this->src_answers, this->dir_prj, 174, "ConfigJson");
	input_answers.close();
}