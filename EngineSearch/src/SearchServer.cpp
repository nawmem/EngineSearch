#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include "../include/SearchServer.h"
#include "../include/RelativeIndex.h"
#include "../include/Entry.h"

int sort_list_word_asc
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
	int count_find_word = 0; // колличество раз когда вставляемое слово встретилось в документах
	int count_freq_word = 0; // колличество раз когда сравнимое слово встретилось в документах
	int count_freq_word_middle = 0; // колличество раз когда сравнимое слово встретилось в документах
	for (auto item_word : freq_dict[insert_word]) count_find_word += static_cast<int>(item_word.count);
	

	if (end == 0)
	{
		sort_list_word.push_back(insert_word);
		return 0;
	}
	else for (auto item_word_middle : freq_dict[sort_list_word[middle]]) count_freq_word_middle += static_cast<int>(item_word_middle.count);

	if (start_pos == end_pos)
	{
		for (auto item_freq_word : freq_dict[sort_list_word[start_pos]]) count_freq_word += static_cast<int>(item_freq_word.count);

		if (count_freq_word <= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		else if (count_freq_word >= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos, insert_word);
		return 0;
	}
	else if ((start_pos + 1) == end_pos)
	{
		if (count_freq_word <= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos, insert_word);
		else if (count_freq_word <= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		return 0;
	}
	else if (count_freq_word_middle == count_find_word)
	{
		sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		return 0;
	}
	else if (count_freq_word_middle < count_find_word)
		sort_list_word_asc(freq_dict, sort_list_word, insert_word, middle, end_pos);
	else if (count_freq_word_middle > count_find_word)
		sort_list_word_asc(freq_dict, sort_list_word, insert_word, start_pos, middle - 1);
	return 0;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
	std::map<std::string, std::vector<Entry>> freq_dictionary = _index->getFreqDictionary();
	// карта уникальных слов запросов <слово, id запроса>
	std::map<std::string, std::vector<std::string>> uniq_list_words;
	std::string req_id = "";
	for (int i=0; i < queries_input.size(); i++)
	{
		std::stringstream str_req(queries_input[i]);
		std::string req_word;
		req_id = "request_";
		if (i >= 0 && i < 10) req_id += ("00" + std::to_string(i + 1));
		else if (i >= 10 && i <= 99) req_id += ("0" + std::to_string(i + 1));
		else if (i >= 99 && i <= 999) req_id += ("" + std::to_string(i + 1));
		uniq_list_words.insert({req_id, {}});
		while (str_req >> req_word)
		{
			sort_list_word_asc(freq_dictionary, uniq_list_words[req_id], req_word, 0, uniq_list_words[req_id].size());
		}
		// создаем список уникальных слов по каждому запросу
	}
	uniq_list_words;


	//// сортируем слова в порядке увеличения частоты встречаемости
	//std::map<std::string, std::vector<std::string>> list_words_asc; // лист отсортированных слов по возрастанию
	//std::map<std::string, std::vector<Entry>> freq_dictionary = _index->getFreqDictionary();
	//// получаем запрос
	//for (int i = 0; i < uniq_list_words.size(); i++)
	//{
	//	// получаем слово из запроса
	//	for (auto word_search : uniq_list_words[i])
	//	{
	//		// ищем слово в словаре freq_dictionary
	//		auto find_word = freq_dictionary.find(word_search.first);
	//		if (find_word != freq_dictionary.end())
	//		{
	//			if (list_words_asc.find(word_search.second) == list_words_asc.end())
	//			{
	//				std::pair<std::string, std::vector<std::string>> item(word_search.second, {});
	//				list_words_asc.insert(item);
	//				list_words_asc[word_search.second].push_back(find_word->first);
	//			}
	//			else
	//			{
	//				// тут добавляем слово
	//				list_words_asc[word_search.second].push_back(find_word->first);

	//				// тут сортируем
	//				sort_list_req();
	//			}
	//			
	//			
	//		}
	//	}
	//}
	


	return { "asdasd", "asdasd" };
}