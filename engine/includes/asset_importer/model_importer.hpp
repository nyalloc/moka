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

#include <application/logger.hpp>
#include <asset_importer/asset_importer.hpp>
#include <filesystem>
#include <graphics/model.hpp>

namespace moka
{
    /**
     * \brief Asset importer for models. Currently only supports .gltf models.
     */
    template <>
    class asset_importer<model>
    {
        logger log_;
        graphics_device& device_;
        std::filesystem::path root_directory_;

    public:
        /**
         * \brief Construct a new model asset importer.
         * \param path The asset folder that all model paths are relative to.
         * \param device Graphics device to upload asset information to.
         */
        asset_importer(const std::filesystem::path& path, graphics_device& device);

        /**
         * \brief Import a new model.
         * \param model_path The model you would like to import.
         * \param material_path The material you want to use with this model.
         * \return The new model.
         */
        model load(const std::filesystem::path& model_path, const std::filesystem::path& material_path) const;
    };
} // namespace moka
