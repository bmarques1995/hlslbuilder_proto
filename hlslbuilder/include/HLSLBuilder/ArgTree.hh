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

		static std::queue<ArgCategory> GetSingleArgs();
		static std::queue<std::pair<ArgCategory, std::string>> GetValuedArgs();
	private:
		static std::vector<std::string_view> s_StrArgs;

		static void ResolveRegex(std::string_view arg);
		static void PushSingleArgTreated(std::string_view arg);
		static void PushValuedArgTreated(std::sregex_token_iterator* arg);

		static const std::unordered_map<std::string_view, ArgCategory> s_ArgMapper;
		static std::unordered_map<ArgCategory, std::string_view> s_ArgValues;

		static std::queue<ArgCategory> s_SingleArgTree;
		static std::queue<std::pair<ArgCategory, std::string>> s_ValuedArgTree;
	};
}