#include "Builder.hh"
#include <sstream>
#include <filesystem>
#include <regex>
#include "Console.hh"
#include "Utils/FileHandler.hh"
#include "SolutionParser.hh"

std::list<HLSLBuilder::Source> HLSLBuilder::Builder::s_Sources;
HLSLBuilder::Version HLSLBuilder::Builder::s_HLSLVersion;
HLSLBuilder::Version HLSLBuilder::Builder::s_VulkanVersion;
HLSLBuilder::BuildMode HLSLBuilder::Builder::s_BuildMode;
HLSLBuilder::OutputTarget HLSLBuilder::Builder::s_OutputTarget;
Json::Value HLSLBuilder::Builder::s_Output;

void HLSLBuilder::Builder::SetBuildArgs(Json::Value& solution, BuildMode buildMode, OutputTarget outputTarget)
{
	s_HLSLVersion = Version(solution["HLSLVersion"].asString());
	ValidateHLSLVersion();
	s_VulkanVersion = Version(solution["VulkanVersion"].asString());
	ValidateVulkanVersion();
	s_BuildMode = buildMode;
	s_OutputTarget = outputTarget;
	ValidateOutputTarget();
	for (unsigned i = 0; i < solution["GraphicsPipelineSources"].size(); i++)
	{
		std::filesystem::path tempPath(solution["GraphicsPipelineSources"][i].asString());
		if (tempPath.is_absolute())
		{
			std::string sourcePath = tempPath.string();
			std::replace(sourcePath.begin(), sourcePath.end(), '\\', '/');
			Source src(sourcePath);
			s_Sources.push_back(src);
		}
		else
		{
			std::regex pattern("^(\\.\\\\|\\./|\\\\|/)");
			std::string pathStr = solution["GraphicsPipelineSources"][i].asString();
			std::string result;
			result = std::regex_replace(pathStr, pattern, "");
			std::string sourcePath =
				(std::filesystem::path(solution["RunningPath"].asString()) / std::filesystem::path(result)).string();
			std::replace(sourcePath.begin(), sourcePath.end(), '\\', '/');
			Source src(sourcePath);
			s_Sources.push_back(src);
		}
	}
}

void HLSLBuilder::Builder::BuildSources()
{
	if(s_OutputTarget == OutputTarget::SPV)
		s_Output["BinVersion"] = s_VulkanVersion.ToString();
	else
		s_Output["BinVersion"] = s_HLSLVersion.ToString();
	uint32_t count = 0u;
	for (auto& i : s_Sources)
	{
		i.CompileVertexShader(s_BuildMode, s_OutputTarget, s_HLSLVersion, s_VulkanVersion);
		i.CompilePixelShader(s_BuildMode, s_OutputTarget, s_HLSLVersion, s_VulkanVersion);
		s_Output["GraphicsShaders"][count] = *(i.GetProperties());
		count++;
	}
	Json::Value solution = *(SolutionParser::GetSolution());
	std::stringstream outputFile;
	outputFile << solution["ProjectName"].asString();
	if (s_OutputTarget == OutputTarget::SPV)
		outputFile << ".spvc";
	else
		outputFile << ".csoc";
	outputFile << ".json";
	std::filesystem::path outputPath = std::filesystem::path(solution["RunningPath"].asString()) / std::filesystem::path(outputFile.str());
	FileHandler::WriteTextFile(outputPath.string(), s_Output.toStyledString().c_str());
}

void HLSLBuilder::Builder::ValidateHLSLVersion()
{
	const std::list<Version> validHLSL =
	{
		{ Version(4,0) },
		{ Version(5,0) },
		{ Version(5,1) },
		{ Version(6,0) },
		{ Version(6,1) },
		{ Version(6,2) },
		{ Version(6,3) },
		{ Version(6,4) },
		{ Version(6,5) },
		{ Version(6,6) },
		{ Version(6,7) }
	};

	auto it = std::find(validHLSL.begin(), validHLSL.end(), s_HLSLVersion);
	if (it == validHLSL.end())
		throw InvalidHLSLVersionException(s_HLSLVersion);
}

void HLSLBuilder::Builder::ValidateVulkanVersion()
{
	const std::list<Version> validVulkan =
	{
		{ Version(1,0) },
		{ Version(1,1) },
		{ Version(1,2) },
		{ Version(1,3) }
	};

	auto it = std::find(validVulkan.begin(), validVulkan.end(), s_VulkanVersion);
	if (it == validVulkan.end())
		throw InvalidHLSLVersionException(s_HLSLVersion);
}

void HLSLBuilder::Builder::ValidateOutputTarget()
{
	if ((s_HLSLVersion.Major < 6) && (s_OutputTarget == OutputTarget::SPV))
		throw InvalidSPVSupportException();
}
