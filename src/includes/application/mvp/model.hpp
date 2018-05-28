
#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace loki
{
    struct base_function_wrapper
    {
        virtual ~base_function_wrapper() = default;
    };

    template<class T>
    struct function_wrapper : base_function_wrapper
    {
        std::function<void(const T&)> function;

        explicit function_wrapper(const std::function<void(const T&)>& function)
            : function(function)
        {}

        virtual ~function_wrapper() = default;
    };

    class property_changed
    {
        std::unordered_multimap<std::string, std::shared_ptr<base_function_wrapper>> properties;

    public:
        template<class T>
        void notify_property_changed(const std::string& name, const T& value)
        {
            const auto range = properties.equal_range(name);
            for (auto it = range.first; it != range.second; ++it)
            {
                const auto base = it->second;
                auto derived = std::dynamic_pointer_cast<function_wrapper<T>>(base);
                derived->function(value);
            }
        }

        template<class T>
        void on_property_changed(const std::string& name, const std::function<void(const T&)>& slot)
        {
            auto wrapper = std::make_shared<function_wrapper<T>>(slot);
            properties.emplace(name, wrapper);
        }
    };

    class base_model : public property_changed
    {

    };

    class base_view : public property_changed
    {
    };

    template<class T>
    class property
    {
        std::string name;
        T value;
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
                this->notifier->template notify_property_changed<T>(name, value);
            }
        }

        const std::string& get_name() const noexcept
        {
            return name;
        }

        property(const std::string& name, const T& value, property_changed* notifier)
            : name(name), value(value), notifier(notifier)
        {
            this->notifier->template notify_property_changed<T>(name, value);
        }

        property<T>& operator=(const T& value) noexcept
        {
            this->value = value;
            this->notifier->template notify_property_changed<T>(name, value);
            return *this;
        }

        property<T>& operator=(T&& value) noexcept
        {
            this->value = std::move(value);
            this->notifier->template notify_property_changed<T>(name, value);
            return *this;
        }
    };

    class concrete_model : public base_model
    {
        property<int> count;
    public:
        concrete_model()
            : count{ "count", 0, this }
        {}
    };

    template<typename model_type, typename view_type>
    class concrete_presenter;

    class concrete_view : public base_view
    {
        concrete_presenter<concrete_model, concrete_view>* presenter = nullptr;
        property<int> count;
    public:
        concrete_view()
            : count{ "count", 0, this }
        {}

        void set_presenter(concrete_presenter<concrete_model, concrete_view>* presenter);

        void update();

        void draw() const;
    };

    struct presenter_messager
    {
        template<typename model_type, typename view_type>
        presenter_messager(model_type& model, view_type& view)
        {}
    };

    template<typename model_type, typename view_type>
    class concrete_presenter
    {
        model_type& model;
        view_type& view;
    public:
        concrete_presenter<model_type, view_type>(model_type& model, view_type& view)
            : model(model), view(view)
        {
            this->view.set_presenter(this);

            this->model.template on_property_changed<int>("count", [this](const int& count)
            {
                std::cout << "Model state updated, notifying view" << std::endl;
                this->view.count.set_value(count, false);
            });

            this->view.template on_property_changed<int>("count", [this](const int& count)
            {
                std::cout << "View state updated, notifying model" << std::endl;
                this->model.count.set_value(count, false);
            });
        }
    };

    inline void concrete_view::set_presenter(concrete_presenter<concrete_model, concrete_view>* presenter)
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

inline void test()
{
    loki::concrete_model model;

    loki::concrete_view view;

    loki::concrete_presenter<loki::concrete_model, loki::concrete_view> presenter
    {
        model,
        view
    };

    const auto running = true;

    while (running)
    {
        view.update();
        view.draw();
    }
}