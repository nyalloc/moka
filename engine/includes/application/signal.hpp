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

#include <functional>
#include <mutex>
#include <unordered_map>

namespace moka
{
    using slot_id = size_t;

    /**
     * \brief Basic signal class. Allows you to write observer patterns while avoiding boilerplate code.
     * \tparam Args Parameter pack. The arguments you want to pass to the slots when calling notify.
     */
    template <typename... Args>
    class signal
    {
        using slot = std::function<void(const Args&...)>;

    public:
        /**
         * \brief Create a new signal object.
         */
        signal();

        /**
         * \brief Connect a slot to this signal.
         * \param slot The slot this signal should connect to.
         * \return An id identifying the slot, so that objects that own the slot may disconnect from the signal upon destruction.
         */
        slot_id connect(slot&& slot) const;

        /**
         * \brief Disconnect a slot from the signal.
         * \param id The id identifying the slot, as returned from connect()
         */
        void disconnect(slot_id id) const;

        /**
         * \brief Destroy all slots connected to this signal.
         */
        void clear() const;

        /**
         * \brief Invoke the signal and notify all connected slots.
         * \param args The args that should be sent to all slots.
         */
        void operator()(const Args&... args) const;

        /**
         * \brief Invoke the signal and notify all connected slots.
         * \param args The args that should be sent to all slots.
         */
        void notify(const Args&... args) const;

    protected:
        mutable std::mutex mutex_;
        mutable std::unordered_map<slot_id, slot> slots_;
        mutable slot_id id_;
    };

    template <typename... Args>
    signal<Args...>::signal() : id_{0}
    {
    }

    template <typename... Args>
    slot_id signal<Args...>::connect(slot&& slot) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto val = slots_.emplace(++id_, std::move(slot));
        return id_;
    }

    template <typename... Args>
    void signal<Args...>::disconnect(slot_id id) const
    {
        slots_.erase(id);
    }

    template <typename... Args>
    void signal<Args...>::clear() const
    {
        slots_.clear();
    }

    template <typename... Args>
    void signal<Args...>::operator()(const Args&... args) const
    {
        notify(args...);
    }

    template <typename... Args>
    void signal<Args...>::notify(const Args&... args) const
    {
        for (const auto& it : slots_)
        {
            it.second(args...);
        }
    }
} // namespace moka
