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
         * \tparam duration_t Your preferred format of the elapsed time (defaults to milliseconds)
         * \return The elapsed time.
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
