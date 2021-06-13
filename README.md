# yba::plugin ![CI](https://github.com/Ybalrid/plugin/workflows/CI/badge.svg)

> This is a WIP project

A simple and safe C++ plugin system implemented as a header-only library

## How to use

Add the "include" folder to the include path of a project that want to use the plugin interface and plugin manager.

## Principle

A program (that we will call the "Host program") that want to load code from a dynamic library (called "Plugin library") 
without any build time dependency beside a common interface definition. 

This small library permit you to do so, really simply by doing the following:

1. The Host program need to instantiate a `yba::plugin_manager` object
2. Both the *Host program* and the *Plugin library* need to share an interface class that derive from `yba::plugin`
3. Plugin library contains a class that  implements the interface mentioned above, define one *exported function* (using the `YPLUGS_BOOTSRAP_SIGNATURE` macro) that create an object of that specific type on the heap and return a pointer casted as a `yba::plugin*`

Calling `yba::plugin_manager::load_plugin("my_plugin_name)` will load the dynamic library using the Operating System API. 
It then can retreive the boostrap function and obtain the `yba::plugin` object from it, and will automatically manages it's lifetime. 
It returns to you a `yba::plugin` object pointer that you can trust is an instance of the plugin interface, and recast down to the interface type.

## Example

Please refer to the content of the 3 "test" directories in the repository: 

- `test_interface` define the plugin interface. It's files are common to the Host program and Plugin library (as defined above)
- `test` contains the plugin implementation 
- `test_host` is a simlple C++ program that wants to load external code in a plugin 

An `yba::plugin` instance's lifetime is defined by the 4 following events (that correspond to the 4 different member functions that constitute the `yba::plugin` interface, and the class ctor/dtor):

1. Plugin object is created (bootsrap function call the class contrstructor)
2. Plugin has been loaded by plugin manager (`bool init()`)
3. Plugin has been unloaded by plugin manager (`bool quit()`)
4. Plugin object is destroyed (class constructor is called because the plugin object owner in `plugin_manager` goes out of scope)

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
const auto* the_plugin = dynamic_cast<my_plugin*>
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

