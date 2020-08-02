#pragma once

#include <string>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#else //UNIX
#include <dlfcn.h>
#endif

namespace yba::utils::operating_system
{
	inline void adapt_library_name(std::string& library_name)
	{
#ifdef _WIN32
		//Do nothing
#else
		//On unix-ish the library name probably needs to start with `lib`
		library_name = std::string("./lib" + library_name + ".so");
#endif
	}
	
	[[nodiscard]] inline void* load_dynamic_library(const std::string& library_name)
	{
#ifdef _WIN32
		return reinterpret_cast<void*>(LoadLibraryA(library_name.c_str()));
#else
		return dlopen(library_name.c_str(), RTLD_NOW);
#endif
	}

	inline void unload_dynamic_library(void* lib_handle)
	{
#ifdef _WIN32
		FreeLibrary(HMODULE(lib_handle));
#else
		dlclose(lib_handle);
#endif
	}

	[[nodiscard]] inline void* get_proc_address(void* lib_handle,
	                                            const std::string& function_name)
	{

#ifdef _WIN32
		return reinterpret_cast<void*>(
			GetProcAddress(HMODULE(lib_handle), LPCSTR(function_name.c_str())));
#else
		return dlsym(lib_handle, function_name.c_str());
#endif
	}
}
