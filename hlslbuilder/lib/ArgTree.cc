#include "ArgTree.hh"
#include "Console.hh"
#include <regex>

std::vector<std::string_view> HLSLBuilder::ArgTree::s_StrArgs = std::vector<std::string_view>();
const std::unordered_map<std::string_view, HLSLBuilder::ArgCategory> s_ArgMapper = 
{
	{"-api", HLSLBuilder::ArgCategory::API},
	{"--target-api", HLSLBuilder::ArgCategory::API},
	{"-b", HLSLBuilder::ArgCategory::BUILD},
	{"--build", HLSLBuilder::ArgCategory::BUILD},
	{"-c", HLSLBuilder::ArgCategory::CONFIG},
	{"--config", HLSLBuilder::ArgCategory::CONFIG},
	{"-h", HLSLBuilder::ArgCategory::HELP},
	{"--help", HLSLBuilder::ArgCategory::HELP}
};
std::unordered_map<HLSLBuilder::ArgCategory, std::string_view> s_ArgValues;

void HLSLBuilder::ArgTree::PushArg(std::string_view arg)
{
	s_StrArgs.push_back(arg);
}

void HLSLBuilder::ArgTree::ResolveArgs()
{
	for (auto& i : s_StrArgs)
	{
		ResolveRegex(i);
	}
}

void HLSLBuilder::ArgTree::ResolveRegex(std::string_view arg)
{
	std::string text = arg.data();

	// Regular expression pattern to split the text using ':'
	std::regex pattern(":");

	// Using std::sregex_token_iterator to split the text
	std::sregex_token_iterator iterator(text.begin(), text.end(), pattern, -1);
	std::sregex_token_iterator end;

	// Iterate through the tokens and print them
	while (iterator != end) {
		Console::Log(iterator->str());
		++iterator;
	}
}
