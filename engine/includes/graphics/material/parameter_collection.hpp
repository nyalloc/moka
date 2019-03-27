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

#include <graphics/material/material_parameter.hpp>
#include <unordered_map>

namespace moka
{
    /**
     * \brief A collection of material parameters.
     */
    class parameter_collection
    {
        std::vector<material_parameter> parameters_;
        std::unordered_map<std::string, size_t> index_lookup_;

    public:
        using iterator = std::vector<material_parameter>::iterator;
        using const_iterator = std::vector<material_parameter>::const_iterator;

        const_iterator begin() const;

        const_iterator end() const;

        iterator begin();

        iterator end();

        void generate_index_lookup();

        parameter_collection(std::initializer_list<material_parameter> parameters);

        parameter_collection(const parameter_collection& rhs);

        parameter_collection& operator=(const parameter_collection& rhs);

        parameter_collection(parameter_collection&& rhs) noexcept;

        parameter_collection& operator=(parameter_collection&& rhs) noexcept;

        ~parameter_collection();

        parameter_collection();

        const material_parameter& operator[](size_t index) const;

        material_parameter& operator[](size_t index);

        material_parameter& operator[](const std::string& name);

        size_t size() const;
    };
} // namespace moka
