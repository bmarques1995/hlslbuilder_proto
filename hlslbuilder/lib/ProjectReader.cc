#include "ProjectReader.hh"
#include <iostream>
#include <string>
#include "Utils/FileHandler.hh"

void HLSLBuilder::ProjectReader::LoadProject(std::string_view path)
{
	std::string json_text;
	FileHandler::ReadTextFile(path, &json_text);
}
