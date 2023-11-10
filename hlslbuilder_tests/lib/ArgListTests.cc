#include <gtest/gtest.h>
#include <iostream>
#include <HLSLBuilder/HLSLBuilder.hh>

namespace HLSLBuilder
{
    TEST(HLSLBuilder, InvalidArg)
    {
        ArgList::PushRawArg("--chaos");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::InvalidArgException);
		ArgList::ClearArgs();
    }

	TEST(HLSLBuilder, MultiAssignment)
	{
		ArgList::PushRawArg("--build:./:./uchigatana");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::BadAssignmentException);
		ArgList::ClearArgs();
	}

	TEST(HLSLBuilder, MismatchControlAssignment)
	{
		ArgList::PushRawArg("--build");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::MismatchArgException);
		ArgList::ClearArgs();
	}

	TEST(HLSLBuilder, MismatchInfoAssignment)
	{
		ArgList::PushRawArg("--version:chaos");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::MismatchArgException);
		ArgList::ClearArgs();
	}

	TEST(HLSLBuilder, MultipeInfoPush)
	{
		ArgList::PushRawArg("--version");
		ArgList::PushRawArg("-h");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::InvalidUsageException);
		ArgList::ClearArgs();
	}

	TEST(HLSLBuilder, MultipleControlPush)
	{
		ArgList::PushRawArg("--build:./");
		ArgList::PushRawArg("-b:./chaos");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::MultipleArgPushException);
		ArgList::ClearArgs();
	}

	TEST(HLSLBuilder, InvalidAssignment)
	{
		//ij
		ArgList::PushRawArg("-c:CSOs");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::BadEvaluationException);
		ArgList::ClearArgs();
	}

	TEST(HLSLBuilder, InvalidControlUsage)
	{
		ArgList::PushRawArg("-api:CSO");
		ArgList::PushRawArg("--version");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::InvalidUsageException);
		ArgList::ClearArgs();
	}

	TEST(HLSLBuilder, InvalidInfoUsage)
	{
		ArgList::PushRawArg("--version");
		ArgList::PushRawArg("-api:CSO");
		ASSERT_THROW(ArgList::ResolveArgs(), HLSLBuilder::InvalidUsageException);
		ArgList::ClearArgs();
	}

	TEST(HLSLBuilder, ValidInfoUsage)
	{
		ArgList::PushRawArg("--version");
		ASSERT_NO_THROW(ArgList::ResolveArgs());
		auto arg = ArgList::GetInfoArg();
		ArgList::ClearArgs();
		ASSERT_EQ(arg, HLSLBuilder::ArgCategory::VERSION);
	}

	TEST(HLSLBuilder, ValidControlUsage)
	{
		ArgList::PushRawArg("-api:CSO");
		ArgList::PushRawArg("-b:./");
		std::list<std::pair<ArgCategory, std::string>> expectedValues = 
		{
			{ArgCategory::API, "cso"},
			{ArgCategory::BUILD, "./"}
		};
		ASSERT_NO_THROW(ArgList::ResolveArgs());
		auto arg = ArgList::GetControlArgs();
		ArgList::ClearArgs();

		ASSERT_EQ(expectedValues.size(), arg.size());
		auto expected_it = expectedValues.begin();
		for (auto it = arg.begin(); it != arg.end(); expected_it++ ,it++)
		{
			ASSERT_EQ(expected_it->first, it->first);
			ASSERT_STREQ(expected_it->second.c_str(), it->second.c_str());
		}
	}
}