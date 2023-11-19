#include <gtest/gtest.h>
#include <iostream>
#include <HLSLBuilder/HLSLBuilder.hh>

namespace HLSLBuilder
{
	extern void ResolveArgList();

	TEST(HLSLBuilder, MissingPropertiesHLS2)
	{
		ArgList::PushRawArg("--build:./assets/test_sample/invalidProperties.hls2");
		ArgList::PushRawArg("-api:CSO");
		ArgList::PushRawArg("-c:Release");
		std::string message;
		ResolveArgList();
		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();

		ASSERT_NO_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs));
		ArgList::ClearArgs();
		
		auto parsedControlArgs = HLSLBuilder::ArgParser::GetBuildInfos();

		ASSERT_THROW(HLSLBuilder::SolutionParser::LoadProject(parsedControlArgs.m_SolutionFilepath), HLSLBuilder::PropertyNotFoundException);
	}

	TEST(HLSLBuilder, InvalidNameHLS2)
	{
		ArgList::PushRawArg("--build:./assets/test_sample/invalidName.hls2");
		ArgList::PushRawArg("-api:CSO");
		ArgList::PushRawArg("-c:Release");
		std::string message;
		ResolveArgList();
		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();

		ASSERT_NO_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs));
		ArgList::ClearArgs();

		auto parsedControlArgs = HLSLBuilder::ArgParser::GetBuildInfos();

		ASSERT_THROW(HLSLBuilder::SolutionParser::LoadProject(parsedControlArgs.m_SolutionFilepath), HLSLBuilder::InvalidNameException);
	}

	TEST(HLSLBuilder, InvalidVersionHLS2)
	{
		ArgList::PushRawArg("--build:./assets/test_sample/invalidVersion.hls2");
		ArgList::PushRawArg("-api:CSO");
		ArgList::PushRawArg("-c:Release");
		std::string message;
		ResolveArgList();
		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();

		ASSERT_NO_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs));
		ArgList::ClearArgs();

		auto parsedControlArgs = HLSLBuilder::ArgParser::GetBuildInfos();

		ASSERT_THROW(HLSLBuilder::SolutionParser::LoadProject(parsedControlArgs.m_SolutionFilepath), HLSLBuilder::InvalidVersionException);
	}

	TEST(HLSLBuilder, InvalidPathHLS2)
	{
		ArgList::PushRawArg("--build:./assets/test_sample/invalidPath.hls2");
		ArgList::PushRawArg("-api:CSO");
		ArgList::PushRawArg("-c:Release");
		std::string message;
		ResolveArgList();
		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();

		ASSERT_NO_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs));
		ArgList::ClearArgs();

		auto parsedControlArgs = HLSLBuilder::ArgParser::GetBuildInfos();

		ASSERT_THROW(HLSLBuilder::SolutionParser::LoadProject(parsedControlArgs.m_SolutionFilepath), HLSLBuilder::SourceNotFoundException);
	}

	TEST(HLSLBuilder, ValidHLS2)
	{
		ArgList::PushRawArg("--build:./assets/test_sample/valid.hls2");
		ArgList::PushRawArg("-api:CSO");
		ArgList::PushRawArg("-c:Release");
		std::string message;
		ResolveArgList();
		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();

		ASSERT_NO_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs));
		ArgList::ClearArgs();

		auto parsedControlArgs = HLSLBuilder::ArgParser::GetBuildInfos();

		ASSERT_NO_THROW(HLSLBuilder::SolutionParser::LoadProject(parsedControlArgs.m_SolutionFilepath));
	}
}