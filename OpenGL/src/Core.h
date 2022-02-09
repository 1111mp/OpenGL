#pragma once

#include "Config.h"

const std::string ProjectSourceDir = PROJECT_SOURCE_DIR;
const std::string AssetsDir = ASSETS_DIR;

// http://aicdg.com/debugbreak/

#ifdef __APPLE__
  #define DEBUGBREAK __builtin_debugtrap()
#else
  #define HZ_DEBUGBREAK __debugbreak()
#endif

#ifdef HZ_ENABLE_ASSERTS
  #define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK; } }
  #define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_HAZEL_ERROR("Assertion Failed: {0}", __VA_ARGS__); HZ_DEBUGBREAK; } }
#else
  #define HZ_ASSERT(x, ...)
  #define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
