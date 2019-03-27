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

#include <graphics/material/parameter_collection.hpp>

namespace moka
{
    parameter_collection::const_iterator parameter_collection::begin() const
    {
        return parameters_.begin();
    }

    parameter_collection::const_iterator parameter_collection::end() const
    {
        return parameters_.end();
    }

    parameter_collection::iterator parameter_collection::begin()
    {
        return parameters_.begin();
    }

    parameter_collection::iterator parameter_collection::end()
    {
        return parameters_.end();
    }

    void parameter_collection::generate_index_lookup()
    {
        for (size_t i = 0; i < parameters_.size(); i++)
        {
            index_lookup_[parameters_[i].name] = i;
        }
    }

    parameter_collection::parameter_collection(std::initializer_list<material_parameter> parameters)
        : parameters_(parameters)
    {
        generate_index_lookup();
    }

    parameter_collection::parameter_collection(const parameter_collection& rhs)
        : parameters_(rhs.parameters_)
    {
        generate_index_lookup();
    }

    parameter_collection& parameter_collection::operator=(const parameter_collection& rhs)
    {
        parameters_ = rhs.parameters_;
        generate_index_lookup();
        return *this;
    }

    parameter_collection::parameter_collection(parameter_collection&& rhs) noexcept
        : parameters_(std::move(rhs.parameters_))
    {
        generate_index_lookup();
    }

    parameter_collection& parameter_collection::operator=(parameter_collection&& rhs) noexcept
    {
        parameters_ = std::move(rhs.parameters_);
        generate_index_lookup();
        return *this;
    }

    parameter_collection::~parameter_collection()
    {
    }

    parameter_collection::parameter_collection()
    {
    }

    const material_parameter& parameter_collection::operator[](const size_t index) const
    {
        return parameters_[index];
    }

    material_parameter& parameter_collection::operator[](const size_t index)
    {
        return parameters_[index];
    }

    material_parameter& parameter_collection::operator[](const std::string& name)
    {
        const auto param = index_lookup_.find(name);

        if (param != index_lookup_.end())
        {
            const auto position = param->second;
            return parameters_[position];
        }

        index_lookup_[name] = parameters_.size();

        auto& e = parameters_.emplace_back();

        e.name = name;

        return e;
    }

    size_t parameter_collection::size() const
    {
        return parameters_.size();
    }
} // namespace moka
