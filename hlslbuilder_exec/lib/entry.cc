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
	HLSLBuilder::ArgTree::ResolveArgs();
	auto singleArgs = HLSLBuilder::ArgTree::GetSingleArgs();
	while (!singleArgs.empty())
	{
		HLSLBuilderCLI::Displayer::Resolve(singleArgs.front());
		singleArgs.pop();
	}
	return 0;
}