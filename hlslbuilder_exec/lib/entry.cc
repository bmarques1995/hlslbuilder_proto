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

	auto infoArgs = HLSLBuilder::ArgTree::GetInfoArg();
	auto controlArgs = HLSLBuilder::ArgTree::GetControlArgs();
	
	if (!controlArgs.empty())
	{
		try
		{
			HLSLBuilder::Builder::SetControlArgs(controlArgs);
			//HLSLBuilder::Builder
		}
		catch (const HLSLBuilder::BuildFileException& e)
		{
			HLSLBuilder::Console::Critical("{0}\n", e.what());
			exit(1);
		}
	}

	HLSLBuilder::Console::End();
	return 0;
}