#pragma once

#include <window/window.hpp>
#include <atomic>
#include <logger/logger.hpp>
#include <graphics/graphics_device.hpp>
#include <asset_importer/filesystem.hpp>

namespace moka
{
    class application
    {
        logger log_;
        bool running_ = true;
    protected:
        window window_;
        graphics_device graphics_;
    public:
        application(int argc, char* argv[]);
        virtual ~application();
        virtual void draw(const game_time delta_time);
        virtual void update(const game_time delta_time);
        int run();
        static filesystem::path data_path();
    };
}