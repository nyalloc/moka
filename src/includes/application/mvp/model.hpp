#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>
#include <iostream>

template<typename T>
using subscriber = std::function<void(const T&)>;

struct base_subscriber_wrapper
{
    base_subscriber_wrapper() = default;
    base_subscriber_wrapper(const base_subscriber_wrapper&) noexcept = delete;
    base_subscriber_wrapper(const base_subscriber_wrapper&&) noexcept = delete;
    base_subscriber_wrapper& operator=(const base_subscriber_wrapper&) noexcept = delete;
    base_subscriber_wrapper& operator=(const base_subscriber_wrapper&&) noexcept = delete;

    virtual ~base_subscriber_wrapper() = default;
};

template<class T>
struct subscriber_wrapper : base_subscriber_wrapper
{
    subscriber<T> function;

    subscriber_wrapper(const subscriber<T>& function)
        : function(function)
    {}

    subscriber_wrapper() = default;
    subscriber_wrapper(const subscriber_wrapper&) noexcept = delete;
    subscriber_wrapper(const subscriber_wrapper&&) noexcept = delete;
    subscriber_wrapper& operator=(subscriber_wrapper&&) noexcept = delete;
    subscriber_wrapper& operator=(const subscriber_wrapper&) noexcept = delete;

    virtual ~subscriber_wrapper() = default;
};

using property_id = uint32_t;

template<class T>
class property;

class property_changed
{
    std::unordered_multimap<property_id, std::shared_ptr<base_subscriber_wrapper>> subscribers;
    property_id available_ids = 0;
public:
    virtual ~property_changed() = default;

    property_id get_new_id()
    {
        return ++available_ids;
    }

    template<class T>
    void notify_property_changed(const property<T>& name);

    template<class T>
    void on_property_changed(const property<T>& name, const subscriber<T>& slot);
};

class base_presenter
{
};

template<class T>
class property
{
    property_id id;
    T& value;
    property_changed* notifier;
public:
    const T& get_value() const noexcept
    {
        return value;
    }

    void set_value(const T& value, const bool notify = true)
    {
        this->value = value;
        if (notify)
        {
            this->notifier->notify_property_changed<T>(*this);
        }
    }

    property_id get_id() const noexcept
    {
        return id;
    }

    property(T& value, property_changed* notifier)
        : id(notifier->get_new_id()), value(value), notifier(notifier)
    {
        this->notifier->notify_property_changed<T>(*this);
    }

    property<T>& operator=(const T& value) noexcept
    {
        this->value = value;
        this->notifier->notify_property_changed<T>(*this);
        return *this;
    }

    property<T>& operator=(T&& value) noexcept
    {
        this->value = std::move(value);
        this->notifier->notify_property_changed<T>(*this);
        return *this;
    }
};

template<class T>
void property_changed::notify_property_changed(const property<T>& property)
{
    // get all subscribers who are subscribed to this property
    const auto range = subscribers.equal_range(property.get_id());

    // iterate over all subscribers and notify them of the property's new state
    for (auto it = range.first; it != range.second; ++it)
    {
        const auto base = it->second;
        auto derived = std::dynamic_pointer_cast<subscriber_wrapper<T>>(base);
        derived->function(property.get_value());
    }
}

template<class T>
void property_changed::on_property_changed(const property<T>& property, const subscriber<T>& slot)
{
    auto wrapper = std::make_shared<subscriber_wrapper<T>>(slot);
    subscribers.emplace(property.get_id(), wrapper);
}

class base_model : public property_changed
{
    int count_ = 0;
public:
    property<int> count;

    base_model()
        : count{ count_, this }
    {}
};

class base_view : public property_changed
{
    int count_ = 0;
public:
    property<int> count;

    base_view()
        : count{ count_, this }
    {}

    virtual void set_presenter(base_presenter* presenter) = 0;
};

class concrete_model : public base_model
{
};

class concrete_view : public base_view
{
    base_presenter* presenter = nullptr;
public:
    concrete_view()
        : base_view()
    {}

    void set_presenter(base_presenter* presenter) override;

    void update();

    void draw() const;
};

class concrete_presenter : public base_presenter
{
    base_model& model;
    base_view& view;
public:
    concrete_presenter(base_model& model, base_view& view)
        : model(model), view(view)
    {
        this->view.set_presenter(this);

        this->model.on_property_changed<int>(model.count, 
            [this](const auto& count)
        {
            std::cout << "Model state updated, notifying view" << std::endl;
            this->view.count.set_value(count, false);
        });

        this->view.on_property_changed<int>(view.count,
            [this](const auto& count)
        {
            std::cout << "View state updated, notifying model" << std::endl;
            this->model.count.set_value(count, false);
        });
    }
};

inline void concrete_view::set_presenter(base_presenter* presenter)
{
    this->presenter = presenter;
}

inline void concrete_view::update()
{
    auto value = count.get_value();
    count.set_value(++value);
}

inline void concrete_view::draw() const
{
    std::cout << count.get_value() << std::endl;
}

inline void test()
{
    concrete_model model;

    concrete_view view;

    concrete_presenter presenter
    {
        model,
        view
    };

    const auto running = true;

    while (running)
    {
        view.update();
        view.draw();

        if (model.count.get_value() == 100)
        {
            model.count.set_value(0);
        }
    }
}