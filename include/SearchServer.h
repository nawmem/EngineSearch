#pragma once
#include "InvertedIndex.h"
#include "RelativeIndex.h"
class SearchServer
{

	InvertedIndex* _index;
    int response_limit = 0;

public:
	/**
	*
	* @param idx ссылка на класс InverterIndex
	* нужен, дл€ того чтобы серчсервер мог знать частоту слов встречаемых в запросе
	* 
	*/
	SearchServer(InvertedIndex* idx) : _index(idx) { };
    /**
	* ƒобавил метод записи в свойство "response_limit" лимита колличества документов на каждый запрос
	* @param in_limit лимит запросов вз€тый из файла config.json
	* @return ничего не возвращает
	*/
    void setResponseLimit(int in_limit);

	/**
	* ћетодо обработки поисковыъ запросов
	* @param queries_input поисковые запросы вз€тые из файла requests.json
	* @return возвоащает отсортированный список релевантных ответов дл€ заданных запросов
	*/

	std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

};