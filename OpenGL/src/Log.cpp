#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel {

  Ref<spdlog::logger> Log::s_HazelLogger;
  Ref<spdlog::logger> Log::s_ClientLogger;

  void Log::Init()
  {
    spdlog::set_pattern("%^[%T] %n: %v%$");

    s_HazelLogger = spdlog::stdout_color_mt("HAZEL");
    s_HazelLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);
  }

}
