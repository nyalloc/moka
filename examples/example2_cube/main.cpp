
#include <asset_importer/resource_manager.hpp>
#include <application/application.hpp>
#include <asset_importer/texture_importer/texture_importer.hpp>
#include <optional>
#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <condition_variable>
#include <queue>
 

namespace messaging
{
    struct message_base
    {
        virtual ~message_base() = default;
    };

    template<typename T>
    struct wrapped_message
    {
        T content;

        template<typename... Args>
        explicit wrapped_message(Args&&... content)
            : content(std::forward<T>(content)...)
        {}
    };

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

        T wait_and_pop()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _data_condition.wait(lock, [this] { return !_data.empty(); });
            T front = std::move(_data.front());
            _data.pop();
            return front;
        }

        std::optional<T> try_pop()
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

        void push(const T& element)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _data.push(element);
            _data_condition.notify_one();
        }

        template<typename... Args>
        void emplace(Args&&... args)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _data.emplace(std::forward<Args>(args)...);
            _data_condition.notify_one();
        }

        bool empty() const
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _data.empty();
        }

        void clear() const
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _data.clear();
        }
    };

    using message_queue = queue<std::shared_ptr<message_base>>;

    class sender
    {
        message_queue* _q;
    public:
        sender() : _q(nullptr) {}
        explicit sender(message_queue* q) : _q(q) {}

        template<typename T>
        void send(T&& msg)
        {
            if(_q)
            {
                _q->push(std::make_shared<wrapped_message<T>>(std::forward<T>(msg)));
            }
        }
    };
}


int main()
{
    std::optional<int> test;

    const auto path = moka::application::data_path()
        / ".." / ".." / ".." / "examples" / "resources";

    moka::resource_manager manager{ path };

    {
        // texture is alive
        auto texture = manager.load_asset<moka::texture_2d>("tile.png");
        std::cout << manager.size<moka::texture_2d>() << std::endl;
        std::cout << "loading texture" << std::endl;
        std::cout << "texture size: " << texture.size() << std::endl;
    }

    // texture has gone out of scope, resource_manager should automatically deallocate any shared resources

    std::cout << manager.size<moka::texture_2d>() << std::endl;

    return 0;
}
