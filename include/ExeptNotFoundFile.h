#pragma once
#include <iostream>

class ExeptNotFoundFile
{
	std::string message;
public:
	ExeptNotFoundFile(std::string in_name_file, std::string in_file_path, int num_str, std::string name_file)
	{
		this->message = "ATTENTION: There is no such file \"" + in_name_file + " at \"" + in_file_path + "\" " + std::to_string(num_str) + " " + name_file;
	}
	std::string GetMessage() const { return this->message; }
};