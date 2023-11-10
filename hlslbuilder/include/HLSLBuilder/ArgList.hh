#pragma once

#include "HLSLBuilderAPI.hh"
#include <sstream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <queue>
#include <regex>
#include <exception>

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

	/**
	* ArgException is a base exception that stores the message string and the getter of the message.
	*/
	class ArgException : public std::exception
	{
	public:
		char const* what() const override;
	protected:
		std::string m_Exception;
	};

	/**
	* InvalidArgException is an exception thrown when an unknown arg is pushed.
	*/
	class InvalidArgException : public ArgException
	{
	public:
		InvalidArgException(std::string_view arg);
	private:
		void BuildInvalidArgString(std::string_view arg);
	};

	/**
	* MismatchArgException is an exception thrown when an info arg is assigned or a control arg is not assigned.
	*/
	class MismatchArgException : public ArgException
	{
	public:
		MismatchArgException(std::string_view arg, bool controlArg);
	private:
		void BuildMismatchString(std::string_view arg, bool controlArg);
	};

	/**
	* BadAssignmentException is an exception thrown when an arg receives two or more values.
	*/
	class BadAssignmentException : public ArgException
	{
	public:
		BadAssignmentException(std::string_view message);
	};

	/**
	* BadEvaluationException is an exception thrown when an arg is assigned with an unallowed value.
	*/
	class BadEvaluationException : public ArgException
	{
	public:
		BadEvaluationException(ArgCategory category, std::string value);
	private:
		void BuildErrorString(ArgCategory category, std::string value);
	};

	/**
	* InvalidUsageException is an exception thrown when a control arg is used beside an info arg.
	*/
	class InvalidUsageException : public ArgException
	{
	public:
		InvalidUsageException(bool multipleInfoArg);
	private:
		void BuildErrorString(bool multipleInfoArg);
	};

	/**
	* MultipleArgPushException is an exception thrown when an arg is push two or more times.
	*/
	class MultipleArgPushException : public ArgException
	{
	public:
		MultipleArgPushException(std::string_view argName);
	private:
		void BuildErrorString(std::string_view argName);
	};

	/**
	* The ArgTree is a static class to wrap all necessary arguments to be sent to D3DCompiler (until HLSL 5.1) or DXC (since HLSL 6.0)
	* When an argument is pushed, it is treated, and, if passes all filters, is stored on a queue to be processed later. All arguments
	* and parameters work with a whitelist system, block unknown args and assignments to be pushed.
	* 
	* There are two types of argument: info and control. Info args tells how the program works, otherside, control args pass necessary values to further processing.
	* The complete list of avaliable arguments can be found in [Properties](@ref docs/properties.md).
	* 
	* There is 6 types of filters, that are detected with a base ArgException, thrown when an error occurs:
	*   * Multiple evaluation: if an arg receives more than one value, is thrown a BadAssignmentException.
	*	* Invalid argument: if the argument doesn't match the valid argument list, is thrown an InvalidArgException.
	*	* Mismatch control and info args: if the argument is mismatched, it generates a MismatchArgException.
	*   * Bad Evaluation: if is sent an invalid argument, a BadEvaluationException will be generated.
	*   * Multiple Push: if an argument is pushed more than one time, a MultipleArgPushException will be generated.
	*   * Invalid Usage: there can be only one info arg, and if an info arg is called, there can't be control args, if this situation occurs, an InvalidUsageException is thrown.
	*/
	class HLSLB_API ArgList
	{
	public:
		/**
		* Push arg allows the user push arguments to call the compiler, 
		* 
		* 
		*/
		static void PushRawArg(std::string_view arg);

		/**
		 * Resolve args will reallocate and validate all pushed args, in category and value.
		 *
		 */

		 /* now is time to test the [Class linkage](@ref ProjectReader) */
		static void ResolveArgs();

		/**
		* Clear all insert args and allows new arg list
		*/
		static void ClearArgs();

		/**
		*
		* Get the info arg to be processed
		* 
		*/
		static ArgCategory GetInfoArg();

		/**
		*
		* Get the control args to be processed, structured in a FIFO format, in a list.
		* Stored in a pair mode, with the casted arg category and its value
		*
		*/
		static std::list<std::pair<ArgCategory, std::string>> GetControlArgs();
	private:
		static std::vector<std::string_view> s_StrArgs;

		static void ClassifyAndEvaluateArgs(std::string_view arg);
		static void ValidateControlAssignment(std::string_view value, HLSLBuilder::ArgCategory category);
		static void PushInfoArgTreated(std::string_view arg);
		static void PushControlArgTreated(std::sregex_token_iterator* arg);
		static bool ArgumentPushed(ArgCategory category);

		static const std::unordered_map<std::string_view, ArgCategory> s_ArgMapper;
		static const std::unordered_map<std::string_view, ArgCategory> s_ValidAssignments;
		static std::unordered_map<ArgCategory, std::string_view> s_ArgValues;

		static const std::list<ArgCategory> s_InfoArgs;
		static const std::list<ArgCategory> s_ControlArgs;

		static ArgCategory s_InfoArg;
		static bool s_InfoAssigned;
		static std::list<std::pair<ArgCategory, std::string>> s_ControlArgTree;
	};
}