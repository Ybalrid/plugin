#pragma once
#include "plugin_bootstrap.hpp"

namespace yba
{
	class plugin
	{
	public:
		virtual ~plugin() { }
		virtual bool init() = 0;
		virtual bool quit() = 0;
	};
}
