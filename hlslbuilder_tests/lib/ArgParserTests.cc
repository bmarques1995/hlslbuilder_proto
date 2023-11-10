#include <gtest/gtest.h>
#include <iostream>
#include <HLSLBuilder/HLSLBuilder.hh>

namespace HLSLBuilder
{
	void ResolveArgList();

	TEST(HLSLBuilder, MissingSolutionFile)
	{
		ArgList::PushRawArg("-api:CSO");
		ResolveArgList();
		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();
		ArgList::ClearArgs();
		ASSERT_FALSE(controlArgs.empty());
		ASSERT_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs), HLSLBuilder::MissingSolutionFileException);
	}

	TEST(HLSLBuilder, MismatchSolutionExtension)
	{
		ArgList::PushRawArg("--build:./uchigatana");
		ResolveArgList();
		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();
		ArgList::ClearArgs();
		ASSERT_FALSE(controlArgs.empty());
		ASSERT_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs), HLSLBuilder::MismatchSolutionFileExtensionException);
	}

	TEST(HLSLBuilder, SolutionFileNotFound)
	{
		ArgList::PushRawArg("--build:./assets/project_sample.hls2");
		ResolveArgList();
		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();
		ArgList::ClearArgs();
		ASSERT_FALSE(controlArgs.empty());
		ASSERT_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs), HLSLBuilder::SolutionFileNotFoundException);
	}

	TEST(HLSLBuilder, ValidBuildArgs)
	{
		ArgList::PushRawArg("--build:./assets/project_sample/triangle.hls2");
		ArgList::PushRawArg("-api:CSO");
		ArgList::PushRawArg("-c:Release");
		std::string message;
		ResolveArgList();

		auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();
		ArgList::ClearArgs();
		ASSERT_FALSE(controlArgs.empty());
		ASSERT_NO_THROW(HLSLBuilder::ArgParser::SetControlArgs(controlArgs));
		auto build_args = HLSLBuilder::ArgParser::GetBuildInfos();
		ASSERT_EQ(build_args.m_OutputTarget, OutputTarget::CSO);
		ASSERT_EQ(build_args.m_BuildMode, BuildMode::RELEASE);
		ASSERT_STREQ(build_args.m_SolutionFilepath.c_str(), "./assets/project_sample/triangle.hls2");
	}

	void ResolveArgList()
	{
		ASSERT_NO_THROW(ArgList::ResolveArgs());
	}
}