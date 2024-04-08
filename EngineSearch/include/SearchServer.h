#pragma once
#include "InvertedIndex.h"
#include "RelativeIndex.h"
class SearchServer
{

	InvertedIndex* _index;

public:
	/**
	*
	* @param idx ссылка на класс InverterIndex
	* нужен, для того чтобы серчсервер мог знать частоту слов встречаемых в запросе
	* 
	*/
	SearchServer(InvertedIndex* idx) : _index(idx) { };

	/**
	* Методо обработки поисковыъ запросов
	* @param queries_input поисковые запросы взятые из файла requests.json
	* @return возвоащает отсортированный список релевантных ответов для заданных запросов
	*/

	std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queries_input);

};