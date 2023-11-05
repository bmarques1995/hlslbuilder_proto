#pragma once

#include <exception>
#include <string>
#include <cstring>
#include "ArgTree.hh"

namespace HLSLBuilder
{
	enum class BuildMode
	{
		DEBUG = 0,
		RELEASE
	};
	
	enum class OutputTarget
	{
		SPV = 0,
		CSO
	};
	
	struct BuildInfos
	{
	public:
		BuildMode m_BuildMode;
		OutputTarget m_OutputTarget;
		std::string m_Filepath;

		BuildInfos(const std::string& filepath = ".", BuildMode buildMode = BuildMode::DEBUG, OutputTarget outputTarget = OutputTarget::CSO) :
			m_Filepath(filepath), m_BuildMode(buildMode), m_OutputTarget(outputTarget)
		{
		
		}
	};

	class MissingBuildFileException : public std::exception
	{
	public:
		MissingBuildFileException();
		char const* what() const override;
	private:
		const std::string m_Message = "Missing build file";
	};
	class Builder
	{
	public:
		static void SetControlArgs(const std::queue<std::pair<HLSLBuilder::ArgCategory, std::string>>& controlArgs);
	private:
		static void CastMapToBuildInfos(std::unordered_map<HLSLBuilder::ArgCategory, std::string>& mappedArgs);
		static BuildInfos s_BuildInfos;
	};
}