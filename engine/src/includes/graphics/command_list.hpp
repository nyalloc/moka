#pragma once


#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <algorithm>
#include <atomic>
#include "graphics_command.hpp"
#include "command_buffer.hpp"
#include <application/profile.hpp>

namespace moka
{
	class command_list
	{
		bool is_sorted_ = false;
		sort_key current_key = 0;
		std::vector<command_buffer> command_packets;
		using iterator = std::vector<command_buffer>::iterator;
		using const_iterator = std::vector<command_buffer>::const_iterator;

		struct key_comparator
		{
			bool operator()(const command_buffer& left, const command_buffer& right) const
			{
				return left.get_key() < right.get_key();
			}
		};

	public:
		command_list() = default;
		~command_list() = default;
		command_list(const command_list& command_list) = delete;

		command_list(command_list&& command_list)
			: command_packets(std::move(command_list.command_packets))
		{}

		command_list& operator = (command_list&& command_list)
		{
			command_packets = std::move(command_list.command_packets);
			return *this;
		}

		command_list& operator = (const command_list& command_list) = delete;

		void destroy()
		{
			command_packets.clear();
			//command_packets.shrink_to_fit();
		}

		void accept(graphics_visitor& device)
		{
			const auto duration = profile<milliseconds>([&]()
			{
				for (auto& command_packet : command_packets)
				{
					command_packet.accept(device);
				}
			});

			int foo = 0;
		}

		void sort()
		{
			std::sort(command_packets.begin(), command_packets.end(), key_comparator{});
			is_sorted_ = true;
		}

		bool is_sorted()
		{
			return is_sorted_;
		}

		bool is_empty()
		{
			return command_packets.empty();
		}

		iterator begin()
		{
			return command_packets.begin();
		}

		const_iterator begin() const
		{
			return command_packets.begin();
		}

		iterator end()
		{
			return command_packets.end();
		}

		const_iterator end() const
		{
			return command_packets.end();
		}

		command_buffer& make_command_buffer(sort_key key)
		{
			static int num_calls = 0;
			num_calls++;
			is_sorted_ = false;

			current_key = key;
			command_packets.emplace_back(key);

			return command_packets.back();
		}

		command_buffer& make_command_buffer()
		{
			return make_command_buffer(current_key + 1);
		}

		clear_command& clear()
		{
			return make_command_buffer().clear();
		}

		clear_command& clear(sort_key key)
		{
			return make_command_buffer(key).clear();
		}

		draw_command& draw()
		{
			return make_command_buffer().draw();
		}

		draw_command& draw(sort_key key)
		{
			return make_command_buffer(key).draw();
		}

		scissor_command& scissor()
		{
			return make_command_buffer().scissor();
		}

		scissor_command& scissor(sort_key key)
		{
			return make_command_buffer(key).scissor();
		}

		viewport_command& viewport()
		{
			return make_command_buffer().viewport();
		}

		viewport_command& viewport(sort_key key)
		{
			return make_command_buffer(key).viewport();
		}

		fill_index_buffer_command& fill_index_buffer()
		{
			return make_command_buffer().fill_index_buffer();
		}

		fill_index_buffer_command& fill_index_buffer(sort_key key)
		{
			return make_command_buffer(key).fill_index_buffer();
		}

		fill_vertex_buffer_command& fill_vertex_buffer()
		{
			return make_command_buffer().fill_vertex_buffer();
		}

		fill_vertex_buffer_command& fill_vertex_buffer(sort_key key)
		{
			return make_command_buffer(key).fill_vertex_buffer();
		}
	};
}