#pragma once

#include "HLSLBuilderAPI.hh"
#include <string>
#include <json/json.h>
#include <exception>

namespace HLSLBuilder
{
	enum class PropertyType
	{
		NAME = 0,
		PATH_ARRAY,
		VERSION
	};

	class PropertyException : public std::exception
	{
	public:
		const char* what() const override;
	protected:
		std::string m_Exception;
	};

	/**
	* PropertyNotFoundException is an exception thrown when a mandatory property is missing.
	* 
	* More details can be found in [HLS2](@ref docs/hls2.md).
	*/
	class PropertyNotFoundException : public PropertyException
	{
	public:
		PropertyNotFoundException(std::string_view property);
	};

	/**
	* InvalidVersionException is an exception thrown when there is a mismatch in a version argument.
	* 
	* More details can be found in [HLS2](@ref docs/hls2.md).
	*/
	class InvalidVersionException : public PropertyException
	{
	public:
		InvalidVersionException(std::string_view arg);
	};

	/**
	* InvalidNameException is an exception thrown when there is a mismatch in a name argument.
	* 
	* More details can be found in [HLS2](@ref docs/hls2.md).
	*/
	class InvalidNameException : public PropertyException
	{
	public:
		InvalidNameException(std::string_view arg);
	};

	/**
	* InvalidNameException is an exception thrown when a source path is not found.
	* 
	* More details can be found in [HLS2](@ref docs/hls2.md).
	*/
	class SourceNotFoundException : public PropertyException
	{
	public:
		SourceNotFoundException(std::string_view arg);
	};

	/**
	* The SolutionParser is the static class responsible to parse the .hls2 input file to a JSON Object.
	*
	* The complete description of HLS2 can be found [here](@ref docs/hls2.md).
	*/
	class HLSLB_API SolutionParser
	{
	public:
		/**
		* LoadProject is responsible to load the `.hls2` to a JSON object, and throws a PropertyNotFoundException if a mandatory property is missing.
		*/
		static void LoadProject(std::string_view path);
		/**
		* GetSolution returns the current HLS2 instance.
		*/
		static const Json::Value* GetSolution();
	private:
		static void ValidateJSONProperties();
		static void ValidateProperty(std::pair<std::string, HLSLBuilder::PropertyType> property);
		static void ValidateVersion(std::string_view version);
		static void ValidateName(std::string_view name);
		static void ValidatePath(std::string path);
		static Json::Value s_HLS2Solution;
		static const std::list<std::pair<std::string, PropertyType>> s_ValidProperties;
	};
}


