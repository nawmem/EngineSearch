#pragma once
#include <iostream>

class ExeptNotFoundFile
{
	std::string message;
public:
	ExeptNotFoundFile(std::string in_name_file, std::string in_file_path)
	{
		this->message = "ATTENTION: There is no such file \"" + in_name_file + " at \"" + in_file_path;
	}
	std::string getMessage() const { return this->message; }
};