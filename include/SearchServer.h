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
	* @param idx ������ �� ����� InverterIndex
	* �����, ��� ���� ����� ���������� ��� ����� ������� ���� ����������� � �������
	* 
	*/
	SearchServer(InvertedIndex* idx) : _index(idx) { };
    /**
	* ������� ����� ������ � �������� "response_limit" ������ ����������� ���������� �� ������ ������
	* @param in_limit ����� �������� ������ �� ����� config.json
	* @return ������ �� ����������
	*/
    void setResponseLimit(int in_limit);

	/**
	* ������ ��������� ��������� ��������
	* @param queries_input ��������� ������� ������ �� ����� requests.json
	* @return ���������� ��������������� ������ ����������� ������� ��� �������� ��������
	*/

	std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

};