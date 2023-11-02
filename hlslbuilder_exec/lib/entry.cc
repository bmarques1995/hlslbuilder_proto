#include <iostream>
#include "HLSLBuilder.hh"

int main(int argc, char** argv)
{
	HLSLBuilder::Console::Init();
	//skip the self run arg
	for (size_t i = 1; i < argc; i++)
	{
		HLSLBuilder::ArgTree::PushArg(argv[i]);
	}
	HLSLBuilder::ArgTree::ResolveArgs();
	HLSLBuilder::Console::Log("HLSL Builder Version {0}", HLSLBuilder::HLSLBuilderVersion);
	return 0;
}