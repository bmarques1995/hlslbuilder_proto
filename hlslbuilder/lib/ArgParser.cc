#include "ArgParser.hh"
#include "ArgParser.hh"
#include "ArgParser.hh"
#include "ArgParser.hh"
#include "Console.hh"
#include "Utils/StringHandler.hh"
#include <filesystem>

HLSLBuilder::BuildInfos HLSLBuilder::ArgParser::s_BuildInfos;

void HLSLBuilder::ArgParser::SetControlArgs(const std::list<std::pair<HLSLBuilder::ArgCategory, std::string>>& controlArgs)
{
	std::list<std::pair<HLSLBuilder::ArgCategory, std::string>> controlArgsCopy = controlArgs;
	std::unordered_map<HLSLBuilder::ArgCategory, std::string> mapControlArgs;
	while (!controlArgsCopy.empty())
	{
		auto val = controlArgsCopy.front();
		mapControlArgs[val.first] = val.second;
		controlArgsCopy.pop_front();
	}
	CastMapToBuildInfos(mapControlArgs);
}

HLSLBuilder::BuildInfos HLSLBuilder::ArgParser::GetBuildInfos()
{
	return s_BuildInfos;
}

void HLSLBuilder::ArgParser::CastMapToBuildInfos(std::unordered_map<HLSLBuilder::ArgCategory, std::string>& mappedArgs)
{
	if (mappedArgs.find(ArgCategory::BUILD) == mappedArgs.end())
		throw MissingSolutionFileException();

	std::string filepath = mappedArgs[ArgCategory::BUILD];
	
	ValidateFileExtension(filepath);
	FindFile(filepath);

	BuildMode buildMode = BuildMode::DEBUG;
	OutputTarget outputTarget = OutputTarget::CSO;

	if (mappedArgs.find(ArgCategory::CONFIG) != mappedArgs.end())
		buildMode = CastStringToBuildMode(mappedArgs[ArgCategory::CONFIG]);
	if (mappedArgs.find(ArgCategory::API) != mappedArgs.end())
		outputTarget = CastStringToOutputTarget(mappedArgs[ArgCategory::API]);

	s_BuildInfos = BuildInfos(filepath, buildMode, outputTarget);
}

void HLSLBuilder::ArgParser::ValidateFileExtension(std::string_view path)
{
	std::string filePath = path.data();
	std::regex extensionRegex(".*\\.([a-zA-Z0-9]+)$");
	std::smatch match;

	if (std::regex_match(filePath, match, extensionRegex)) {
		std::string extension = match.str(1);
		std::string expectedExtension = "hls2";
		if (expectedExtension.compare(extension) != 0)
		{
			throw MismatchSolutionFileExtensionException();
		}
	}
	else {
		throw MismatchSolutionFileExtensionException();
	}
}

void HLSLBuilder::ArgParser::FindFile(std::string_view path)
{
	if (!std::filesystem::exists(path))
		throw SolutionFileNotFoundException();
}

HLSLBuilder::BuildMode HLSLBuilder::ArgParser::CastStringToBuildMode(std::string strValue)
{
	const std::unordered_map<std::string, BuildMode> buildModeMap =
	{
		{ "debug", BuildMode::DEBUG },
		{ "release", BuildMode::RELEASE }
	};

	auto it = buildModeMap.find(strValue);
	if (it != buildModeMap.end())
		return it->second;
	else
		return BuildMode::DEBUG;
	
}

HLSLBuilder::OutputTarget HLSLBuilder::ArgParser::CastStringToOutputTarget(std::string strValue)
{
	const std::unordered_map<std::string, OutputTarget> outputTargetMap =
	{
		{ "cso", OutputTarget::CSO },
		{ "spv", OutputTarget::SPV }
	};

	auto it = outputTargetMap.find(strValue);
	if (it != outputTargetMap.end())
		return it->second;
	else
		return OutputTarget::CSO;
}

char const* HLSLBuilder::SolutionFileException::what() const
{
	return m_Message.c_str();
}

HLSLBuilder::MissingSolutionFileException::MissingSolutionFileException()
{
	m_Message = "Missing build file";
}

HLSLBuilder::MismatchSolutionFileExtensionException::MismatchSolutionFileExtensionException()
{
	m_Message = "The only accepted extension is \".hls2\"";
}

HLSLBuilder::SolutionFileNotFoundException::SolutionFileNotFoundException()
{
	m_Message = "Build file not found in given path";
}
