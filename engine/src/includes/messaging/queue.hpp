#pragma once

#include <mutex>
#include <queue>
#include <messaging/message.hpp>
#include <optional>

namespace moka
{
    template<typename T>
    class queue
    {
        mutable std::mutex _mutex;
        std::queue<T> _data;
        std::condition_variable _data_condition;
    public:
        queue() noexcept = default;
        queue(const queue& rhs) noexcept = default;
        queue(queue&& rhs) noexcept = default;
        queue& operator = (queue&& rhs) noexcept = default;
        queue& operator = (const queue& rhs) noexcept = default;

        T wait_and_pop();

        std::optional<T> try_pop();

        void push(const T& element);

        template<typename... Args>
        void emplace(Args&&... args);

        bool empty() const;

        void clear() const;
    };

    template <typename T>
    T queue<T>::wait_and_pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _data_condition.wait(lock, [this] { return !_data.empty(); });
        T front = std::move(_data.front());
        _data.pop();
        return front;
    }

    template <typename T>
    std::optional<T> queue<T>::try_pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_data.empty())
        {
            return std::nullopt;
        }
        T front = std::move(_data.front());
        _data.pop();
        return front;
    }

    template <typename T>
    void queue<T>::push(const T& element)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _data.push(element);
        _data_condition.notify_one();
    }

    template <typename T>
    template <typename ... Args>
    void queue<T>::emplace(Args&&... args)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _data.emplace(std::forward<Args>(args)...);
        _data_condition.notify_one();
    }

    template <typename T>
    bool queue<T>::empty() const
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _data.empty();
    }

    template <typename T>
    void queue<T>::clear() const
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _data.clear();
    }

    using message_queue = queue<std::shared_ptr<message_base>>;
}
