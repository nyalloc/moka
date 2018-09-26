#pragma once

#include <graphics/graphics_device.hpp>
#include <unordered_map>
#include <vector>
#include <any>

namespace moka
{
	struct effect_parameter
	{
		uniform_handle handle;
		std::string name;
		std::any data;

		effect_parameter(
			uniform_handle handle
			, std::string&& name
			, std::any&& data)
			: handle(handle)
			, name(std::move(name))
			, data(std::move(data))
		{}
	};

	class effect_parameter_collection
	{
	private:
		std::vector<effect_parameter> parameters_;
	    std::unordered_map<std::string, size_t> index_lookup_;   

	public:
		effect_parameter_collection(std::vector<effect_parameter>&& parameters)
			: parameters_(std::move(parameters))
		{
			for (size_t i = 0; i < parameters_.size(); i++)
			{
				index_lookup_[parameters_[i].name] = i;
			}
		}

		const effect_parameter& operator[](const size_t index)
		{
			return parameters_[index];
		}

		const effect_parameter& operator[](const std::string& name)
		{
			return parameters_[index_lookup_[name]];
		}

		size_t size() const
		{
			return parameters_.size();
		}
	};

	class effect
	{
	protected:
		program_handle program_;
		effect_parameter_collection parameters_;

	public:

		effect(program_handle program, effect_parameter_collection&& parameters)
			: program_(program)
			, parameters_(std::move(parameters))
		{}

		const effect_parameter& operator[](const size_t index)
		{
			return parameters_[index];
		}

		const effect_parameter& operator[](const std::string& name)
		{
			return parameters_[name];
		}
	};
}