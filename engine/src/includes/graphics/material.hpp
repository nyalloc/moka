#pragma once

#include <graphics/graphics_device.hpp>
#include <graphics/texture_2d.hpp>
#include <unordered_map>
#include <vector>
#include <variant>
#include <any>

namespace moka
{
	using parameter = std::variant<float, glm::vec3, glm::vec4, glm::mat3, glm::mat4, texture_2d>;

	struct effect_parameter
	{
		parameter_type type;
		std::string name;
		parameter data;
		size_t count = 1;

		effect_parameter() = default;

		effect_parameter(
			const std::string& name
			, parameter_type type
			, const parameter& data
			, size_t count = 1)
			: name(std::move(name))
			, type(type)
			, data(data)
			, count(count)
		{}

		effect_parameter(
			const std::string& name
			, const float data
			, size_t count = 1)
			: effect_parameter(name, parameter_type::float32, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const glm::vec3& data
			, size_t count = 1)
			: effect_parameter(name, parameter_type::vec3, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const glm::vec4& data
			, size_t count = 1)
			: effect_parameter(name, parameter_type::vec4, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const glm::mat3& data
			, size_t count = 1)
			: effect_parameter(name, parameter_type::mat3, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const glm::mat4& data
			, size_t count = 1)
			: effect_parameter(name, parameter_type::mat4, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const texture_2d& data
			, size_t count = 1)
			: effect_parameter(name, parameter_type::texture, data, count)
		{}

		effect_parameter& operator = (const float data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::float32;
			return *this;
		}

		effect_parameter& operator = (const glm::vec3& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::vec3;
			return *this;
		}

		effect_parameter& operator = (const glm::vec4& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::vec4;
			return *this;
		}

		effect_parameter& operator = (const glm::mat3& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::mat3;
			return *this;
		}

		effect_parameter& operator = (const glm::mat4& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::mat4;
			return *this;
		}

		effect_parameter& operator = (const texture_2d& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::texture;
			return *this;
		}
	};

	class parameter_collection
	{
	private:
		std::vector<effect_parameter> parameters_;
	    std::unordered_map<std::string, size_t> index_lookup_;   
	public:

		void generate_index_lookup()
		{
			for (size_t i = 0; i < parameters_.size(); i++)
			{
				index_lookup_[parameters_[i].name] = i;
			}
		}

		parameter_collection(std::initializer_list<effect_parameter> parameters)
			: parameters_(parameters)
		{
			generate_index_lookup();
		}

		parameter_collection() = default;

		const effect_parameter& operator[](const size_t index) const
		{
			return parameters_[index];
		}

		effect_parameter& operator[](const size_t index)
		{
			return parameters_[index];
		}

		effect_parameter& operator[](const std::string& name)
		{
			auto param = index_lookup_.find(name);

			if (param != index_lookup_.end())
			{
				auto position = param->second;
				return parameters_[position];
			}

			effect_parameter e;
			e.name = name;
			parameters_.emplace_back(e);

			auto size = parameters_.size() - 1;
			index_lookup_.emplace(name, size);

			return parameters_[size];
		}

		size_t size() const
		{
			return parameters_.size();
		}
	};

	class material
	{
	protected:
		program_handle program_ = { std::numeric_limits<uint16_t>::max() };
		parameter_collection parameters_;

	public:

		material() = default;

		material(program_handle program, parameter_collection&& parameters)
			: program_(program)
			, parameters_(std::move(parameters))
		{}

		program_handle get_program() const
		{
			return program_;
		}

		size_t size() const
		{
			return parameters_.size();
		}

		const effect_parameter& operator[](const size_t index) const
		{
			return parameters_[index];
		}

		effect_parameter& operator[](const size_t index)
		{
			return parameters_[index];
		}

		effect_parameter& operator[](const std::string& name)
		{
			return parameters_[name];
		}
	};
}