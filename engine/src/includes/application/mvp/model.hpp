#pragma once

#include <application/application.hpp>
#include <memory>
#include <iostream>
#include <application/signal.hpp>

namespace moka
{
    template<class T>
    class property
    {
        T value;
        signal<T> changed_signal;
    public:
        ~property() = default;
        property<T>(const property<T>& rhs) = delete;
        property<T>(property<T>&& rhs) = delete;
        property<T>& operator = (const property<T>& rhs) = delete;
        property<T>& operator = (property<T>&& rhs) = delete;

        const T& get_value() const noexcept
        {
            return value;
        }

        void set_value(const T& value, const bool notify_this = true)
        {
            if (this->value != value)
            {
                this->value = value;
                if (notify_this)
                {
                    changed_signal.event_handler(value);
                }
            }
        }

        void set_value(T&& value, const bool notify_this = true)
        {
            if (this->value != value)
            {
                this->value = value;
                if (notify_this)
                {
                    changed_signal.event_handler(value);
                }
            }
        }

        property(T&& value)
            : value(value)
        {
            changed_signal.event_handler(std::move(value));
        }

        property<T>& operator=(const T& value) noexcept
        {
            if (this->value != value)
            {
                this->value = value;
                changed_signal.event_handler(value);
            }
            return *this;
        }

        property<T>& operator=(T&& value) noexcept
        {
            if (this->value != value)
            {
                this->value = std::move(value);
                changed_signal.event_handler(value);
            }
            return *this;
        }

        signal_id on_changed(std::function<void(T)>&& slot) const
        {
            return changed_signal.connect(std::move(slot));
        }
    };

    class base_model
    {
    public:
        property<int> count;

        base_model()
            : count{ 0 }
        {}
    };

    class base_presenter {};

    class base_view
    {
    public:
        virtual ~base_view() = default;

        property<int> count;

        base_view()
            : count{ 0 }
        {}

        virtual void set_presenter(base_presenter* presenter) = 0;
    };

    class concrete_model : public base_model
    {
        application& application;

    public:
        concrete_model(moka::application& application)
            : application(application)
        {}
    };

    class concrete_view : public base_view
    {
        base_presenter* presenter = nullptr;
        application& application;

    public:
        concrete_view(moka::application& application)
            : base_view(), application(application)
        {}

        void set_presenter(base_presenter* presenter) override;

        void update();

        void draw() const;
    };

    class concrete_presenter : public base_presenter
    {
        application& application;
        base_model& model;
        base_view& view;
    public:
        concrete_presenter(moka::application& application, base_model& model, base_view& view)
            : application(application), model(model), view(view)
        {
            this->view.set_presenter(this);

            this->model.count.on_changed([this](int count)
            {
                std::cout << "Model state updated, notifying view" << std::endl;
                this->view.count.set_value(count, false);
            });

            this->view.count.on_changed([this](int count)
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
}