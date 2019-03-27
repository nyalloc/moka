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
