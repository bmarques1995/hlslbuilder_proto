#pragma once

#include <string>
#include <exception>

namespace HLSLBuilder
{
	struct Version
	{
		unsigned Major;
		unsigned Minor;

		Version();

		Version(std::string version);
		Version(unsigned major, unsigned minor);

		bool operator==(Version version) const;
		std::string ToString() const;
	};

	class CompilerException : public std::exception
	{
	public:
		const char* what() const override;
	protected:
		std::string m_Exception;
	};

	class InvalidHLSLVersionException : public CompilerException
	{
	public:
		InvalidHLSLVersionException(Version version);
	};

	class InvalidVulkanVersionException : public CompilerException
	{
	public:
		InvalidVulkanVersionException(Version version);
	};

	class InvalidSPVSupportException : public CompilerException
	{
	public:
		InvalidSPVSupportException();
	};

	class CompileErrorException : public CompilerException
	{
	public:
		CompileErrorException(std::string error);
	};
}