#include "ArgTree.hh"
#include "Console.hh"
#include "Utils/StringHandler.hh"

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

const std::unordered_map<std::string_view, HLSLBuilder::ArgCategory> HLSLBuilder::ArgTree::s_ValidAssignments =
{
	{"cso", HLSLBuilder::ArgCategory::API},
	{"spv", HLSLBuilder::ArgCategory::API},
	{"debug", HLSLBuilder::ArgCategory::CONFIG},
	{"release", HLSLBuilder::ArgCategory::CONFIG}
};

HLSLBuilder::ArgCategory HLSLBuilder::ArgTree::s_InfoArg;
bool HLSLBuilder::ArgTree::s_InfoAssigned;
std::list<std::pair<HLSLBuilder::ArgCategory, std::string>> HLSLBuilder::ArgTree::s_ControlArgTree;

const std::list<HLSLBuilder::ArgCategory> HLSLBuilder::ArgTree::s_InfoArgs = std::list<HLSLBuilder::ArgCategory>(
	{
		HLSLBuilder::ArgCategory::HELP,
		HLSLBuilder::ArgCategory::VERSION
	});
const std::list<HLSLBuilder::ArgCategory> HLSLBuilder::ArgTree::s_ControlArgs = std::list<HLSLBuilder::ArgCategory>(
	{
		HLSLBuilder::ArgCategory::API,
		HLSLBuilder::ArgCategory::BUILD,
		HLSLBuilder::ArgCategory::CONFIG
	});

void HLSLBuilder::ArgTree::PushRawArg(std::string_view arg)
{
	s_StrArgs.push_back(arg);
}

void HLSLBuilder::ArgTree::ResolveArgs()
{
	for (auto& i : s_StrArgs)
	{
		ClassifyAndEvaluateArgs(i);
	}
	//validate usage
}

void HLSLBuilder::ArgTree::ClearArgs()
{
	s_StrArgs.clear();
	s_InfoAssigned = false;
	while (s_ControlArgTree.size() > 0)
		s_ControlArgTree.pop_front();
}

HLSLBuilder::ArgCategory HLSLBuilder::ArgTree::GetInfoArg()
{
	return s_InfoArg;
}

std::list<std::pair<HLSLBuilder::ArgCategory, std::string>> HLSLBuilder::ArgTree::GetControlArgs()
{
	return s_ControlArgTree;
}

void HLSLBuilder::ArgTree::ClassifyAndEvaluateArgs(std::string_view arg)
{
	std::string text = arg.data();

	std::regex pattern(":");

	std::sregex_token_iterator matcher(text.begin(), text.end(), pattern, -1);
	std::sregex_token_iterator end;

	size_t numOfMatches = std::distance(matcher, end) - 1;

	if (numOfMatches > 1)
	{
		throw BadAssignmentException("Invalid Argument Assigment Evaluation, neither argument accept two values");
	}

	if (numOfMatches == 0)
		PushInfoArgTreated(text);
	else
		PushControlArgTreated(&matcher);

}

void HLSLBuilder::ArgTree::ValidateControlAssignment(std::string_view value, HLSLBuilder::ArgCategory category)
{
	if (category == ArgCategory::BUILD)
		return;
	auto list_it = s_ValidAssignments.find(value);
	if ((list_it == s_ValidAssignments.end()) || (list_it->second != category))
		throw BadEvaluationException(category, value.data());
}

void HLSLBuilder::ArgTree::PushInfoArgTreated(std::string_view arg)
{
	auto it = s_ArgMapper.find(arg);
	if (it != s_ArgMapper.end())
	{
		//multiple push treatment
		if(s_ControlArgTree.size() > 0)
			throw InvalidUsageException(false);
		if(s_InfoAssigned)
			throw InvalidUsageException(true);
		auto list_it = std::find(s_InfoArgs.begin(), s_InfoArgs.end(), it->second);
		if (list_it == s_InfoArgs.end())
			throw MismatchArgException(arg, true);
		else
		{
			s_InfoAssigned = true;
			s_InfoArg = it->second;
		}
	}
	else
		throw InvalidArgException(arg);
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
		if (s_InfoAssigned)
			throw InvalidUsageException(false);
		if (ArgumentPushed(it->second))
			throw MultipleArgPushException(args[0]);
		auto list_it = std::find(s_ControlArgs.begin(), s_ControlArgs.end(), it->second);
		if (list_it == s_ControlArgs.end())
			throw MismatchArgException(args[0], false);
		else
		{
			StringHandler::ToLower(&args[1]);
			ValidateControlAssignment(args[1], it->second);
			s_ControlArgTree.push_back(std::make_pair(it->second, args[1]));
		}
	}
	else
		throw InvalidArgException(args[0]);
	delete[] args;
}

bool HLSLBuilder::ArgTree::ArgumentPushed(ArgCategory category)
{
	bool found = false;
	for (auto it = s_ControlArgTree.begin(); it != s_ControlArgTree.end(); it++)
	{
		auto value = *it;
		if (value.first == category)
		{
			found = true;
			break;
		}
	}
	return found;
}

char const* HLSLBuilder::ArgException::what() const
{
	return m_Exception.c_str();
}

HLSLBuilder::BadEvaluationException::BadEvaluationException(ArgCategory category, std::string value)
{
	BuildErrorString(category, value);
}

void HLSLBuilder::BadEvaluationException::BuildErrorString(ArgCategory category, std::string value)
{
	const std::unordered_map<ArgCategory, std::string> categoryCast =
	{
		{HLSLBuilder::ArgCategory::API, "-api"},
		{HLSLBuilder::ArgCategory::API, "--target-api"},
		{HLSLBuilder::ArgCategory::CONFIG, "-c"},
		{HLSLBuilder::ArgCategory::CONFIG, "--config"}
	};

	std::stringstream error;
	error << "Invalid assignment to: ";
	auto category_it = categoryCast.find(category);
	if (category_it != categoryCast.end())
		error << category_it->second;
	error << ", the value <" << value << "> isn't allowed";
	m_Exception = error.str();
}

HLSLBuilder::InvalidArgException::InvalidArgException(std::string_view arg)
{
	BuildInvalidArgString(arg);
}

void HLSLBuilder::InvalidArgException::BuildInvalidArgString(std::string_view arg)
{
	std::stringstream messageBuffer;
	messageBuffer << "Argument: " << arg << " not found";
	m_Exception = messageBuffer.str();
}

HLSLBuilder::MismatchArgException::MismatchArgException(std::string_view arg, bool controlArg)
{
	BuildMismatchString(arg, controlArg);
}

void HLSLBuilder::MismatchArgException::BuildMismatchString(std::string_view arg, bool controlArg)
{
	std::stringstream messageBuffer;
	messageBuffer << "Invalid Argument Assigment\nArgument: " << arg;
	if (controlArg)
		messageBuffer << " is a control arg, so must be assigned";
	else
		messageBuffer << " isn't a control arg, so is unassigned";
	m_Exception = messageBuffer.str();
}

HLSLBuilder::BadAssignmentException::BadAssignmentException(std::string_view message)
{
	m_Exception = message.data();
}

HLSLBuilder::InvalidUsageException::InvalidUsageException(bool multipleInfoArg)
{
	BuildErrorString(multipleInfoArg);
}

void HLSLBuilder::InvalidUsageException::BuildErrorString(bool multipleInfoArg)
{
	m_Exception = multipleInfoArg ? "Unable to use multiple info arguments" : "Info arguments are not allowed using control args";
}

HLSLBuilder::MultipleArgPushException::MultipleArgPushException(std::string_view argName)
{
	BuildErrorString(argName);
}

void HLSLBuilder::MultipleArgPushException::BuildErrorString(std::string_view argName)
{
	std::stringstream messageBuffer;
	messageBuffer << "Argument: \"" << argName.data() << "\" insert twice.";
	m_Exception = messageBuffer.str();
}
