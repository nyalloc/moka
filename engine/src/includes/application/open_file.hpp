#pragma once

#include <string>

enum class file_type
{
    gltf,
    hdr
};

bool open_file(std::string& path, file_type type);
