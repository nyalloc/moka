#pragma once

#include <graphics/buffer.hpp>
#include <cstdint>
#include <graphics/graphics_api.hpp>
#include <glm/glm.hpp>
#include <mutex>

namespace moka
{
	constexpr size_t get_uniform_size(const uniform_type type, const size_t count = 1) noexcept
	{
		switch (type)
		{
		case uniform_type::texture: return sizeof(texture_binding) * count;
		case uniform_type::vec3: return sizeof(glm::vec3) * count;
		case uniform_type::vec4: return sizeof(glm::vec4) * count;
		case uniform_type::mat3: return sizeof(glm::mat3) * count;
		case uniform_type::mat4: return sizeof(glm::mat4) * count;
		case uniform_type::float32: return sizeof(float) * count;
		default:;
		}
		return 0;
	};

	class uniform_buffer
	{
		static_buffer buffer_;
		mutable std::mutex mutex_;
	public:
		void set_uniform(const uniform_handle uniform, const uniform_type type, const void* data, const size_t count)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			// write the uniform handle as a header. 
			// this means the owner can read the first 16 bits and use the value to lookup the info needed to read the body
			buffer_.write_value(uniform.id);



			// read the raw memory uniform data
			buffer_.write_memory(data, get_uniform_size(type, count));
		}

		uniform_handle read_uniform_handle()
		{
			std::lock_guard<std::mutex> lock(mutex_);

			// read the uin32_t worth of header flags into a helper class that lets us unpack the data
			return uniform_handle{ buffer_.read_value<uint16_t>() };
		}

		const std::byte* read_uniform_body(const size_t size) 
		{
			std::lock_guard<std::mutex> lock(mutex_);

			// read the raw memory uniform data
			return buffer_.read_memory(size);
		}

		void set_position(const size_t position)
		{
			std::lock_guard<std::mutex> lock(mutex_);

			return buffer_.set_position(position);
		}

		size_t position() const
		{
			std::lock_guard<std::mutex> lock(mutex_);

			return buffer_.position();
		}

		size_t end() const
		{
			std::lock_guard<std::mutex> lock(mutex_);

			return buffer_.end();
		}

		void clear()
		{
			std::lock_guard<std::mutex> lock(mutex_);

			buffer_.clear();
		}
	};
}
