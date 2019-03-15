#pragma once

#include "graphics/buffer/index_buffer.hpp"
#include "graphics/buffer/vertex_buffer.hpp"
#include "graphics/transform.hpp"
#include <vector>

namespace moka
{
    enum rendering_mode
    {
        points,
        lines,
        triangles
    };

    struct primitive final
    {
        rendering_mode mode;
        uint32_t mesh = 0;
        index_buffer indices;
        vertex_buffer vertices;
        int32_t material = -1;
        glm::vec3 bounds_min = {0.0f, 0.0f, 0.0f};
        glm::vec3 bounds_max = {0.0f, 0.0f, 0.0f};
    };

    struct node final
    {
        int parent = -1;
    };

    struct material final
    {
        std::string name;

        struct
        {
            int32_t texture = -1;
            glm::vec4 factor = {1.0f, 1.0f, 1.0f, 1.0f};
        } base_color;

        struct
        {
            int32_t texture = -1;
            float metallic_factor = 0.5f;
            float roughness_factor = 0.5f;
        } metallic_roughness;

        struct
        {
            int32_t texture = -1;
            float factor = 1.0f; // normal-scale
        } normal;

        struct
        {
            int32_t texture = -1;
            float factor = 1.0f; // occlusion-strength
        } occlusion;

        struct
        {
            int32_t texture = -1;
            glm::vec3 factor = {1.0f, 1.0f, 1.0f};
        } emmisive;
    };

    struct texture final
    {
        std::string uri;
        int32_t info;
        int32_t tex;
    };

    struct mesh final
    {
        std::vector<primitive> primitives;
        int node;
    };

    struct scene
    {
        std::vector<int> nodes;
    };

    struct renderer
    {
        std::vector<node> nodes;
        std::vector<mesh> mesh;
        std::vector<texture> textures;
        std::vector<material> materials;
    };
} // namespace moka
