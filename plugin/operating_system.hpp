#pragma once

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#endif

namespace yba::utils::operating_system
{
	void sanitize(std::string& library_name)
	{
#ifdef _WIN32
		//Do nothing
#else
		//On unix-ish the library name probably needs to start with `lib`
#endif
		
	}
	[[nodiscard]] void* load_dynamic_library(const std::string& library_name)
	{
#ifdef _WIN32
		return reinterpret_cast<void*>(LoadLibraryA(library_name.c_str()));
#else
		static_assert(false) //IMPLEMENT ME
#endif
	}

	void unload_dynamic_library(void* lib_handle)
	{
#ifdef _WIN32
		FreeLibrary(HMODULE(lib_handle));
#else
		static_assert(false) //IMPLEMENT ME
#endif
	}

	[[nodiscard]] void* get_proc_address(void* lib_handle, const std::string& function_name)
	{

#ifdef _WIN32
		return reinterpret_cast<void*>(GetProcAddress(HMODULE(lib_handle), LPCSTR(function_name.c_str())));
#else
		static_assert(false) //IMPLEMENT ME
#endif
	}
}
