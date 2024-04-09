
// EngineSearch.cpp: определяет точку входа для приложения.
//

#include "EngineSearch.h"
#include <nlohmann/json.hpp>
#include "include/InvertedIndex.h"
#include "include/ConverterJSON.h"
#include "include/SearchServer.h"

using json = nlohmann::json;

using namespace std;

void readStrRequests(std::string abs_patch_requests, std::string name_file_requests, std::vector<std::string>& currents_requests)
{
	std::fstream requests_json(abs_patch_requests + name_file_requests);

	if (requests_json.is_open())
	{
		json read_word_req = json::parse(requests_json);

		for (std::string str_words_req : read_word_req["requests"])
		{
			currents_requests.push_back(str_words_req);
		}
	}
	else
	{
		throw ExeptNotFoundFile(name_file_requests, abs_patch_requests);
	}
}

int main()
{
	size_t count_errors = 0;
	// получаем объект данных из файлов json в директории configs
	ConverterJSON converter_json = ConverterJSON();
	std::vector<std::string> all_text_doc = { "" };
	try
	{
		all_text_doc = converter_json.GetTextDocument();
	}
	catch (const ExeptNotFoundFile& msg)
	{
		std::cout << msg.GetMessage() << std::endl;
		count_errors++;
	}
	
	// Объект с данными о индексировании документов
	InvertedIndex* inverted_index = new InvertedIndex();
	inverted_index->UpdateDocumentBase(all_text_doc);

	// Получаем вектор строк запросов из файла requests.json в директории configs
	// чтобы далее передать в сервер поиска слов в отиндексированнымх документах

	std::vector<std::string> words_request;

	try
	{
		readStrRequests(
			"C:\\cpp\\EngineSearch\\EngineSearch\\configs\\",
			"requests.json",
			words_request
		);
	}
	catch (const ExeptNotFoundFile& msg)
	{
		std::cout << msg.GetMessage() << std::endl;
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

	// записываем рузельтат в answers.json
	converter_json.PutAnswer(put_answer);

	if (count_errors > 0)
		std::cout << "Обратите внимание на предупреждения показанные в консоли.\n Если они имеются исправьте их и запустите программу заново.\n Программа с ошибками и предупреждениями не сможет выдать итоговый результат.";
	else
		std::cout << "DONE";

	return 0;
}
