#pragma once

#include <exception>
#include <string>
#include <cstring>
#include "ArgList.hh"

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
	
	/**
	* Contains all infos to build the project:
	*	* SolutionPreProcessor file: *.hls2
	*   * Build mode: debug/release
	*   * Build api: spv(vulkan target)/cso(d3d target)
	*/
	struct HLSLB_API BuildInfos
	{
	public:
		BuildMode m_BuildMode;
		OutputTarget m_OutputTarget;
		std::string m_SolutionFilepath;

		BuildInfos() :
			m_SolutionFilepath("."),
			m_OutputTarget(OutputTarget::CSO),
			m_BuildMode(BuildMode::DEBUG)
		{
		
		}
		
		BuildInfos(const std::string& filepath, BuildMode buildMode = BuildMode::DEBUG, OutputTarget outputTarget = OutputTarget::CSO) :
			m_SolutionFilepath(filepath), m_BuildMode(buildMode), m_OutputTarget(outputTarget)
		{
		
		}
	};

	/**
	* Common exception of solution file, mother class for these 3 exception:
	*	* MissingSolutionFileException
	*   * MismatchSolutionFileExtensionException
	*   * SolutionFileNotFoundException
	*/
	class SolutionFileException : public std::exception
	{
	public:
		char const* what() const override;
	protected:
		std::string m_Message;
	};

	/**
	* Thrown when the -b arg is not set.
	*/
	class MissingSolutionFileException : public SolutionFileException
	{
	public:
		MissingSolutionFileException();
	};

	/**
	* Thrown when the extension doesn't match ".hls2".
	*/
	class MismatchSolutionFileExtensionException : public SolutionFileException
	{
	public:
		MismatchSolutionFileExtensionException();
	};

	/**
	* Thrown when the ".hls2" file is not found.
	*/
	class SolutionFileNotFoundException : public SolutionFileException
	{
	public:
		SolutionFileNotFoundException();
	};

	/**
	* A class to convert the string cmd args to proper args.
	*/
	class HLSLB_API ArgParser
	{
	public:
		/**
		* Receives the arg list and parse then to proper enums and filter the file input 
		*/
		static void SetControlArgs(const std::list<std::pair<HLSLBuilder::ArgCategory, std::string>>& controlArgs);
		/**
		* Return all build infos
		*/
		static BuildInfos GetBuildInfos();
	private:
		static void CastMapToBuildInfos(std::unordered_map<HLSLBuilder::ArgCategory, std::string>& mappedArgs);
		static void ValidateFileExtension(std::string_view path);
		static void FindFile(std::string_view path);
		static BuildMode CastStringToBuildMode(std::string strValue);
		static OutputTarget CastStringToOutputTarget(std::string strValue);
		static BuildInfos s_BuildInfos;
	};
}