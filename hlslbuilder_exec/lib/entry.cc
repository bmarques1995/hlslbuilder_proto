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

	auto singleArgs = HLSLBuilder::ArgTree::GetInfoArgs();
	auto valuedArgs = HLSLBuilder::ArgTree::GetControlArgs();
	if ((valuedArgs.size() > 0) && (singleArgs.size() > 0))
	{
		HLSLBuilderCLI::Displayer::InfoControlError();
		exit(1);
	}
	if ((singleArgs.size() > 1))
	{
		HLSLBuilderCLI::Displayer::MultipleInfoError();
		exit(1);
	}
	
	while (!singleArgs.empty())
	{
		HLSLBuilderCLI::Displayer::Resolve(singleArgs.front());
		singleArgs.pop();
	}
	return 0;
}