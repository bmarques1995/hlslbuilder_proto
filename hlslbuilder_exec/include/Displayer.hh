#pragma once

#include <HLSLBuilder.hh>
#include <functional>
//#include <unordered_map>

namespace HLSLBuilderCLI
{
	class Displayer
	{
	public:
		static void Resolve(HLSLBuilder::ArgCategory category);
		static void MultipleInfoError();
		static void InfoControlError();
	private:
		static const std::unordered_map<HLSLBuilder::ArgCategory, std::function<void()>> s_ArgResolvers;
	};
}