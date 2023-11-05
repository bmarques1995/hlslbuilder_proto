#include <iostream>
#include <HLSLBuilder.hh>
#include <filesystem>
#include "Displayer.hh"

int main(int argc, char** argv)
{
	HLSLBuilder::Console::Init();
	for (size_t i = 1; i < argc; i++)
	{
		HLSLBuilder::ArgTree::PushRawArg(argv[i]);
	}
	try
	{
		HLSLBuilder::ArgTree::ResolveArgs();
	}
	catch(HLSLBuilder::ArgException e)
	{
		HLSLBuilder::Console::Critical("{0}\n",e.what());
		HLSLBuilderCLI::Displayer::Resolve(HLSLBuilder::ArgCategory::HELP);
		exit(2);
	}

	auto infoArgs = HLSLBuilder::ArgTree::GetInfoArgs();
	auto controlArgs = HLSLBuilder::ArgTree::GetControlArgs();
	if ((controlArgs.size() > 0) && (infoArgs.size() > 0))
	{
		HLSLBuilderCLI::Displayer::InfoControlError();
		exit(1);
	}
	if ((infoArgs.size() > 1))
	{
		HLSLBuilderCLI::Displayer::MultipleInfoError();
		exit(1);
	}
	
	while (!infoArgs.empty())
	{
		HLSLBuilderCLI::Displayer::Resolve(infoArgs.front());
		infoArgs.pop();
	}
	if(!controlArgs.empty())
		try
		{
			HLSLBuilder::Builder::SetControlArgs(controlArgs);
		}
		catch (const HLSLBuilder::MissingBuildFileException& e)
		{
			HLSLBuilder::Console::Critical("{0}\n", e.what());
		}

	return 0;
}