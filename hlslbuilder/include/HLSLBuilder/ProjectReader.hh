#pragma once

#include "HLSLBuilderAPI.hh"
#include <string>

namespace HLSLBuilder
{
	class HLSLB_API ProjectReader
	{
	public:
		/**
		 * ... Hello Doxygen ...
		 */
		static void LoadProject(std::string_view path);
	};
}


