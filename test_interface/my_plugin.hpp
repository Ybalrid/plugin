#pragma once
#include "plugin/plugin.hpp"

//Define an interface that derive from yba::plugin
//This is the actual "plugin" interface 
class my_plugin : public yba::plugin
{
public:
	[[nodiscard]] virtual const char* get_data() const = 0;
};
