#include "Builder.hh"
#include "Console.hh"
#include "Utils/StringHandler.hh"
#include <filesystem>

HLSLBuilder::BuildInfos HLSLBuilder::Builder::s_BuildInfos;

void HLSLBuilder::Builder::SetControlArgs(const std::list<std::pair<HLSLBuilder::ArgCategory, std::string>>& controlArgs)
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

void HLSLBuilder::Builder::CastMapToBuildInfos(std::unordered_map<HLSLBuilder::ArgCategory, std::string>& mappedArgs)
{
	const std::unordered_map<std::string, BuildMode> buildModeMap =
	{
		{ "debug", BuildMode::DEBUG },
		{ "release", BuildMode::RELEASE }
	};

	const std::unordered_map<std::string, OutputTarget> outputTargetMap =
	{
		{ "cso", OutputTarget::CSO },
		{ "spv", OutputTarget::SPV }
	};

	if (mappedArgs.find(ArgCategory::BUILD) == mappedArgs.end())
		throw MissingBuildFileException();

	std::string filepath = mappedArgs[ArgCategory::BUILD];
	
	ValidateFileExtension(filepath);
	FindFile(filepath);

	s_BuildInfos = BuildInfos(filepath, BuildMode::DEBUG, OutputTarget::CSO);
}

void HLSLBuilder::Builder::ValidateFileExtension(std::string_view path)
{
	std::string filePath = path.data();
	std::regex extensionRegex(".*\\.([a-zA-Z0-9]+)$");
	std::smatch match;

	if (std::regex_match(filePath, match, extensionRegex)) {
		std::string extension = match.str(1);
		std::string expectedExtension = "hls2";
		if (expectedExtension.compare(extension) != 0)
		{
			throw MismatchBuildFileExtensionException();
		}
	}
	else {
		throw MismatchBuildFileExtensionException();
	}
}

void HLSLBuilder::Builder::FindFile(std::string_view path)
{
	if (!std::filesystem::exists(path))
		throw BuildFileNotFoundException();
}

char const* HLSLBuilder::BuildFileException::what() const
{
	return m_Message.c_str();
}

HLSLBuilder::MissingBuildFileException::MissingBuildFileException()
{
	m_Message = "Missing build file";
}

HLSLBuilder::MismatchBuildFileExtensionException::MismatchBuildFileExtensionException()
{
	m_Message = "The only accepted extension is \".hls2\"";
}

HLSLBuilder::BuildFileNotFoundException::BuildFileNotFoundException()
{
	m_Message = "Build file not found in given path";
}
