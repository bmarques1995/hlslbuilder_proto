#include "starter.hh"
#include <iostream>
#include <string>
#include "Utils/FileHandler.hh"

void HLSLBuilder::Starter::PrintHello()
{
	std::string json_text;
	FileHandler::ReadTextFile("assets/test.json", &json_text);
	std::cout << json_text << "\n";
}
