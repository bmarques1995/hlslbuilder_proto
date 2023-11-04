#pragma once

#include "HLSLBuilderAPI.hh"
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <queue>
#include <regex>

namespace HLSLBuilder
{
	enum class ArgCategory
	{
		BUILD = 0,
		API,
		CONFIG,
		HELP,
		VERSION
	};

	class HLSLB_API ArgTree
	{
	public:
		/**
		 * Push arg allows the user push arguments to call the compiler, 
		 * the complete list of avaliable arguments can be found in [Properties](@ref docs/properties.md).
		 * 
		 */

		/* now is time to test the [Class linkage](@ref ProjectReader) */
		static void PushRawArg(std::string_view arg);

		/**
		 * Resolve args will reallocate and validate all pushed args, in category and value.
		 *
		 */

		 /* now is time to test the [Class linkage](@ref ProjectReader) */
		static void ResolveArgs();

		static std::queue<ArgCategory> GetInfoArgs();
		static std::queue<std::pair<ArgCategory, std::string>> GetControlArgs();
	private:
		static std::vector<std::string_view> s_StrArgs;

		static void ResolveRegex(std::string_view arg);
		static void PushInfoArgTreated(std::string_view arg);
		static void PushControlArgTreated(std::sregex_token_iterator* arg);

		static const std::unordered_map<std::string_view, ArgCategory> s_ArgMapper;
		static std::unordered_map<ArgCategory, std::string_view> s_ArgValues;

		static const std::list<ArgCategory> s_InfoArgs;
		static const std::list<ArgCategory> s_ControlArgs;

		static std::queue<ArgCategory> s_InfoArgTree;
		static std::queue<std::pair<ArgCategory, std::string>> s_ControlArgTree;
	};
}