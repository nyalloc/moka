#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/camera/camera_builder.hpp>
#include <graphics/transform.hpp>
#include <memory>

namespace moka
{
    // A camera. This wrapper class provides a simple wrapper interface to access any camera object.
    // An example of the bridge pattern, this abstraction can vary independently of its implementation.
    class camera
    {
        std::unique_ptr<base_camera> camera_;

    public:
        virtual ~camera();

        using builder = camera_builder;

        camera(const camera& camera) = delete;

        camera(camera&& camera) noexcept;

        camera& operator=(const camera& camera) = delete;

        camera& operator=(camera&& camera) = delete;

        camera(std::unique_ptr<base_camera>&& camera);

        glm::mat4 get_view() const;

        const glm::quat& get_rotation() const;

        const glm::vec3& get_position() const;

        const glm::mat4& get_projection() const;

        const transform& get_transform() const;

        void update(float delta_time) const;
    };
} // namespace moka
