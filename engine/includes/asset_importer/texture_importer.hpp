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

#include <filesystem>
#include <graphics/texture_handle.hpp>

namespace moka
{
    /**
     * \brief Load a texture from file into host memory.
     * \param path The path to the texture you want to import.
     * \param width This value will be re-assigned in the function to the width of the image.
     * \param height This value will be re-assigned in the function to the height of the image.
     * \param format This value will be re-assigned in the function to the format of the image.
     * \param requested_format The format you wish the image to be imported with.
     * \return The host image data - a heap-allocated array of bytes! Free this data after use using free_texture()
     */
    std::byte* texture_load(
        const std::filesystem::path& path,
        int& width,
        int& height,
        host_format& format,
        host_format requested_format = host_format::auto_detect);

    /**
     * \brief Load an HDR texture from file into host memory.
     * \param path The path to the HDR texture you want to import.
     * \param width This value will be re-assigned in the function to the width of the image.
     * \param height This value will be re-assigned in the function to the height of the image.
     * \param format This value will be re-assigned in the function to the format of the image.
     * \param requested_format The format you wish the image to be imported with.
     * \return The host image data - a heap-allocated array of floats! Free this data after use using free_texture()
     */
    float* texture_load_hdr(
        const std::filesystem::path& path,
        int& width,
        int& height,
        host_format& format,
        host_format requested_format = host_format::auto_detect);

    /**
     * \brief Free a heap-allocated buffer of host texture data.
     * \param data The heap-allocated buffer of host texture data you wish to free.
     */
    void free_texture(void* data);
} // namespace moka
