#pragma once

#include <string>
#include "HLSLBuilderAPI.hh"

namespace HLSLBuilder
{
	class HLSLB_API StringHandler
	{
	public:
		static void ToLower(std::string* content);
		static void ToUpper(std::string* content);
	};
}