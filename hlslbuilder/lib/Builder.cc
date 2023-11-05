#include "Builder.hh"
#include "Utils/StringHandler.hh"

HLSLBuilder::BuildInfos HLSLBuilder::Builder::s_BuildInfos;

HLSLBuilder::MissingBuildFileException::MissingBuildFileException()
{
}

char const* HLSLBuilder::MissingBuildFileException::what() const
{
	return m_Message.c_str();
}

void HLSLBuilder::Builder::SetControlArgs(const std::queue<std::pair<HLSLBuilder::ArgCategory, std::string>>& controlArgs)
{
	std::queue<std::pair<HLSLBuilder::ArgCategory, std::string>> controlArgsCopy = controlArgs;
	std::unordered_map<HLSLBuilder::ArgCategory, std::string> mapControlArgs;
	while (!controlArgsCopy.empty())
	{
		auto val = controlArgsCopy.front();
		mapControlArgs[val.first] = val.second;
		controlArgsCopy.pop();
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
	
	s_BuildInfos = BuildInfos(filepath, BuildMode::DEBUG, OutputTarget::CSO);
}
