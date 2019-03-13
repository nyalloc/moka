#pragma once

#include "graphics/device/graphics_device.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include <unordered_map>
#include <vector>

namespace moka
{
    // class texture_cache
    //{
    //    graphics_device& device_;

    //    std::vector<texture> textures_;
    //    std::unordered_map<std::string, int> path_to_textures_;

    // public:
    //    texture_cache(graphics_device& device) : device_(device)
    //    {
    //    }

    //    bool exists(const std::string& path)
    //    {
    //        return path_to_textures_.find(path) != path_to_textures_.end();
    //    }

    //    texture get_texture(const std::string& path)
    //    {
    //        return textures_[path_to_textures_.at(path)];
    //    }

    //    void add_texture(const texture t, const std::string& path)
    //    {
    //        const auto index = textures_.size();
    //        textures_.emplace_back(t);
    //        path_to_textures_[path] = index;
    //    }

    //    ~texture_cache()
    //    {
    //        clear();
    //    }

    //    void clear()
    //    {
    //        // use graphics_device to kill all textures

    //        textures_.clear();
    //        path_to_textures_.clear();
    //    }
    //};

    // class shader_cache
    //{
    //    graphics_device& device_;

    //    std::vector<shader> shaders_;
    //    std::unordered_map<std::string, int> path_to_shaders_;

    // public:
    //    shader_cache(graphics_device& device) : device_(device)
    //    {
    //    }

    //    bool exists(const std::string& path)
    //    {
    //        return path_to_shaders_.find(path) != path_to_shaders_.end();
    //    }

    //    shader get_shader(const std::string& path)
    //    {
    //        return shaders_[path_to_shaders_.at(path)];
    //    }

    //    void add_shader(const shader t, const std::string& path)
    //    {
    //        const auto index = shaders_.size();
    //        shaders_.emplace_back(t);
    //        path_to_shaders_[path] = index;
    //    }

    //    ~shader_cache()
    //    {
    //        clear();
    //    }

    //    void clear()
    //    {
    //        // use graphics_device to kill all shaders

    //        shaders_.clear();
    //        path_to_shaders_.clear();
    //    }
    //};
} // namespace moka
