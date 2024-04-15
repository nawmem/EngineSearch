#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <nlohmann/json.hpp>
#include "../include/SearchServer.h"
#include "../include/RelativeIndex.h"
#include "../include/Entry.h"
#include "ConverterJSON.h"

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
	int middle = (start + end) / 2;
	int count_insert_word = 0;			// колличество раз когда вставляемое слово встретилось в документах
	int count_compare_word = 0;			// колличество раз когда сравнимое слово встретилось в документах
	int count_middle_word = 0;		// колличество раз когда слово из середины встретилось в документах

    if (sort_list_word.size() == 0)
    {
        sort_list_word.push_back(insert_word);
        return 0;
    }

    // считаем кол-во совпадния вставляемого слова из списка по всем документам
    for (auto add_word: freq_dict[insert_word])
        count_insert_word += add_word.count;
    // считаем кол-во совпадния среднего слова из списка по всем документам
    for (auto middle_word: freq_dict[sort_list_word[middle]])
        count_middle_word += middle_word.count;

    if (start_pos == end_pos)
    {
        for (auto compare_word: freq_dict[sort_list_word[start_pos]])
            count_compare_word += compare_word.count;

        if (sort_list_word[start_pos] == insert_word)
            return 0;
        else if (count_insert_word >= count_compare_word)
        {
            sort_list_word.insert(sort_list_word.begin() + start_pos + 1, insert_word);
            return 0;
        }
        else if (count_insert_word < count_compare_word )
        {
            sort_list_word.insert(sort_list_word.begin() + start_pos, insert_word);
            return 0;
        }
    }

    if (count_insert_word <= count_middle_word) // меньше чем центральный эелемент
    {
        sortListWordAsc(freq_dict, sort_list_word, insert_word, start_pos, middle);
    }
    else if (count_insert_word > count_middle_word) // больше чем центральный эелемент
    {
        sortListWordAsc(freq_dict, sort_list_word, insert_word, middle + 1, end_pos);
    }

    return 0;


}

void sortRelative(std::vector<RelativeIndex>& relevance, int start, int end)
{
	if (start == end) return;
	int middle = (start + end) / 2;				// определяем середину списка
												// и рекурсивно вызываем функцию сортировки для каждой половины

    int start_left_point = start;				// начало левого списка
    int start_right_point = middle + 1;			// начало правого списка
	sortRelative(relevance, start, middle);
	sortRelative(relevance, middle + 1, end);
	std::vector<RelativeIndex> tmp_relevance;
	
	// перебираем список от 
	for (int i = 0; i < end - start + 1; i++)
	{
		// добвляем больший экземпляр RelativeIndex из двух списков, либо остаток от первого
		if (start_right_point > end || ((start_left_point <= middle) && (relevance[start_left_point].rank > relevance[start_right_point].rank)))
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

void SearchServer::setResponseLimit(int in_limit)
{
    if (in_limit > 0) this->response_limit = in_limit;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
	std::map<std::string, std::vector<Entry>> freq_dictionary = _index->GetFreqDictionary();
	// список уникальных слов запросов
	std::vector<std::vector<std::string>> uniq_list_words;
//    ConverterJSON* converter_json_s = new ConverterJSON;
//    int response_limit = converter_json_s->GetResponseLimit();
//    delete converter_json_s;
	for (int i = 0; i < queries_input.size(); i++)
	{
		std::stringstream str_req(queries_input[i]);
		std::string req_word;
		// сортируем слова в порядке увеличения частоты встречаемости
		uniq_list_words.push_back({});
		while (str_req >> req_word)
		{
			sortListWordAsc(freq_dictionary, uniq_list_words[i], req_word, 0, uniq_list_words[i].size() - 1);
		}
	}



	size_t max_relevance_number = 0;											 // максимальное число совпадений для расчета относительной релевантности
	std::vector<std::vector<RelativeIndex>> requests;					         // список запроса и список найденых документов
	std::vector<RelativeIndex> relevance;										 // список найденых документов у отдельного запроса
	std::vector<std::vector<RelativeIndex>> return_vector_relative_index = {};	 // вектор который будем возвращать в итоге 

	// получаем вектора запросов в которых хранятся отсортированные слова по возрастанию совпадений
	for (auto& id_request : uniq_list_words)//(auto id_request : uniq_list_words)
	{
		size_t count_doc = 0;
		for (auto& word_request : id_request) // получили слово из запроса
		{
			// считаем сколько документов получилось у всех слов одного запроса и так по каждому запросу
			for (auto& current_doc : freq_dictionary[word_request])
			{

                // если элемента нет то просто пушим
                // надо найти эелемент и прибавить к нему еще одн значение соответсвия
                int found_target = 0;                            // индекс найденого эелемента
                bool is_found_target = false;                    //найден ли эелемент или нет
                for (int i = 0; i < relevance.size() && i < 5 ; i++) {
                    // ищем есть ли в списке уже этот документ
                    // и записываем его номер в списке
                    int id_rel_index = relevance[i].doc_id;
                    if (id_rel_index == current_doc.doc_id) {
                        found_target = i;
                        is_found_target = true;
                        break;
                    } else {
                        is_found_target = false;
                        found_target = i;
                    }
                }

                // если размер списка ноль или документ не найден
                // мы добавляем в конец новый документ
                // если есть такой то прибавляем кол-во совпадений по документу
                if (relevance.size() == 0 || is_found_target == false) {
                    RelativeIndex tmp_rel_index;
                    tmp_rel_index.doc_id = current_doc.doc_id;
                    tmp_rel_index.rank = current_doc.count;
                    relevance.push_back(tmp_rel_index);
                } else {
                    relevance[found_target].rank += current_doc.count;
                }


                // устанавливаем максимум для расчета относительной релевантности
                if (max_relevance_number < relevance[found_target].rank)
                    max_relevance_number = relevance[found_target].rank;

			}
		}

        // сортируем relevance лист
        if (relevance.size() > 0)
            sortRelative(relevance, 0, relevance.size() - 1);
        // обрезаем выдачу до лимита ответов на запрос
        if (relevance.size() >= this->response_limit) relevance.resize(this->response_limit);

        //высчитываем относительную релевантность
        for (auto& c_doc : relevance)
            c_doc.rank = c_doc.rank / max_relevance_number;

        max_relevance_number = 1; // скидываем максимальное кол-во совпадений
        // тут это все дело вставляем в requests чтобы потом пеобразовать в json
        requests.push_back(relevance);
		relevance.clear();
	}	

	return requests;
}