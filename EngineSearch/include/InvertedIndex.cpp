#include "../src/InvertedIndex.h"
//
// Created by user on 13.02.2024.
//

#include "../src/InvertedIndex.h"
#include <fstream>
#include <thread>
#include <sstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> text_files_input) {

    size_t count_files = text_files_input.size(); // колличество файлов в которые нужно прогнать
    std::vector<std::thread> threads;
    for (std::string in_text: text_files_input)
    {
        threads.push_back(std::thread([this](std::string file_text){
            // открываем файл
            this->docs.push_back(file_text);
        }, in_text));
    }

    // ждем завершения потоков
    for (int i = 0; i < count_files; i++)
    {
        threads[i].join();
    }

    // перебираем коллекцию this->docs и ищем соответсвие в this->freq_dictionary
    // составляем список уникальных слов в документе и их колличество
    for (int i = 0; i < this->docs.size(); ++i) {
        std::stringstream str;
        str << this->docs[i];
        std::string word;
        while (str >> word)
        {
            // поиск ключа по слову
            if (this->freq_dictionary.find(word) != this->freq_dictionary.end())
            {
                bool is_item_dict = false;
                Entry *num_iter_dict;
                for ( int j = 0; j < this->freq_dictionary[word].size(); j++)
                {
                    if (this->freq_dictionary[word][j].doc_id == i)
                    {
                        is_item_dict = true;
                        num_iter_dict = &this->freq_dictionary[word][j];
                        break;
                    }
                }
                if (is_item_dict)
                {
                    num_iter_dict->count++;
                }
                else
                {
                    Entry add_new_entry = {static_cast<size_t>(i), 1};
                    this->freq_dictionary[word].push_back(add_new_entry);
                }

            }
            else
            {
                Entry new_entry = {static_cast<size_t>(i), 1};
                this->freq_dictionary.insert({word, {new_entry}});
            }

        }
    }

}

std::vector<Entry> InvertedIndex::GetWordCount(std::string word)
{
    for (auto v_entry: this->freq_dictionary)
    {
        if (v_entry.first == word)
        {
            return v_entry.second;
        }
    }
    return {{0, 0}};
}