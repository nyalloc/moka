#pragma once

#include <functional>
#include <mutex>
#include <unordered_map>

namespace moka
{
    using signal_id = size_t;

    /**
    * \brief Basic signal class. Allows you to write observer patterns while avoiding boilerplate code.
    * \tparam Args Parameter pack. The arguments you want to pass to the slots when calling notify.
    */
    template <typename... Args>
    class signal
    {
    public:
        signal();

        signal_id connect(std::function<void(Args...)>&& slot) const;

        void disconnect(signal_id id) const;

        void clear() const;
        
        void operator()(const Args&... args) const;
        
        void notify(const Args&... args) const;
    protected:
        mutable std::mutex mutex_;
        mutable std::unordered_map<signal_id, std::function<void(Args...)>> slots_;
        mutable signal_id id_;
    };

    template <typename... Args>
    signal<Args...>::signal() : id_{0}
    {}

    template <typename... Args>
    signal_id signal<Args...>::connect(std::function<void(Args...)>&& slot) const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        slots_.emplace(++id_, std::move(slot));
        return id_;
    }

    template <typename... Args>
    void signal<Args...>::disconnect(signal_id id) const
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
        for (auto it : slots_)
        {
            it.second(args...);
        }
    }
}
