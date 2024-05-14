//
// Created by user on 13.02.2024.
//

#include "EngineSearch.h"
#include <nlohmann/json.hpp>
#include "include/InvertedIndex.h"
#include "include/ConverterJSON.h"
#include "include/SearchServer.h"

using json = nlohmann::json;

using namespace std;


int main()
{
	size_t count_errors = 0;
	// получаем объект данных из файлов json в директории configs
	ConverterJSON converter_json;
	std::vector<std::string> all_text_doc;
	try
	{
		all_text_doc = converter_json.getTextDocument();
	}
	catch (const ExeptNotFoundFile& msg)
	{
		std::cout << msg.getMessage() << std::endl;
		count_errors++;
	}

	// Объект с данными об индексировании документов
	InvertedIndex inverted_index;
	inverted_index.updateDocumentBase(all_text_doc);

	// Получаем вектор строк запросов из файла requests.json в директории configs
	// чтобы далее передать в сервер поиска слов в отиндексированнымх документах
    std::vector<std::string> words_request;
    try
    {
        words_request = converter_json.getRequest();
    }
    catch (const ExeptNotFoundFile& msg)
    {
        std::cout << msg.getMessage() << std::endl;
        count_errors++;
    }


	// Объект индексации документов, в конструктор которого передаем указатель на объект inverted_index
	SearchServer search_server(&inverted_index);
    int limit_request_response = converter_json.getResponseLimit();
    search_server.setResponseLimit(limit_request_response);
	auto all_request = search_server.search(words_request);
	std::vector<std::vector<std::pair<int, float>>> put_answer;
	for (int i = 0; i < all_request.size(); i++)
	{
		auto& current_req = all_request[i];
		put_answer.emplace_back();
		for (auto& current_doc: current_req)
		{
			put_answer[i].push_back(std::pair<int, float>( { current_doc.doc_id, current_doc.rank }));
		}
	}
	// записываем рузельтат в answers.json
	converter_json.putAnswer(put_answer);

	if (count_errors > 0)
    {
        std::cout << "Обратите внимание на предупреждения показанные в консоли.\n Если они имеются исправьте их и запустите программу заново.\n Программа с ошибками и предупреждениями не сможет выдать итоговый результат.";
        return 1;
    }
    else
    {
        std::cout << "DONE";
        return 0;
    }

}
