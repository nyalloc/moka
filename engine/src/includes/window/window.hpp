#pragma once

#include <memory>
#include <string>
#include <application/signal.hpp>

namespace moka
{
    struct context_handle
    {
		uint16_t id;
    };

    class window
    {
    public:
        signal<> exit;
        window(const std::string& title);
        ~window();
        void swap_buffer() const;
        void set_size(int width, int height);
		context_handle make_context() const;
		void make_current(const context_handle handle);
    private:
        class impl;
        std::unique_ptr<impl> impl_;
    };
}
