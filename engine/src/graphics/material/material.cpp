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
#include <graphics/material/material.hpp>

namespace moka
{
    material::material() = default;
    material::~material() = default;
    material::material(const material& rhs) = default;
    material& material::operator=(const material& rhs) = default;

    material::material(material&& rhs) noexcept
        : alpha_mode_(rhs.alpha_mode_),
          programs_(std::move(rhs.programs_)),
          parameters_(std::move(rhs.parameters_)),
          blend_(rhs.blend_),
          culling_(rhs.culling_),
          polygon_mode_(rhs.polygon_mode_),
          depth_test_(rhs.depth_test_),
          scissor_test_(rhs.scissor_test_)
    {
    }

    material& material::operator=(material&& rhs) noexcept
    {
        alpha_mode_ = rhs.alpha_mode_;
        programs_ = std::move(rhs.programs_);
        parameters_ = std::move(rhs.parameters_);
        blend_ = rhs.blend_;
        culling_ = rhs.culling_;
        polygon_mode_ = rhs.polygon_mode_;
        depth_test_ = rhs.depth_test_;
        scissor_test_ = rhs.scissor_test_;
        return *this;
    }

    material::material(
        std::vector<program_handle>&& programs,
        parameter_collection&& parameters,
        alpha_mode alpha_mode,
        const blend& blend,
        const culling& culling,
        const polygon_mode& polygon_mode,
        bool depth_test,
        bool scissor_test)
        : alpha_mode_(alpha_mode),
          programs_(std::move(programs)),
          parameters_(std::move(parameters)),
          blend_(blend),
          culling_(culling),
          polygon_mode_(polygon_mode),
          depth_test_(depth_test),
          scissor_test_(scissor_test)
    {
    }

    material::material(program_handle program_handle)
        : programs_{program_handle}
    {
    }

    alpha_mode material::get_alpha_mode() const
    {
        return alpha_mode_;
    }

    program_handle material::get_program() const
    {
        return programs_[active_program_];
    }

    size_t material::size() const
    {
        return parameters_.size();
    }

    bool material::get_scissor_test() const
    {
        return scissor_test_;
    }

    bool material::get_depth_test() const
    {
        return depth_test_;
    }

    material::const_iterator material::begin() const
    {
        return parameters_.begin();
    }

    material::const_iterator material::end() const
    {
        return parameters_.end();
    }

    material::iterator material::begin()
    {
        return parameters_.begin();
    }

    material::iterator material::end()
    {
        return parameters_.end();
    }

    const material_parameter& material::operator[](const size_t index) const
    {
        return parameters_[index];
    }

    material_parameter& material::operator[](const size_t index)
    {
        return parameters_[index];
    }

    material_parameter& material::operator[](const std::string& name)
    {
        return parameters_[name];
    }

    const blend& material::get_blend() const
    {
        return blend_;
    }

    const culling& material::get_culling() const
    {
        return culling_;
    }

    const polygon_mode& material::get_polygon_mode() const
    {
        return polygon_mode_;
    }

    void material::set_active_program(size_t active_program)
    {
        active_program_ = active_program;
    }
} // namespace moka
