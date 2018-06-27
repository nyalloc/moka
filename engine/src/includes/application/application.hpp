#pragma once

#include <window/window_.hpp>
#include <safe_queue.hpp>
#include <atomic>
#include <logger/logger.hpp>
#include <graphics_api.hpp>
#include <entry.hpp>

namespace neon
{
    class application
    {
        event_dispatcher dispatcher_;
        logger log_;
        bool running_ = true;
    protected:
        window window_;
        graphics_api graphics_;
    public:
        template<typename T, typename event_type, typename = std::enable_if<std::is_base_of_v<event, event_type>>>
        void post_event(event_type&& event, event_subscriber& recipient);
        application(int argc, char* argv[]);
        virtual ~application();
        virtual void draw(const game_time delta_time);
        virtual void update(const game_time delta_time);
        int run();
    };

    template <typename T, typename event_type, typename>
    void application::post_event(event_type&& event, event_subscriber& recipient)
    {
        dispatcher_.post_event(std::move(event), recipient);
    }

}