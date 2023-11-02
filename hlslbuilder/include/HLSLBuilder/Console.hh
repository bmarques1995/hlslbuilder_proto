#pragma once

#include "HLSLBuilderAPI.hh"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

template<typename... Args>
using format_string_t = std::string_view;

namespace HLSLBuilder
{
    class HLSLB_API Console
    {
    public:
        static void Init();
        static void End();

        template<typename... Args>
        static inline void Debug(format_string_t<Args...> fmt, Args &&... args)
        {
            s_Logger->trace(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static inline void Log(format_string_t<Args...> fmt, Args &&... args)
        {
            s_Logger->info(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static inline void Warn(format_string_t<Args...> fmt, Args &&... args)
        {
            s_Logger->warn(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static inline void Error(format_string_t<Args...> fmt, Args &&... args)
        {
            s_Logger->error(fmt, std::forward<Args>(args)...);
        }

        template<typename... Args>
        static inline void Critical(format_string_t<Args...> fmt, Args &&... args)
        {
            s_Logger->critical(fmt, std::forward<Args>(args)...);
        }
    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
    };
}