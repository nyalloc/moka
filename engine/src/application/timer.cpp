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
#include <application/timer.hpp>

namespace moka
{
    timer::~timer()
    {
        stop();
    }

    timer::timer(bool start)
        : started_(false),
          paused_(false),
          reference_(std::chrono::steady_clock::now()),
          accumulated_(std::chrono::duration<long double>(0))
    {
        if (start)
        {
            this->start();
        }
    }

    void timer::start()
    {
        if (!started_)
        {
            started_ = true;
            paused_ = false;
            accumulated_ = std::chrono::duration<long double>(0);
            reference_ = std::chrono::steady_clock::now();
        }
        else if (paused_)
        {
            reference_ = std::chrono::steady_clock::now();
            paused_ = false;
        }
    }

    void timer::stop()
    {
        if (started_ && !paused_)
        {
            const auto now = std::chrono::steady_clock::now();
            accumulated_ =
                accumulated_ +
                std::chrono::duration_cast<std::chrono::duration<long double>>(now - reference_);
            paused_ = true;
        }
    }

    void timer::reset()
    {
        if (started_)
        {
            started_ = false;
            paused_ = false;
            reference_ = std::chrono::steady_clock::now();
            accumulated_ = std::chrono::duration<long double>(0);
        }
    }
} // namespace moka
