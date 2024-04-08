
// EngineSearch.cpp: определяет точку входа для приложения.
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
	// получаем объект данных из файлов json в директории configs
	ConverterJSON converter_json = ConverterJSON();
	std::vector<std::string> all_text_doc = converter_json.GetTextDocument();
	
	// Объект с данными о индексировании документов
	InvertedIndex* inverted_index = new InvertedIndex();
	inverted_index->UpdateDocumentBase(all_text_doc);

	// Получаем вектор строк запросов из файла requests.json в директории configs
	// чтобы далее передать в сервер поиска слов в отиндексированнымх документах
	std::fstream requests_json("C:\\cpp\\EngineSearch\\EngineSearch\\configs\\requests.json");
	std::vector<std::string> words_request;
	if (requests_json.is_open())
	{
		json read_word_req = json::parse(requests_json);

		for (std::string str_words_req : read_word_req["requests"])
		{
			words_request.push_back(str_words_req);
		}
	}

	// Объект индексации документов, в конструктор которого передаем указатель на объект inverted_index
	SearchServer search_server(inverted_index);
	auto all_request = search_server.Search(words_request);
	std::vector<std::vector<std::pair<int, float>>> put_answer;
	for (int i = 0; i < all_request.size(); i++)
	{
		auto& current_req = all_request[i];
		put_answer.push_back({});
		for (auto& current_doc: current_req)
		{
			put_answer[i].push_back(std::pair<int, float>( { current_doc.doc_id, current_doc.rank }));
		}
	}

	converter_json.PutAnswer(put_answer);



	return 0;
}
