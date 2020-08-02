# yba::plugin

> This is a WIP project

A simple and safe C++ plugin system as a header only library

## principle

Givein a program (called Host program) that want to load code from a dynamic library (called Plugin library) without any build time dependancy beside an interface definition, this library permit you to do so, really simply by doing the following:

Host program instanciate a `plugin_manager` object

Host program and Plugin library share an interface that derive from `plugin`

Plugin library implements the interface mentionned above, and give a *single exported function* that create an object of that type on the heap and return a pointer.

`plugin_manager` load the library via it's name in an os defined way. It then can access that function and create a `plugin` object from it, and automatically manages it's lifetime. It returns to you a plugin object that you can trust is an instance of the plugin interface, and recast.


Consider the content of the 3 "test" directories. `test` contains the plugin implementation, `test_host` is a program that wants to load external code in a plugin, and `test_interface` define the plugin interface.

An `yba::plugin` instance's lifetime is defined by 4 events : 

1. Plugin object is created (constructor)
2. Plugin has been loaded by plugin manager (`bool init()`)
3. Plugin has been unloaded by plugin manager (`bool quit()`)
4. Plugin object is destroyed (destructor)

Here's the common interface that is shared between the host program and the plugin (`test_interface/my_plugin.hpp`)

```cpp
#pragma once
#include "plugin/plugin.hpp"

//Define an interface that derive from yba::plugin
//This is the actual "plugin" interface 
class my_plugin : public yba::plugin
{
public:
	[[nodiscard]] virtual const char* get_data() const = 0;
};
```

Here's what the host program needs to do load and use a plugin via the `yba::plugin_manager`

```cpp
//Somewhere you have created this object:
yba::plugin_manager my_plugin_manager;

//Want to load the plugin called "test" that implements a "my_plugin"? do this:
const auto* the_plugin = reinterpret_cast<my_plugin*>
	(my_plugin_manager.load_plugin("test"));

//You can talk to it by calling the interface
printf("%s\n", the_plugin->get_data());
```

Here's the *actual* implementation of the plugin (`test/main.cpp`)
```cpp
//Implement an yba::plugin interface in a dynamic library
#include <cstdio>

#include "plugin/plugin.hpp"
#include "test_interface/my_plugin.hpp"

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
	my_plugin_impl()
	{
		printf("my_plugin ctor (deterministic init)\n");
	}

	~my_plugin_impl()
	{
		printf("my_plugin dtor (deterministic un-init)\n");
	}

	[[nodiscard]] const char* get_data() const final
	{
		return "Is this thing even working?";
	}
};

//This function is needed, and it's signature changes depending of the operating system used, that's why it's a macro.
//It needs to return a `yba::plugin` pointer, that points to a newlly created object of the plugin's implementation
YPLUGS_BOOTSRAP_SIGNATURE()
{
	return reinterpret_cast<yba::plugin*>(new my_plugin_impl);
}
```


