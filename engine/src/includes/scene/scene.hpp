#pragma once

#include <graphics/api/graphics_api.hpp>
#include <graphics/buffer/index_buffer.hpp>
#include <graphics/buffer/vertex_buffer.hpp>
#include <graphics/material/material.hpp>
#include <graphics/transform.hpp>

namespace moka
{
    namespace experimental
    {
        struct primitive final
        {
            vertex_buffer vertex_buffer{};
            uint32_t vertex_count{};
            index_buffer index_buffer{};
            index_type index_type{};
            uint32_t index_count{};
            uint32_t index_buffer_offset{};
            primitive_type type{};
            int material{};
        };

        struct node final
        {
            transform local;
            int mesh;
            int children[64];
            size_t child_count;
        };

        struct scene final
        {
            // there should always be a root dummy node
            node nodes[1024];
            size_t node_count{};
        };

        struct mesh final
        {
            std::vector<primitive> primitives;
        };

        struct renderer final
        {
            std::vector<scene> scenes;
            std::vector<mesh> meshes;
            std::vector<material> materials;
            std::vector<texture> textures;
            std::vector<vertex_layout> layouts;

            int active_scene = 0;

            void draw_node(scene& scene, node& node, const glm::mat4& parent_matrix)
            {
                auto& mesh = meshes[node.mesh];

                auto& transform = node.local;
                const auto matrix = transform.to_matrix() * parent_matrix;

                // draw all the primitives in this mesh
                for (auto& primitive : mesh.primitives)
                {
                }

                // draw all the primitives in this mesh's children
                for (size_t i = 0; i < node.child_count; i++)
                {
                    draw_node(scene, scene.nodes[i], matrix);
                }
            }

            void draw()
            {
                auto& scene = scenes[active_scene];

                draw_node(scene, scene.nodes[0], glm::mat4{});
            }
        };
    } // namespace experimental
} // namespace moka
