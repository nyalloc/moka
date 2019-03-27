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

#include <glm/glm.hpp>
#include <graphics/transform.hpp>

namespace moka
{
    /**
     * \brief a very basic camera - contains only the necessary data members to get things drawing on your screen.
     */
    class basic_camera
    {
    protected:
        transform transform_;  /**< The transform of this camera.*/
        glm::mat4 projection_; /**< The projection of this camera.*/

    public:
        virtual ~basic_camera() = default;

        basic_camera() = default;

        /**
         * \brief Create a new basic_camera object.
         * \param transform The transform of this camera.
         * \param projection The projection of this camera.
         */
        basic_camera(const transform& transform, const glm::mat4& projection);

        /**
         * \brief Update this camera.
         * \param delta_time The current delta time.
         */
        virtual void update(float delta_time);

        /**
         * \brief Get the view matrix.
         * \return The view matrix.
         */
        virtual glm::mat4 get_view() const;

        /**
         * \brief Get the rotation of the camera.
         * \return The rotation of the camera as a quaternion.
         */
        virtual const glm::quat& get_rotation() const;

        /**
         * \brief Set the rotation of the camera.
         * \param rotation A quaternion defining the new rotation of the camera.
         */
        virtual void set_rotation(const glm::quat& rotation);

        /**
         * \brief Get the position of the camera.
         * \return The position of the camera.
         */
        virtual const glm::vec3& get_position() const;

        /**
         * \brief Set the position of the camera.
         * \param position A vec3 defining the new position of the camera.
         */
        virtual void set_position(const glm::vec3& position);

        /**
         * \brief Get the projection of the camera.
         * \return The projection of the camera.
         */
        virtual const glm::mat4& get_projection() const;

        /**
         * \brief Set the projection of the camera.
         * \param projection A mat4 defining the new projection of the camera.
         */
        virtual void set_projection(const glm::mat4& projection);

        /**
         * \brief Set the transform of the camera.
         * \param transform The new transform of the camera.
         */
        virtual void set_transform(const transform& transform);

        /**
         * \brief Get the transform of the camera.
         * \return The camera's transform.
         */
        virtual const transform& get_transform() const;

        /**
         * \brief Set the perspective of the camera.
         * \param radians Specifies the field of view angle in the y direction. Expressed in radians.
         * \param aspect Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
         */
        virtual void set_perspective(float radians, float aspect);
    };
} // namespace moka
