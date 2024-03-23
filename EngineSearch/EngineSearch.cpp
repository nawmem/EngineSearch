﻿
// EngineSearch.cpp: определяет точку входа для приложения.
//

#include "EngineSearch.h"
#include <nlohmann/json.hpp>
#include "src/InvertedIndex.h"
#include "src/ConverterJSON.h"

using json = nlohmann::json;

using namespace std;

int main()
{
	ConverterJSON config_file = ConverterJSON();
	//for (auto& item : config_file.GetRequest())
	//{
	//	std::cout << item << std::endl;
	//}
	/*for (std::string item_str : config_file.GetTextDocument())
	{
		std::cout << item_str << std::endl;
	}*/
	std::vector<std::string> strww = config_file.GetTextDocument();
	for (std::string strww_i : strww)
	{
		std::cout << strww_i << std::endl;
	}
	

	InvertedIndex ii = InvertedIndex();
	std::string word = "dda";
	std::vector<std::string> words = { "Error1", "erroor1", "error1" };
	ii.UpdateDocumentBase(strww);
	std::vector<Entry> entrys = ii.GetWordCount(word);
	for (auto item_entry : entrys)
	{
		std::cout << item_entry.doc_id << " - " << item_entry.count << std::endl;
	}
	return 0;
}
