//
// Created by user on 13.02.2024.
//

#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"
#include "../include/InvertedIndex.h"

using json = nlohmann::json;

void InvertedIndex::updateDocumentBase(std::vector<std::string> text_files_input) {

    // получаем кол-во слов
    size_t count_files = text_files_input.size(); 
    std::vector<std::thread> threads; // вектор потоков
    for (std::string in_text: text_files_input)
    {
        this->docs.push_back(in_text);
       
    }

    

    // перебираем коллекцию this->docs и ищем соответсвие в this->freq_dictionary
    // составляем список уникальных слов в документе и их колличество
    for (int i = 0; i < this->docs.size(); ++i) 
    {
        std::stringstream str;
        std::string word;
        this->m_docs.lock();
        std::thread fd_thread([this, &str, &word, &i]()
        {
            // открываем файл
            str << this->docs[i];
            while (str >> word)
            {
                // поиск ключа по слову
                if (this->freq_dictionary.find(word) != this->freq_dictionary.end())
                {
                    bool is_dict = false;
                    Entry *num_iter_dict;
                    for ( int j = 0; j < this->freq_dictionary[word].size(); j++)
                    {
                        if (this->freq_dictionary[word][j].doc_id == i)
                        {
                            is_dict = true;
                            num_iter_dict = &this->freq_dictionary[word][j];
                            break;
                        }
                    }
                    if (is_dict)
                    {
                        num_iter_dict->count++; // если совпадения были в документе то инкриментируем
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

        });

        fd_thread.join();
        this->m_docs.unlock();
        // ждем завершения потоков
        
    }
}

std::vector<Entry> InvertedIndex::getWordCount(std::string word)
{
    for (auto docs: this->freq_dictionary)
    {
        if (docs.first == word)
        {
            return docs.second;
        }
    }
    return {{0, 0}};
}