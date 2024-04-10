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
	int count_find_word = 0;			// колличество раз когда вставляемое слово встретилось в документах
	int count_freq_word = 0;			// колличество раз когда сравнимое слово встретилось в документах
	int count_freq_word_middle = 0;		// колличество раз когда слово из середины встретилось в документах
	for (auto current_word : freq_dict[insert_word]) count_find_word += static_cast<int>(current_word.count);
	
	// если список пустой то пушим в него первый элемент
	if (sort_list_word.size() == 0)
	{
		sort_list_word.push_back(insert_word);
		return 0;
	}
	else 
		// тут можем начать высчитывать кол-во совпадений среднего элемента из списка
		// для дальнейшего сравнения с тем что нужно вставить
		for (auto current_word_middle : freq_dict[sort_list_word[middle]])
			count_freq_word_middle += static_cast<int>(current_word_middle.count);

	for (auto current_freq_word : freq_dict[sort_list_word[start_pos]]) count_freq_word += static_cast<int>(current_freq_word.count);
	
	if (start_pos == end_pos) // тут сравниваем с одним элементом оставшимся от деления массива на пары
	{
		if (count_freq_word <= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		else if (count_freq_word >= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos, insert_word);
		return 0;
	}
	else if ((start_pos + 1) == end_pos) // тут работаем с двумя эелементами оставшимися от деления
	{
		if (count_freq_word >= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos, insert_word);
		else if (count_freq_word <= count_find_word) sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		return 0;
	}
	else if (count_freq_word_middle == count_find_word) // вдруг у нас попался равный эелемент то вставляем после него
	{
		sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
		return 0;
	}
	// ниже в обоих вариантах сравниваем кол-во совпадений центрального элемента
	// с кол-вом того слова которое нужно вставить
	// если оно больше или меньше то вызываем рикурсию чтобы разделить массив на пополам
	// и продолжить поиски
	else if (count_freq_word_middle < count_find_word)
		sortListWordAsc(freq_dict, sort_list_word, insert_word, middle, end_pos);
	else if (count_freq_word_middle > count_find_word)
		sortListWordAsc(freq_dict, sort_list_word, insert_word, start_pos, middle);

	return 0;
}



void sortRelative(std::vector<RelativeIndex>& relevance, int start, int end)
{
	if (start == end) return;					
	int middle = (start + end) / 2;				// определяем середину списка
												// и рекурсивно вызываем функцию сортировки для каждой половины
	sortRelative(relevance, start, middle);
	sortRelative(relevance, middle + 1, end);
	std::vector<RelativeIndex> tmp_relevance;
	int start_left_point = start;				// начало левого списка
	int start_right_point = middle + 1;			// начало правого списка
	
	// перебираем список от 
	for  (int i = 0; i < end - start + 1 && end < relevance.size() - 1; i++)
	{
		// добвляем больший RelativeIndex из двух списков либо остаток от первого
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
	// сохраняем изменения в relevance
	for (int i = 0; i < tmp_relevance.size(); i++)
		relevance[i + start] = tmp_relevance[i];
}


std::vector<std::vector<RelativeIndex>> SearchServer::Search(const std::vector<std::string>& queries_input)
{
	std::map<std::string, std::vector<Entry>> freq_dictionary = _index->GetFreqDictionary();
	// список уникальных слов запросов
	std::vector<std::vector<std::string>> uniq_list_words;
	std::string req_id = "";

	for (int i = 0; i < queries_input.size(); i++)
	{
		std::stringstream str_req(queries_input[i]);
		std::string req_word;
		// сортируем слова в порядке увеличения частоты встречаемости
		uniq_list_words.push_back({});
		while (str_req >> req_word)
		{
			sortListWordAsc(freq_dictionary, uniq_list_words[i], req_word, 0, uniq_list_words[i].size());
		}
	}



	size_t max_relevance_number = 0;											 // максимальное число совпадений для расчета относительной релевантности
	std::vector<std::vector<RelativeIndex>> requests;					         // список запроса и список найденых документов
	std::vector<RelativeIndex> relevance;										 // список найденых документов у отдельного запроса
	std::vector<std::vector<RelativeIndex>> return_vector_relative_index = {};	 // вектор который будем возвращать в итоге 

	// получаем вектора запросов в которых хранятся отсортированные слова по возрастанию совпадений
	for (auto id_request : uniq_list_words) 
	{
		size_t count_doc = 0;
		for (auto word_request : id_request) // получили слово из запроса
		{
			// считаем сколько документов получилось у всех слов одного запроса и так по каждому запросу
			for (auto current_doc : freq_dictionary[word_request])
			{
				// если эелемента нет то просто пушим
				// надо найти эелемент и прибавить к нему еще одн значение соответсвия
				int found_target = 0;							// индекс найденого эелемента
				bool is_found_target = false;					//найден ли эелемент или нет
				for (int i = 0; i < relevance.size(); i++)
				{
					// ищем есть ли в списке уже этот документ
					// и записываем его номер в списке
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
					
				// если размер списка ноль или документ не найден
				// мы добавляем в конец новый документ
				// если есть такой то прибавляем кол-во совпадений по документу
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
					

				// устанавливаем максимум для расчета относительной релевантности
				if (max_relevance_number < relevance[found_target].rank)
					max_relevance_number = relevance[found_target].rank;
			}
		}
		// сортируем relevance лист
		sortRelative(relevance, 0, relevance.size());


		// тут это все дело вставляем в requests чтобы потом пеобразовать в json
		requests.push_back(relevance);
		relevance.clear();
	}	

	std::vector<std::vector<RelativeIndex>> reuslt_relative;
	 //высчитываем относительную релевантность
	for (auto& current_req : requests)
	{
        // это должно быть не тут нужно делить в запросе
		for (auto& current_doc : current_req) current_doc.rank = current_doc.rank / max_relevance_number;
		max_relevance_number = 1; // скидываем максимальное кол-во совпадений
	}
;
	return requests;
}