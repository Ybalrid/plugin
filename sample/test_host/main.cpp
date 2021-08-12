#include <cstdio>

#include "plugin/plugin_manager.hpp"
#include "my_plugin.hpp"

/*
 * Test program:
 *  - create an yba::plugin_interface
 *  - load a "my_plugin" defined in the test_interface in a plugin called "test"
 *  - call a method implemented in "my_plugin"
 *  - let the plugin_manager go out of scope, and see all thing cleanly unload
 */

int main()
{
	try
	{
		yba::plugin_manager my_plugin_manager;
		const auto* the_plugin
			= dynamic_cast<my_plugin*>(my_plugin_manager.load_plugin("test"));

		printf("%s\n", the_plugin->get_data());
	}
	catch(const std::exception& e)
	{
		fprintf(stderr, "%s\n", e.what());
		return -1;
	}

	return 0;
}
