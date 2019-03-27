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
#include <graphics/buffer/vertex_layout.hpp>

namespace moka
{
    vertex_layout::vertex_layout(std::vector<vertex_attribute>&& layout)
        : layout_(std::move(layout))
    {
        for (const auto& element : layout_)
        {
            total_size_ += element.size;
        }
    }

    vertex_layout::vertex_layout(const std::initializer_list<vertex_attribute> layout)
        : vertex_layout(std::vector<vertex_attribute>{layout})
    {
    }

    vertex_layout::iterator vertex_layout::begin() noexcept
    {
        return layout_.begin();
    }

    vertex_layout::iterator vertex_layout::end() noexcept
    {
        return layout_.end();
    }

    vertex_layout::const_iterator vertex_layout::begin() const noexcept
    {
        return layout_.begin();
    }

    vertex_layout::const_iterator vertex_layout::end() const noexcept
    {
        return layout_.end();
    }

    size_t vertex_layout::total_size() const noexcept
    {
        return total_size_;
    }
} // namespace moka
