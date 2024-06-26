//
// Created by user on 13.02.2024.
//

#ifndef ENGINESEARCH_INVERTEDINDEX_H
#define ENGINESEARCH_INVERTEDINDEX_H
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include "../include/Entry.h"
class InvertedIndex {
private:
    std::vector<std::string> docs; // содержимое файлов
    std::mutex m_docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary; // список вхождений с частотой слов
public:
    InvertedIndex() = default;

    /**
    * Обновить или заполнить базу документов, в которой совершаем поиск
    * @param docs_input содежимое документов
    */
    void updateDocumentBase(std::vector<std::string> docs_input);

    /**
     * Метод определяет вхождений слова в загруженной базе
     * @param word - искомое слово
     * @return список вхождений с частотой слов
     */
    std::vector<Entry> getWordCount(std::string word);

    std::map<std::string, std::vector<Entry>> GetFreqDictionary()
    {
        return this->freq_dictionary;
    }
};


#endif //ENGINESEARCH_INVERTEDINDEX_H
