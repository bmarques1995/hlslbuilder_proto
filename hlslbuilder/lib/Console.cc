#include "Console.hh"
#include <chrono>
#include <spdlog/sinks/dup_filter_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> HLSLBuilder::Console::s_Logger;

void HLSLBuilder::Console::Init()
{
	std::shared_ptr<spdlog::sinks::dup_filter_sink_mt> client_dup_filter = std::make_shared<spdlog::sinks::dup_filter_sink_mt>(std::chrono::seconds(5));
	std::vector<spdlog::sink_ptr> clientLogSinks;

	clientLogSinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

	clientLogSinks[0]->set_pattern("%^%v%$");
	client_dup_filter->add_sink(clientLogSinks[0]);

	s_Logger.reset(new spdlog::logger("HLSLBuilder", client_dup_filter));
	spdlog::register_logger(s_Logger);
	s_Logger->set_level(spdlog::level::trace);
	s_Logger->flush_on(spdlog::level::trace);
}

void HLSLBuilder::Console::End()
{
	s_Logger.reset();
}
