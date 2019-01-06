#pragma once

#include <graphics/texture.hpp>
#include <graphics/api/graphics_api.hpp>
#include <unordered_map>
#include <vector>
#include <variant>
#include <glm/glm.hpp>

namespace moka
{
	using parameter = std::variant<float, glm::vec3, glm::vec4, glm::mat3, glm::mat4, moka::texture>;

	struct effect_parameter
	{
		parameter_type type = parameter_type::null;
		std::string name;
		parameter data;
		size_t count = 1;

		effect_parameter() = default;

		~effect_parameter() = default;

		effect_parameter(
			const std::string& name
			, const parameter_type type
			, const parameter& data
			, const size_t count = 1)
			: type(type)
			, name(name)
			, data(data)
			, count(count)
		{}

		effect_parameter(
			const std::string& name
			, const float data
			, const size_t count = 1)
			: effect_parameter(name, parameter_type::float32, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const glm::vec3& data
			, const size_t count = 1)
			: effect_parameter(name, parameter_type::vec3, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const glm::vec4& data
			, const size_t count = 1)
			: effect_parameter(name, parameter_type::vec4, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const glm::mat3& data
			, const size_t count = 1)
			: effect_parameter(name, parameter_type::mat3, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const glm::mat4& data
			, const size_t count = 1)
			: effect_parameter(name, parameter_type::mat4, data, count)
		{}

		effect_parameter(
			const std::string& name
			, const texture& data
			, const size_t count = 1)
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

		effect_parameter& operator = (const texture& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::texture;
			return *this;
		}

		effect_parameter& operator = (glm::vec3&& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::vec3;
			return *this;
		}

		effect_parameter& operator = (glm::vec4&& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::vec4;
			return *this;
		}

		effect_parameter& operator = (glm::mat3&& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::mat3;
			return *this;
		}

		effect_parameter& operator = (glm::mat4&& data)
		{
			this->data = data;
			this->count = 1;
			type = parameter_type::mat4;
			return *this;
		}

		effect_parameter& operator = (texture&& data)
		{
			this->data = std::move(data);
			this->count = 1;
			type = parameter_type::texture;
			return *this;
		}

		effect_parameter& operator = (const effect_parameter& rhs) = default;

		effect_parameter& operator = (effect_parameter&& rhs) noexcept = default;

		effect_parameter(const effect_parameter& rhs) = default;

		effect_parameter(effect_parameter&& rhs) noexcept = default;
	};

	class parameter_collection
	{
		std::vector<effect_parameter> parameters_;
	    std::unordered_map<std::string, size_t> index_lookup_;

	public:
		using iterator = std::vector<effect_parameter>::iterator;
		using const_iterator = std::vector<effect_parameter>::const_iterator;

		const_iterator begin() const
		{
			return parameters_.begin();
		}

		const_iterator end() const 
		{
			return parameters_.end();
		}

		iterator begin()
		{
			return parameters_.begin();
		}

		iterator end()
		{
			return parameters_.end();
		}

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

		parameter_collection(const parameter_collection& rhs)
			: parameters_(rhs.parameters_)
		{
			generate_index_lookup();
		}

		parameter_collection& operator = (const parameter_collection& rhs)
		{
			parameters_ = rhs.parameters_;
			generate_index_lookup();
			return *this;
		}

		parameter_collection(parameter_collection&& rhs) noexcept
			: parameters_(std::move(rhs.parameters_))
		{
			generate_index_lookup();
		}

		parameter_collection& operator = (parameter_collection&& rhs) noexcept
		{
			parameters_ = std::move(rhs.parameters_);
			generate_index_lookup();
			return *this;
		}

		~parameter_collection() = default;

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
			const auto param = index_lookup_.find(name);

			if (param != index_lookup_.end())
			{
				const auto position = param->second;
				return parameters_[position];
			}

			index_lookup_[name] = parameters_.size();

			auto& e = parameters_.emplace_back();

			e.name = name;

			return e;
		}

		size_t size() const
		{
			return parameters_.size();
		}
	};

	enum class polygon_draw_mode
	{
		fill,
		line,
		points
	};

	struct polygon_mode
	{
		polygon_draw_mode mode = polygon_draw_mode::fill;
		face faces = face::front_and_back;
	};

	struct culling
	{
		bool enabled = true;
		face faces = face::back;
	};

	struct blend
	{
		bool enabled = false;
		blend_equation equation = blend_equation::func_add;
		blend_function_factor source = blend_function_factor::src_alpha;
		blend_function_factor destination = blend_function_factor::one_minus_src_alpha;
	};

	class material_builder;

	class material
	{
	protected:
		alpha_mode alpha_mode_ = alpha_mode::opaque;
		program program_ = { std::numeric_limits<uint16_t>::max() };
		parameter_collection parameters_;
		blend blend_;
		culling culling_;
		polygon_mode polygon_mode_;
		bool depth_test_ = true;
		bool scissor_test_ = false;
	public:

		using iterator = parameter_collection::iterator;
		using const_iterator = parameter_collection::const_iterator;

		using builder = material_builder;

		material() = default;

		~material() = default;

		material(const material& rhs) = default;

		material& operator = (const material& rhs) = default;

		material(material&& rhs) noexcept
			: alpha_mode_(rhs.alpha_mode_)
			, program_(rhs.program_)
			, parameters_(std::move(rhs.parameters_))
			, blend_(rhs.blend_)
			, culling_(rhs.culling_)
			, polygon_mode_(rhs.polygon_mode_)
			, depth_test_(rhs.depth_test_)
			, scissor_test_(rhs.scissor_test_)
		{}

		material& operator = (material&& rhs) noexcept
		{
			alpha_mode_ = rhs.alpha_mode_;
			program_ = rhs.program_;
			parameters_ = std::move(rhs.parameters_);
			blend_ = rhs.blend_;
			culling_ = rhs.culling_;
			polygon_mode_ = rhs.polygon_mode_;
			depth_test_ = rhs.depth_test_;
			scissor_test_ = rhs.scissor_test_;
			return *this;
		}

		material(program program, parameter_collection&& parameters, alpha_mode alpha_mode, const blend& blend, const culling& culling, const polygon_mode& polygon_mode, bool depth_test, bool scissor_test)
			: alpha_mode_(alpha_mode)
			, program_(program)
			, parameters_(std::move(parameters))
			, blend_(blend)
			, culling_(culling)
			, polygon_mode_(polygon_mode)
			, depth_test_(depth_test)
			, scissor_test_(scissor_test)
		{}

		explicit material(program program)
			: program_(program)
		{}

		alpha_mode get_alpha_mode() const
		{
			return alpha_mode_;
		}

		program get_program() const
		{
			return program_;
		}

		size_t size() const
		{
			return parameters_.size();
		}

		bool get_scissor_test() const
		{
			return scissor_test_;
		}

		bool get_depth_test() const
		{
			return depth_test_;
		}

		const_iterator begin() const
		{
			return parameters_.begin();
		}

		const_iterator end() const
		{
			return parameters_.end();
		}

		iterator begin()
		{
			return parameters_.begin();
		}

		iterator end()
		{
			return parameters_.end();
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

		const blend& get_blend() const
		{
			return blend_;
		}

		const culling& get_culling() const
		{
			return culling_;
		}

		const polygon_mode& get_polygon_mode() const
		{
			return polygon_mode_;
		}
	};
}