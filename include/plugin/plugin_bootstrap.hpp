#pragma once
#define ___YPLUGS_BOOTSTRAP_PROC_NAME __y_plugs_plugin_entry_point
#define ___YPLUGS_BOOTSTRAP_XSTR(s) ___YPLUGS_BOOTSTRAP_STR(s)
#define ___YPLUGS_BOOTSTRAP_STR(s) #s
#define ___YPLUGS_BOOTSTRAP_PROC_NAME_STR                                      \
	___YPLUGS_BOOTSTRAP_STR(__y_plugs_plugin_entry_point)
#ifdef _WIN32
#define ___YPLUGS_BOOTSTRAP_EXPORT_SYMBOL _declspec(dllexport)
#else
#define ___YPLUGS_BOOTSTRAP_EXPORT_SYMBOL
#endif
#define YPLUGS_BOOTSRAP_SIGNATURE                                              \
	extern "C" ___YPLUGS_BOOTSTRAP_EXPORT_SYMBOL yba::plugin*                  \
		___YPLUGS_BOOTSTRAP_PROC_NAME
