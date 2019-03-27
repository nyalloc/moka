/*
===========================================================================
Moka Source Code
Copyright 2019 Stuart Adams. All rights reserved.
https://github.com/stuartdadams/moka
stuartdadams | linkedin.com/in/stuartdadams

This file is part of the Moka Real-Time Physically-Based Rendering Project.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===========================================================================
*/
#pragma once

#include "graphics/device/graphics_device.hpp"
#include "graphics/shader.hpp"
#include "graphics/texture_handle.hpp"
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
