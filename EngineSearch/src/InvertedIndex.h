//
// Created by user on 13.02.2024.
//

#ifndef ENGINESEARCH_INVERTEDINDEX_H
#define ENGINESEARCH_INVERTEDINDEX_H
#include <iostream>
#include <string>
#include <vector>
#include "Entry.h"
#include <map>
class InvertedIndex {
private:
    std::vector<std::string> docs; // содержимое файлов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // список вхождений с частотой слов
public:
    InvertedIndex() = default;

    /**
    * Обновить или заполнить базу документов, в которой совершаем поиск
    * @param docs_input содежимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> docs_input);

    /**
     * Метод определяет вхождений слова в загруженной базе
     * @param word - искомое слово
     * @return список вхождений с частотой слов
     */
    std::vector<Entry> GetWordCount(std::string word);
};


#endif //ENGINESEARCH_INVERTEDINDEX_H
