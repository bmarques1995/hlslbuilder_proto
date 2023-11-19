#include <iostream>
#include <HLSLBuilder.hh>
#include <filesystem>
#include "Displayer.hh"

int main(int argc, char** argv)
{
	HLSLBuilder::Console::Init();
	for (size_t i = 1; i < argc; i++)
	{
		HLSLBuilder::ArgList::PushRawArg(argv[i]);
	}
	try
	{
		HLSLBuilder::ArgList::ResolveArgs();
	}
	catch(HLSLBuilder::ArgException e)
	{
		HLSLBuilder::Console::Critical("{0}\n",e.what());
		HLSLBuilderCLI::Displayer::Resolve(HLSLBuilder::ArgCategory::HELP);
		exit(2);
	}

	auto infoArg = HLSLBuilder::ArgList::GetInfoArg();
	auto controlArgs = HLSLBuilder::ArgList::GetControlArgs();
	
	if (!controlArgs.empty())
	{
		try
		{
			HLSLBuilder::ArgParser::SetControlArgs(controlArgs);
		}
		catch (const HLSLBuilder::SolutionFileException& e)
		{
			HLSLBuilder::Console::Critical("{0}\n", e.what());
			exit(1);
		}
	}
	else
	{
		HLSLBuilderCLI::Displayer::Resolve(infoArg);
	}

	HLSLBuilder::ArgList::ClearArgs();

	auto parsedControlArgs = HLSLBuilder::ArgParser::GetBuildInfos();

	try
	{
		HLSLBuilder::SolutionParser::LoadProject(parsedControlArgs.m_SolutionFilepath);
	}
	catch (HLSLBuilder::PropertyException& e)
	{
		HLSLBuilder::Console::Critical(e.what());
	}

	Json::Value solution = *HLSLBuilder::SolutionParser::GetSolution();
	try
	{
		HLSLBuilder::Builder::SetBuildArgs(solution, parsedControlArgs.m_BuildMode, parsedControlArgs.m_OutputTarget);
		HLSLBuilder::Builder::BuildSources();
	}
	catch (HLSLBuilder::CompilerException e)
	{
		HLSLBuilder::Console::Error(e.what());
	}

	HLSLBuilder::Console::End();
	return 0;
}