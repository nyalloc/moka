#pragma once

#include <graphics/device/graphics_visitor.hpp>
#include <graphics/command/command_list.hpp>
#include <graphics/command/clear_command.hpp>
#include <graphics/command/draw_command.hpp>
#include <graphics/command/scissor_command.hpp>
#include <graphics/command/viewport_command.hpp>
#include <graphics/command/fill_index_buffer_command.hpp>
#include <graphics/command/fill_vertex_buffer_command.hpp>
#include <cstdint>
#include <type_traits>
#include <vector>
#include <application/profile.hpp>

namespace moka
{
	using sort_key = uint64_t;

	class command_buffer
	{
		std::vector<graphics_command_ptr> commands_;
		sort_key id_ = 0;

		template<typename T>
		T& emplace_back()
		{
			static_assert(std::is_base_of_v<graphics_command, T>, "Please only emplace commands that derive from the graphics_command interface!");

			commands_.emplace_back(std::make_unique<T>());

			auto& ptr = commands_.back();
			auto cast = static_cast<T*>(ptr.get());
			return *cast;
		}
	public:

		command_buffer(sort_key id)
			: id_(id)
		{}

		command_buffer() = default;
		~command_buffer() = default;
		command_buffer(const command_buffer& command_buffer) = delete;

		command_buffer(command_buffer&& command_buffer)
			: commands_(std::move(command_buffer.commands_))
			, id_(command_buffer.id_)
		{}

		command_buffer& operator = (command_buffer&& command_buffer)
		{
			commands_ = std::move(command_buffer.commands_);
			id_ = command_buffer.id_;
			return *this;
		}

		command_buffer& operator = (const command_buffer& command_buffer) = delete;

		const sort_key& get_key() const
		{
			return id_;
		}

		void accept(graphics_visitor& device)
		{
			const auto duration = profile<milliseconds>([&]()
			{
				for (auto& command : commands_)
				{
					command->accept(device);
				}
			});

			int foo = 0;
		}

		clear_command& clear()
		{
			return emplace_back<moka::clear_command>();
		}

		draw_command& draw()
		{
			return emplace_back<moka::draw_command>();
		}

		scissor_command& scissor()
		{
			return emplace_back<moka::scissor_command>();
		}

		viewport_command& viewport()
		{
			return emplace_back<moka::viewport_command>();
		}

		fill_index_buffer_command& fill_index_buffer()
		{
			return emplace_back<moka::fill_index_buffer_command>();
		}

		fill_vertex_buffer_command& fill_vertex_buffer()
		{
			return emplace_back<moka::fill_vertex_buffer_command>();
		}
	};
}