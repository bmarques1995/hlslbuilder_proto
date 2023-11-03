#include <iostream>
#include "HLSLBuilder.hh"
#include <filesystem>

int main(int argc, char** argv)
{
	HLSLBuilder::Console::Init();
	//skip the self run arg
	for (size_t i = 1; i < argc; i++)
	{
		HLSLBuilder::ArgTree::PushRawArg(argv[i]);
	}
	HLSLBuilder::ArgTree::ResolveArgs();
	HLSLBuilder::Console::Log("HLSL Builder Version {0}", HLSLBuilder::HLSLBuilderVersion);
	return 0;
}