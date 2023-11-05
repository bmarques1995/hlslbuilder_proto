#include "Utils/StringHandler.hh"

void HLSLBuilder::StringHandler::ToLower(std::string* content)
{
	for (std::string::iterator it = content->begin(); it != content->end(); it++)
	{
		*it = std::tolower(*it);
	}
}

void HLSLBuilder::StringHandler::ToUpper(std::string* content)
{
	for (std::string::iterator it = content->begin(); it != content->end(); it++)
	{
		*it = std::toupper(*it);
	}
}
