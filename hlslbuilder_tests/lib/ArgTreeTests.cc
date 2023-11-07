#include <gtest/gtest.h>
#include <iostream>
#include <HLSLBuilder/HLSLBuilder.hh>

namespace HLSLBuilder
{
    TEST(HLSLBuilder, InvalidArg)
    {
        ArgTree::PushRawArg("--chaos");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
        ASSERT_STREQ(message.c_str(), "Argument: --chaos not found");
    }

	TEST(HLSLBuilder, MultiAssignment)
	{
		ArgTree::PushRawArg("--build:./:./uchigatana");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
		ASSERT_STREQ(message.c_str(), "Invalid Argument Assigment Evaluation, neither argument accept two values");
	}

	TEST(HLSLBuilder, MismatchControlAssignment)
	{
		ArgTree::PushRawArg("--build");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
		ASSERT_STREQ(message.c_str(), "Invalid Argument Assigment\nArgument: --build is a control arg, so must be assigned");
	}

	TEST(HLSLBuilder, MismatchInfoAssignment)
	{
		ArgTree::PushRawArg("--version:chaos");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
		ASSERT_STREQ(message.c_str(), "Invalid Argument Assigment\nArgument: --version isn't a control arg, so is unassigned");
	}

	TEST(HLSLBuilder, MultipeInfoPush)
	{
		ArgTree::PushRawArg("--version");
		ArgTree::PushRawArg("-h");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
		ASSERT_STREQ(message.c_str(), "Unable to use multiple info arguments");
	}

	TEST(HLSLBuilder, MultipleControlPush)
	{
		ArgTree::PushRawArg("--build:./");
		ArgTree::PushRawArg("-b:./chaos");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
		ASSERT_STREQ(message.c_str(), "Argument: \"-b\" insert twice.");
	}

	TEST(HLSLBuilder, InvalidAssignment)
	{
		ArgTree::PushRawArg("-c:CSOs");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
		ASSERT_STREQ(message.c_str(), "Invalid assignment to: -c, the value <csos> isn't allowed");
	}

	TEST(HLSLBuilder, InvalidControlUsage)
	{
		ArgTree::PushRawArg("-api:CSO");
		ArgTree::PushRawArg("--version");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
		ASSERT_STREQ(message.c_str(), "Info arguments are not allowed using control args");
	}

	TEST(HLSLBuilder, InvalidInfoUsage)
	{
		ArgTree::PushRawArg("--version");
		ArgTree::PushRawArg("-api:CSO");
		std::string message;
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
			message = e.what();
		}
		ArgTree::ClearArgs();
		ASSERT_STREQ(message.c_str(), "Info arguments are not allowed using control args");
	}

	TEST(HLSLBuilder, ValidInfoUsage)
	{
		ArgTree::PushRawArg("--version");
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
		}
		auto arg = ArgTree::GetInfoArg();
		ArgTree::ClearArgs();
		ASSERT_EQ(arg, HLSLBuilder::ArgCategory::VERSION);
	}

	TEST(HLSLBuilder, ValidControlUsage)
	{
		ArgTree::PushRawArg("-api:CSO");
		ArgTree::PushRawArg("-b:./");
		std::list<std::pair<ArgCategory, std::string>> expectedValues = 
		{
			{ArgCategory::API, "cso"},
			{ArgCategory::BUILD, "./"}
		};
		try
		{
			ArgTree::ResolveArgs();
		}
		catch (const ArgException& e)
		{
		}
		auto arg = ArgTree::GetControlArgs();
		ArgTree::ClearArgs();

		ASSERT_EQ(expectedValues.size(), arg.size());
		auto expected_it = expectedValues.begin();
		for (auto it = arg.begin(); it != arg.end(); expected_it++ ,it++)
		{
			ASSERT_EQ(expected_it->first, it->first);
			ASSERT_STREQ(expected_it->second.c_str(), it->second.c_str());
		}
	}
}