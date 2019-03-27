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

#include <chrono>

namespace moka
{
    /**
     * \brief A stopwatch style timer. Defines high-level time functions so that you don't have to go to a lot of effort just to find out how long something takes.
     */
    class timer final
    {
        bool started_;
        bool paused_;
        std::chrono::steady_clock::time_point reference_;
        std::chrono::duration<long double> accumulated_;

    public:
        timer(const timer& other) = default;

        timer(timer&& other) = default;

        timer& operator=(const timer& other) = default;

        timer& operator=(timer&& other) = default;

        ~timer();

        /**
         * \brief Create a new timer object.
         * \param start Should the timer start straight away after being constructed?
         */
        explicit timer(bool start);

        /**
         * \brief Start the timer.
         */
        void start();

        /**
         * \brief Stop the timer.
         */
        void stop();

        /**
         * \brief Reset the timer's state.
         */
        void reset();

        /**
         * \brief Get the currently elapsed time.
         * \tparam duration_t Your preferred format of the elapsed time
         * (defaults to milliseconds) \return The elapsed time.
         */
        template <class duration_t = std::chrono::milliseconds>
        auto elapsed() const;
    };

    template <class duration_t>
    auto timer::elapsed() const
    {
        if (started_)
        {
            if (paused_)
            {
                return std::chrono::duration_cast<duration_t>(accumulated_).count();
            }
            return std::chrono::duration_cast<duration_t>(
                       accumulated_ + (std::chrono::steady_clock::now() - reference_))
                .count();
        }
        return duration_t(0).count();
    }
} // namespace moka
