#include "Displayer.hh"

const std::unordered_map<HLSLBuilder::ArgCategory, std::function<void()>> HLSLBuilderCLI::Displayer::s_ArgResolvers = 
{
	{
        HLSLBuilder::ArgCategory::HELP, []()
	    {
		    HLSLBuilder::Console::Log("HLSLBuilder commands:");
		    HLSLBuilder::Console::Log("usage: HLSLBuilder version [-v | --version] [-h | --help]");
		    HLSLBuilder::Console::Log("[-b/--build]:<build_file_path>(must match a .hls2)");
		    HLSLBuilder::Console::Log("[-c/--config]:<build_mode>(Debug/Release: can be either upper or lower case)");
		    HLSLBuilder::Console::Log("[-api/--target-api]:<binary_output>(SPV/CSO: can be either upper or lower case)");
		    HLSLBuilder::Console::Log("[-h/--help] and [-v/--version], if used, can't receive another arg,\nif used with more args, and exception will be thrown, after detected the second arg");
	    }
	},
	{
        HLSLBuilder::ArgCategory::VERSION, []()
        {
            HLSLBuilder::Console::Log("HLSLBuilder version {0}", HLSLBuilder::HLSLBuilderVersion);
        }
    }
};

void HLSLBuilderCLI::Displayer::Resolve(HLSLBuilder::ArgCategory category)
{
    auto it = s_ArgResolvers.find(category);
    if (it != s_ArgResolvers.end())
        it->second();
}
