#pragma once

#include <map>
#include <functional>
#include <mutex>

namespace loki
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
        mutable std::mutex mutex;
        mutable std::map<signal_id, std::function<void(Args...)>> slots;
        mutable signal_id id;
    };

    template <typename... Args>
    signal<Args...>::signal() : id{0}
    {}

    template <typename... Args>
    signal_id signal<Args...>::connect(std::function<void(Args...)>&& slot) const
    {
        std::lock_guard<std::mutex> lock(mutex);
        slots.emplace(++id, slot);
        return id;
    }

    template <typename... Args>
    void signal<Args...>::disconnect(signal_id id) const
    {
        slots.erase(id);
    }

    template <typename... Args>
    void signal<Args...>::clear() const
    {
        slots.clear();
    }

    template <typename... Args>
    void signal<Args...>::operator()(const Args&... args) const
    {
        notify(args...);
    }

    template <typename... Args>
    void signal<Args...>::notify(const Args&... args) const
    {
        for (auto it : slots)
        {
            it.second(args...);
        }
    }
}
