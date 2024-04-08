#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <nlohmann/json.hpp>
#include "../include/SearchServer.h"
#include "../include/RelativeIndex.h"
#include "../include/Entry.h"

using json = nlohmann::json;

int sortListWordAsc
(
	std::map<std::string, std::vector<Entry>>& freq_dict, 
	std::vector<std::string>& sort_list_word, 
	std::string& insert_word, 
	int start, 
	int end
)
{
	int start_pos = start;
	int end_pos = end;
	int middle = (end - start) / 2 + start;
	int count_find_word = 0;			// ����������� ��� ����� ����������� ����� ����������� � ����������
	int count_freq_word = 0;			// ����������� ��� ����� ��������� ����� ����������� � ����������
	int count_freq_word_middle = 0;		// ����������� ��� ����� ����� �� �������� ����������� � ����������
	for (auto current_word : freq_dict[insert_word]) count_find_word += static_cast<int>(current_word.count);
	
	// ���� ������ ������ �� ����� � ���� ������ �������
	if (sort_list_word.size() == 0)
	{
		sort_list_word.push_back(insert_word);
		return 0;
	}
	else 
		// ��� ����� ������ ����������� ���-�� ���������� �������� �������� �� ������
		// ��� ����������� ��������� � ��� ��� ����� ��������
		for (auto current_word_middle : freq_dict[sort_list_word[middle]])
			count_freq_word_middle += static_cast<int>(current_word_middle.count);

	for (auto current_freq_word : freq_dict[sort_list_word[start_pos]]) count_freq_word += static_cast<int>(current_freq_word.count);
	
	if (start_pos == end_pos) // ��� ���������� � ����� ��������� ���������� �� ������� ������� �� ����
	{
		if (count_freq_word <= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		else if (count_freq_word >= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos, insert_word);
		return 0;
	}
	else if ((start_pos + 1) == end_pos) // ��� �������� � ����� ����������� ����������� �� �������
	{
		if (count_freq_word >= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos, insert_word);
		else if (count_freq_word <= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		return 0;
	}
	else if (count_freq_word_middle == count_find_word) // ����� � ��� ������� ������ �������� �� ��������� ����� ����
	{
		sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		return 0;
	}
	// ���� � ����� ��������� ���������� ���-�� ���������� ������������ ��������
	// � ���-��� ���� ����� ������� ����� ��������
	// ���� ��� ������ ��� ������ �� �������� �������� ����� ��������� ������ �� �������
	// � ���������� ������
	else if (count_freq_word_middle < count_find_word)
		sortListWordAsc(freq_dict, sort_list_word, insert_word, middle, end_pos);
	else if (count_freq_word_middle > count_find_word)
		sortListWordAsc(freq_dict, sort_list_word, insert_word, start_pos, middle);

	return 0;
}



void resultSortRelative(std::vector<RelativeIndex>& relevance, int start, int end)
{
	if (start == end) return;					
	int middle = (start + end) / 2;				// ���������� �������� ������
												// � ���������� �������� ������� ���������� ��� ������ ��������
	resultSortRelative(relevance, start, middle);
	resultSortRelative(relevance, middle + 1, end);
	std::vector<RelativeIndex> tmp_relevance;
	int start_left_point = start;				// ������ ������ ������
	int start_right_point = middle + 1;			// ������ ������� ������
	
	// ���������� ������ �� 
	for  (int i = 0; i < end - start + 1 && end < relevance.size() - 1; i++)
	{
		// �������� ������� RelativeIndex �� ���� ������� ���� ������� �� �������
		if ((start_left_point <= middle && relevance[start_left_point].rank > relevance[start_right_point].rank) || start_right_point > end)
		{
			tmp_relevance.push_back(relevance[start_left_point]);
			start_left_point++;
		}
		else
		{
			tmp_relevance.push_back(relevance[start_right_point]);
			start_right_point++;
		}

	}
	// ��������� ��������� � relevance
	for (int i = 0; i < tmp_relevance.size(); i++)
		relevance[i + start] = tmp_relevance[i];
}


std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
	std::map<std::string, std::vector<Entry>> freq_dictionary = _index->getFreqDictionary();
	// ����� ���������� ���� �������� <�����, id �������>
	std::map<std::string, std::vector<std::string>> uniq_list_words;
	std::string req_id = "";

	for (int i = 0; i < queries_input.size(); i++)
	{
		std::stringstream str_req(queries_input[i]);
		std::string req_word;
		req_id = "request_";
		if (i >= 0 && i < 10) req_id += ("00" + std::to_string(i + 1));
		else if (i >= 10 && i <= 99) req_id += ("0" + std::to_string(i + 1));
		else if (i >= 99 && i <= 999) req_id += ("" + std::to_string(i + 1));
		uniq_list_words.insert({ req_id, {} });
		// ��������� ����� � ������� ���������� ������� �������������
		while (str_req >> req_word)
		{
			sortListWordAsc(freq_dictionary, uniq_list_words[req_id], req_word, 0, uniq_list_words[req_id].size());
		}
	}

	size_t max_relevance_number = 0;											 // ������������ ����� ���������� ��� ������� ������������� �������������
	json answers = { };															 // ��� ��������� ������� ����� ���������� � answers.json
	std::map<std::string,std::vector<RelativeIndex>> requests;					 // ������ ���������� �������� ������� � ������ �������� ����������
	std::vector<RelativeIndex> relevance;										 // ������ �������� ���������� � ���������� �������
	std::vector<std::vector<RelativeIndex>> return_vector_relative_index = {};	 // ������ ������� ����� ���������� � ����� 

	// �������� ������� �������� � ������� �������� ��������������� ����� �� ����������� ����������
	for (auto id_request : uniq_list_words) 
	{
		size_t count_doc = 0;
		for (auto word_request : id_request.second) // �������� ����� �� �������
		{
			// ������� ������� ���������� ���������� � ���� ���� ������ ������� � ��� �� ������� �������
			for (auto current_doc : freq_dictionary[word_request])
			{
				// ���� ��������� ��� �� ������ �����
				// ���� ����� �������� � ��������� � ���� ��� ��� �������� �����������
				int found_target = 0;							// ������ ��������� ���������
				bool is_found_target = false;					//������ �� �������� ��� ���
				for (int i = 0; i < relevance.size(); i++)
				{
					// ���� ���� �� � ������ ��� ���� ��������
					// � ���������� ��� ����� � ������
					int id_rel_index = relevance[i].doc_id;
					if (id_rel_index == current_doc.doc_id)
					{
						found_target = i;
						is_found_target = true;
						break;
					}
					
					else
					{
						is_found_target = false;
						found_target = i;
					}
				}
					
				// ���� ������ ������ ���� ��� �������� �� ������
				// �� ��������� � ����� ����� ��������
				// ���� ���� ����� �� ���������� ���-�� ���������� �� ���������
				if (relevance.size() == 0 || is_found_target == false )
				{
					RelativeIndex tmp_rel_index;
					tmp_rel_index.doc_id = current_doc.doc_id;
					tmp_rel_index.rank = current_doc.count;
					relevance.push_back(tmp_rel_index);
				}
				else
				{
					relevance[found_target].rank += current_doc.count;
				}
					

				// ������������� �������� ��� ������� ������������� �������������
				if (max_relevance_number < relevance[found_target].rank)
					max_relevance_number = relevance[found_target].rank;
			}
		}
		// ��������� relevance ����
		resultSortRelative(relevance, 0, relevance.size());

		// ��� ��� ��� ���� ��������� � std::map<std::string,std::vector<RelativeIndex>> requests ����� ����� ������������� � json
		if (requests.find(id_request.first) == requests.end()) requests.insert({ id_request.first, relevance });
		else requests[id_request.first] = relevance;
		relevance.clear();
	}	


	std::vector<std::vector<RelativeIndex>> reuslt_relative;
	 //����������� ������������� �������������
	for (auto& current_req : requests)
	{
		for (auto& current_doc : current_req.second) current_doc.rank = current_doc.rank / max_relevance_number;
		max_relevance_number = 1; // ��������� ������������ ���-�� ����������
		reuslt_relative.push_back(current_req.second);
	}
	
	// � ��� ���������� ����� �������������� ���� � json ))
	for (auto& result_req : requests)
	{
		// ��� ����������� ���� ������� ��������� ��� ��� � ����������� � ���� result (false ��� true)
		if (result_req.second.size() == 0) answers["answers"][result_req.first]["result"] = "false";
		else answers["answers"][result_req.first]["result"] = "true";
		
		// ���� ������� ������ ������ ���������
		// �� ������� ���� relative
		if (result_req.second.size() > 1)
		{
			for (auto& result_doc : result_req.second)
			{
				answers["answers"][result_req.first]["relevance"].push_back({ {"docid", result_doc.doc_id} });
				answers["answers"][result_req.first]["relevance"].push_back({ {"rank", result_doc.rank} });
				
				long int calc_result_rank = result_doc.rank * 1000000;
				size_t doc_position = answers["answers"][result_req.first]["relevance"].size();
				json* edit_rank = &answers["answers"][result_req.first]["relevance"][doc_position - 1]["rank"];
				double add_rank = *edit_rank;
				// �������� � �0������ ���. ����� 1 ���� �� 1.0
				if (calc_result_rank > 999999) *edit_rank = static_cast<int>(*edit_rank);
				// �������� �� 6 ������ ����� �������
				else answers["answers"][result_req.first]["relevance"][doc_position - 1]["rank"] = std::trunc(add_rank * 1000000000) / 1000000000.0000000000f; ;
			}
		}
		// ���� ����� ���� �������� �� ������ ��������� � ������ � ��������� �������
		else if (result_req.second.size() == 1)
		{
			for (auto one_doc : result_req.second)
			{
				answers["answers"][result_req.first]["docid"] = one_doc.doc_id;
				answers["answers"][result_req.first]["rank"] = one_doc.rank;

			}
		}
			
	}

	return reuslt_relative;
}