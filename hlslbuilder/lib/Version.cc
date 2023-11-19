#include "Version.hh"
#include <sstream>
#include "Console.hh" 

HLSLBuilder::Version::Version() :
	Minor(0), Major(0)
{
}

HLSLBuilder::Version::Version(std::string version)
{
	std::istringstream iss(version);
	std::string token;
	try
	{
		std::getline(iss, token, '.');
		Major = std::stoul(token);
		std::getline(iss, token, '.');
		Minor = std::stoul(token);
	}
	catch (const std::out_of_range& e)
	{
		Console::Error("Out of range error: ", e.what());
	}
}

HLSLBuilder::Version::Version(unsigned major, unsigned minor) :
	Major(major), Minor(minor)
{
	
}

bool HLSLBuilder::Version::operator==(Version version) const
{
	return ((this->Major == version.Major) && (this->Minor == version.Minor));
}

std::string HLSLBuilder::Version::ToString() const
{
	std::string castedVersion;
	std::stringstream buffer;
	buffer << Major << "." << Minor;
	castedVersion = buffer.str();
	return castedVersion;
}

HLSLBuilder::InvalidHLSLVersionException::InvalidHLSLVersionException(Version version)
{
	std::stringstream buffer;
	buffer << "HLSL " << version.Major << "." << version.Minor << " isn't supported or do not exists";
	m_Exception = buffer.str();
}

const char* HLSLBuilder::CompilerException::what() const
{
	return m_Exception.c_str();
}

HLSLBuilder::InvalidVulkanVersionException::InvalidVulkanVersionException(Version version)
{
	std::stringstream buffer;
	buffer << "Vulkan " << version.Major << "." << version.Minor << " isn't supported or do not exists";
	m_Exception = buffer.str();
}

HLSLBuilder::InvalidSPVSupportException::InvalidSPVSupportException()
{
	m_Exception = "Spir-V output is allowed only after HLSL 6.0";
}

HLSLBuilder::CompileErrorException::CompileErrorException(std::string error)
{
}
