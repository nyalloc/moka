
#include <graphics/material/parameter_collection.hpp>

namespace moka
{
    parameter_collection::const_iterator parameter_collection::begin() const
    {
        return parameters_.begin();
    }

    parameter_collection::const_iterator parameter_collection::end() const
    {
        return parameters_.end();
    }

    parameter_collection::iterator parameter_collection::begin()
    {
        return parameters_.begin();
    }

    parameter_collection::iterator parameter_collection::end()
    {
        return parameters_.end();
    }

    void parameter_collection::generate_index_lookup()
    {
        for (size_t i = 0; i < parameters_.size(); i++)
        {
            index_lookup_[parameters_[i].name] = i;
        }
    }

    parameter_collection::parameter_collection(std::initializer_list<material_parameter> parameters)
        : parameters_(parameters)
    {
        generate_index_lookup();
    }

    parameter_collection::parameter_collection(const parameter_collection& rhs)
        : parameters_(rhs.parameters_)
    {
        generate_index_lookup();
    }

    parameter_collection& parameter_collection::operator=(const parameter_collection& rhs)
    {
        parameters_ = rhs.parameters_;
        generate_index_lookup();
        return *this;
    }

    parameter_collection::parameter_collection(parameter_collection&& rhs) noexcept
        : parameters_(std::move(rhs.parameters_))
    {
        generate_index_lookup();
    }

    parameter_collection& parameter_collection::operator=(parameter_collection&& rhs) noexcept
    {
        parameters_ = std::move(rhs.parameters_);
        generate_index_lookup();
        return *this;
    }

    parameter_collection::~parameter_collection()
    {
    }

    parameter_collection::parameter_collection()
    {
    }

    const material_parameter& parameter_collection::operator[](const size_t index) const
    {
        return parameters_[index];
    }

    material_parameter& parameter_collection::operator[](const size_t index)
    {
        return parameters_[index];
    }

    material_parameter& parameter_collection::operator[](const std::string& name)
    {
        const auto param = index_lookup_.find(name);

        if (param != index_lookup_.end())
        {
            const auto position = param->second;
            return parameters_[position];
        }

        index_lookup_[name] = parameters_.size();

        auto& e = parameters_.emplace_back();

        e.name = name;

        return e;
    }

    size_t parameter_collection::size() const
    {
        return parameters_.size();
    }
} // namespace moka
