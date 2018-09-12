#pragma once

#include <cstddef>

namespace moka
{
	template<size_t Size>
	class basic_static_buffer
	{
		std::byte buffer_[Size];
		size_t end_ = 0;
		size_t position_ = 0;
	public:
		basic_static_buffer()
		{
			clear();
		}

		bool write_memory(const void* data, const size_t size)
		{
			if (end_ + size < Size)
			{
				memcpy(&buffer_[end_], data, size);
				end_ += size;
				return true;
			}
			return false;
		}

		template<typename T>
		bool write_value(const T& data)
		{
			return write_memory(&data, sizeof(T));
		}

		void set_position(const size_t position)
		{
			position_ = position;
		}

		const std::byte* read_memory(const size_t size)
		{
			const std::byte* result = &buffer_[position_];
			position_ += size;
			return result;
		}

		template<typename T>
		T read_value()
		{
			T result{};
			auto data = read_memory(sizeof(T));
			memcpy(&result, data, sizeof(T));
			return result;
		}

		size_t position() const
		{
			return position_;
		}

		size_t end() const
		{
			return end_;
		}

		void clear()
		{
			end_ = 0;
			position_ = 0;
			memset(&buffer_, 0, Size);
		}
	};

	using static_buffer = basic_static_buffer<1024>;
}
