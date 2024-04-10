#pragma once
#include "InvertedIndex.h"
#include "RelativeIndex.h"
class SearchServer
{

	InvertedIndex* _index;

public:
	/**
	*
	* @param idx ������ �� ����� InverterIndex
	* �����, ��� ���� ����� ���������� ��� ����� ������� ���� ����������� � �������
	* 
	*/
	SearchServer(InvertedIndex* idx) : _index(idx) { };

	/**
	* ������ ��������� ��������� ��������
	* @param queries_input ��������� ������� ������ �� ����� requests.json
	* @return ���������� ��������������� ������ ����������� ������� ��� �������� ��������
	*/

	std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queries_input);

};