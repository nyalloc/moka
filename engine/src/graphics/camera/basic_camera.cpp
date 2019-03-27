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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <graphics/camera/basic_camera.hpp>

namespace moka
{
    basic_camera::basic_camera(const transform& transform, const glm::mat4& projection)
        : transform_(transform), projection_(projection)
    {
    }

    void basic_camera::update(const float delta_time)
    {
    }

    glm::mat4 basic_camera::get_view() const
    {
        return glm::lookAt(
            transform_.get_position(),
            transform_.get_position() + transform_.front(),
            transform_.up());
    }

    const glm::quat& basic_camera::get_rotation() const
    {
        return transform_.get_rotation();
    }

    void basic_camera::set_rotation(const glm::quat& rotation)
    {
        return transform_.set_rotation(rotation);
    }

    const glm::vec3& basic_camera::get_position() const
    {
        return transform_.get_position();
    }

    void basic_camera::set_position(const glm::vec3& position)
    {
        return transform_.set_position(position);
    }

    const glm::mat4& basic_camera::get_projection() const
    {
        return projection_;
    }

    void basic_camera::set_projection(const glm::mat4& projection)
    {
        projection_ = projection;
    }

    void basic_camera::set_transform(const transform& transform)
    {
        transform_ = transform;
    }

    const transform& basic_camera::get_transform() const
    {
        return transform_;
    }

    void basic_camera::set_perspective(const float radians, const float aspect)
    {
        projection_ = glm::perspective(radians, aspect, 0.1f, 100.0f);
    }
} // namespace moka
