#pragma once

#include <cassert>
#include <string>
#include <memory>
#include <stdexcept>
#include <vector>
#include <unordered_map>

#include "operating_system.hpp"
#include "plugin.hpp"

namespace yba
{
	class dynamic_library
	{
	public:
		//deactivate copy
		dynamic_library& operator=(const dynamic_library&) = delete;
		dynamic_library(const dynamic_library&) = delete;

		//custom move
		dynamic_library& operator=(dynamic_library&& other) noexcept
		{
			handle = other.handle;
			other.handle = nullptr;
			return *this;
		}

		dynamic_library(dynamic_library&& other) noexcept
		{
			*this = std::move(other);
		}

		dynamic_library() = default;
		dynamic_library(const std::string& name)
		{
			std::string library_name = name;
			utils::operating_system::sanitize(library_name);
			handle = utils::operating_system::load_dynamic_library(library_name);
			if (!handle)
				throw std::runtime_error("Did not load");
		}

		~dynamic_library()
		{
			if (handle != nullptr)
				utils::operating_system::unload_dynamic_library(handle);
			handle = nullptr;
		}

		plugin* create_plugin_instance()
		{
			auto fptr = get_boostrap_function();
			if (!fptr)
				throw std::runtime_error("Did not find yplugs bootstrap function in plugin library");
			return fptr();
		}

	private:
		using boostrap_function = plugin * (*)();
		boostrap_function get_boostrap_function()
		{
			return reinterpret_cast<boostrap_function>(utils::operating_system::get_proc_address(handle, ___YPLUGS_BOOTSTRAP_PROC_NAME_STR));
		}

		void* handle = nullptr;
	};

	class plugin_manager
	{
	public:
		plugin_manager() = default;
		plugin* load_plugin(const std::string& library_name)
		{
			auto library_iterator = libraries.find(library_name);
			if (library_iterator == libraries.end())
			{
				libraries[library_name] = dynamic_library(library_name);
				library_iterator = libraries.find(library_name);
				assert(library_iterator != libraries.end());
			}
			dynamic_library& library = library_iterator->second;

			plugin* plugin_instance_ptr = library.create_plugin_instance();
			plugins.emplace_back(plugin_instance_ptr);

			assert((uintptr_t)plugin_instance_ptr == (uintptr_t)plugins.back().get());
			plugin_instance_ptr->init();

			return plugin_instance_ptr;
		}

		~plugin_manager()
		{
			for (auto& plugin : plugins)
				plugin->quit();
		}

	private:
		std::unordered_map<std::string, dynamic_library> libraries;
		std::vector<std::unique_ptr<plugin>> plugins;
	};
}
