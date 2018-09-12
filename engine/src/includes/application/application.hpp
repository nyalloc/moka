#pragma once

#include <application/window.hpp>
#include <application/logger.hpp>
#include <graphics/graphics_device.hpp>
#include <asset_importer/filesystem.hpp>

namespace moka
{
    class application
    {
        logger log_;
        bool running_ = true;
		void poll_events();
    protected:
        window window_;
        graphics_device graphics_;
    public:
        application(int argc, char* argv[]);
        virtual ~application();
        virtual void draw(const game_time delta_time) = 0;
        virtual void update(const game_time delta_time) = 0;
        int run();
        static filesystem::path data_path();
    };
}