//Implement an yba::plugin interface in a dynamic library
#include <cstdio>

#include "plugin/plugin.hpp"
#include "my_plugin.hpp"

class my_plugin_impl : public my_plugin
{
	bool init() final
	{
		printf("my_plugin init (dynamic init by plugin manager)\n");
		return true;
	}

	bool quit() final
	{
		printf("my_plugin quit (dynamic un-init by plugin manager)\n");
		return true;
	}

public:
	my_plugin_impl() { printf("my_plugin ctor (deterministic init)\n"); }

	~my_plugin_impl() { printf("my_plugin dtor (deterministic un-init)\n"); }

	[[nodiscard]] const char* get_data() const final
	{
		return "Is this thing even working?";
	}
};

YPLUGS_BOOTSRAP_SIGNATURE()
{
	return reinterpret_cast<yba::plugin*>(new my_plugin_impl);
}
