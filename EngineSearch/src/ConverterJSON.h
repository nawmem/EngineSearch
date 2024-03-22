//
// Created by user on 13.02.2024.
//

#ifndef ENGINESEARCH_CONVERTERJSON_H
#define ENGINESEARCH_CONVERTERJSON_H
#include <iostream>
#include <fstream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;


class ConverterJSON {
    std::string src_config = "C:\\Users\\User\\source\\repos\\EngineSearch\\EngineSearch\\configs\\config.json";
    std::string src_request = "C:\\Users\\User\\source\\repos\\EngineSearch\\EngineSearch\\configs\\requests.json";
    std::string src_answers = "C:\\Users\\User\\source\\repos\\EngineSearch\\EngineSearch\\configs\\answers.json";
public:
    ConverterJSON() = default;

    /**
     * метод получения содержимого документа
     * @return спиок с содежимым файлов
     */
    std::vector<std::string> GetTextDocument();

    /**
     * @return возвращает колличество ответов на один запрос
     */
    int GetResponseLimit();

    /**
     * Получает запросы из файла request
     * @return список запросов из файла
     */
    std::vector<std::string> GetRequest();

    /**
     * Ложит в файл answer результат поисковых запросов
     */
    void putAnswer(std::vector<std::vector<std::pair<int, float>>> answer);
};


#endif //ENGINESEARCH_CONVERTERJSON_H
