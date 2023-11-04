#include "ArgTree.hh"
#include "Console.hh"


std::vector<std::string_view> HLSLBuilder::ArgTree::s_StrArgs = std::vector<std::string_view>();
const std::unordered_map<std::string_view, HLSLBuilder::ArgCategory> HLSLBuilder::ArgTree::s_ArgMapper =
{
	{"-api", HLSLBuilder::ArgCategory::API},
	{"--target-api", HLSLBuilder::ArgCategory::API},
	{"-b", HLSLBuilder::ArgCategory::BUILD},
	{"--build", HLSLBuilder::ArgCategory::BUILD},
	{"-c", HLSLBuilder::ArgCategory::CONFIG},
	{"--config", HLSLBuilder::ArgCategory::CONFIG},
	{"-h", HLSLBuilder::ArgCategory::HELP},
	{"--help", HLSLBuilder::ArgCategory::HELP},
	{"-v", HLSLBuilder::ArgCategory::VERSION},
	{"--version", HLSLBuilder::ArgCategory::VERSION}
};
std::unordered_map<HLSLBuilder::ArgCategory, std::string_view> HLSLBuilder::ArgTree::s_ArgValues;

std::queue<HLSLBuilder::ArgCategory> HLSLBuilder::ArgTree::s_InfoArgTree;
std::queue<std::pair<HLSLBuilder::ArgCategory, std::string>> HLSLBuilder::ArgTree::s_ControlArgTree;

const std::list<HLSLBuilder::ArgCategory> HLSLBuilder::ArgTree::s_InfoArgs = std::list<HLSLBuilder::ArgCategory>({ HLSLBuilder::ArgCategory::HELP, HLSLBuilder::ArgCategory::VERSION });
const std::list<HLSLBuilder::ArgCategory> HLSLBuilder::ArgTree::s_ControlArgs = std::list<HLSLBuilder::ArgCategory>({ HLSLBuilder::ArgCategory::API, HLSLBuilder::ArgCategory::BUILD, HLSLBuilder::ArgCategory::CONFIG });

void HLSLBuilder::ArgTree::PushRawArg(std::string_view arg)
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

std::queue<HLSLBuilder::ArgCategory> HLSLBuilder::ArgTree::GetInfoArgs()
{
	return s_InfoArgTree;
}

std::queue<std::pair<HLSLBuilder::ArgCategory, std::string>> HLSLBuilder::ArgTree::GetControlArgs()
{
	return s_ControlArgTree;
}

void HLSLBuilder::ArgTree::ResolveRegex(std::string_view arg)
{
	std::string text = arg.data();

	std::regex pattern(":");

	std::sregex_token_iterator matcher(text.begin(), text.end(), pattern, -1);
	std::sregex_token_iterator end;

	size_t numOfMatches = std::distance(matcher,end) - 1;

	if (numOfMatches > 1)
	{
		Console::Error("Invalid argument");
		return;
	}
	
	if (numOfMatches == 0)
		PushInfoArgTreated(text);
	else
		PushControlArgTreated(&matcher);

}

void HLSLBuilder::ArgTree::PushInfoArgTreated(std::string_view arg)
{
	auto it = s_ArgMapper.find(arg);
	if (it != s_ArgMapper.end())
	{
		auto list_it = std::find(s_InfoArgs.begin(), s_InfoArgs.end(), it->second);
		if (list_it == s_InfoArgs.end())
			Console::Error("Argument: {0} is not an info argument", arg);
		else
			s_InfoArgTree.push(it->second);
	}
	else
		Console::Error("Argument: {0} not found", arg);
}

void HLSLBuilder::ArgTree::PushControlArgTreated(std::sregex_token_iterator* arg)
{
	std::sregex_token_iterator end;
	size_t elementsCount = (*arg)->length();
	std::string* args = new std::string[elementsCount];
	size_t i = 0;
	while ((*arg) != end)
	{
		args[i] = (*arg)->str();
		++(*arg);
		++i;
	}
	auto it = s_ArgMapper.find(args[0]);
	if (it != s_ArgMapper.end())
	{
		auto list_it = std::find(s_ControlArgs.begin(), s_ControlArgs.end(), it->second);
		if(list_it == s_ControlArgs.end())
			Console::Error("Argument: {0} is not a control argument", args[0]);
		else
			s_ControlArgTree.push(std::make_pair(it->second, args[1]));
	}
	else
		Console::Error("Argument: {0} not found", args[0]);
	delete[] args;
}
