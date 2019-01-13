#include <algorithm>
#include <graphics/command/command_list.hpp>

namespace moka
{
    bool command_list::key_comparator::operator()(const command_buffer& left, const command_buffer& right) const
    {
        return left.get_key() < right.get_key();
    }

    command_list::command_list() = default;

    command_list::~command_list() = default;

    command_list::command_list(command_list&& command_list) noexcept
        : command_packets_(std::move(command_list.command_packets_))
    {
    }

    command_list& command_list::operator=(command_list&& command_list) noexcept
    {
        command_packets_ = std::move(command_list.command_packets_);
        return *this;
    }

    void command_list::destroy()
    {
        command_packets_.clear();
        // command_packets.shrink_to_fit();
    }

    void command_list::accept(graphics_visitor& device)
    {
        for (auto& command_packet : command_packets_)
        {
            command_packet.accept(device);
        }
    }

    void command_list::sort()
    {
        std::sort(command_packets_.begin(), command_packets_.end(), key_comparator{});
        is_sorted_ = true;
    }

    bool command_list::is_sorted() const
    {
        return is_sorted_;
    }

    bool command_list::is_empty() const
    {
        return command_packets_.empty();
    }

    command_list::iterator command_list::begin()
    {
        return command_packets_.begin();
    }

    command_list::const_iterator command_list::begin() const
    {
        return command_packets_.begin();
    }

    command_list::iterator command_list::end()
    {
        return command_packets_.end();
    }

    command_list::const_iterator command_list::end() const
    {
        return command_packets_.end();
    }

    command_buffer& command_list::make_command_buffer(const sort_key key)
    {
        static int num_calls = 0;
        num_calls++;
        is_sorted_ = false;

        current_key_ = key;
        command_packets_.emplace_back(key);

        return command_packets_.back();
    }

    command_buffer& command_list::make_command_buffer()
    {
        return make_command_buffer(current_key_ + 1);
    }

    clear_command& command_list::clear()
    {
        return make_command_buffer().clear();
    }

    clear_command& command_list::clear(const sort_key key)
    {
        return make_command_buffer(key).clear();
    }

    draw_command& command_list::draw()
    {
        return make_command_buffer().draw();
    }

    draw_command& command_list::draw(const sort_key key)
    {
        return make_command_buffer(key).draw();
    }

    scissor_command& command_list::scissor()
    {
        return make_command_buffer().scissor();
    }

    scissor_command& command_list::scissor(sort_key key)
    {
        return make_command_buffer(key).scissor();
    }

    viewport_command& command_list::viewport()
    {
        return make_command_buffer().viewport();
    }

    viewport_command& command_list::viewport(sort_key key)
    {
        return make_command_buffer(key).viewport();
    }

    fill_index_buffer_command& command_list::fill_index_buffer()
    {
        return make_command_buffer().fill_index_buffer();
    }

    fill_index_buffer_command& command_list::fill_index_buffer(sort_key key)
    {
        return make_command_buffer(key).fill_index_buffer();
    }

    fill_vertex_buffer_command& command_list::fill_vertex_buffer()
    {
        return make_command_buffer().fill_vertex_buffer();
    }

    fill_vertex_buffer_command& command_list::fill_vertex_buffer(sort_key key)
    {
        return make_command_buffer(key).fill_vertex_buffer();
    }
} // namespace moka
