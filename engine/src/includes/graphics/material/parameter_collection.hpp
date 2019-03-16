#pragma once

#include <graphics/material/material_parameter.hpp>
#include <unordered_map>

namespace moka
{
    class parameter_collection
    {
        std::vector<material_parameter> parameters_;
        std::unordered_map<std::string, size_t> index_lookup_;

    public:
        using iterator = std::vector<material_parameter>::iterator;
        using const_iterator = std::vector<material_parameter>::const_iterator;

        const_iterator begin() const;

        const_iterator end() const;

        iterator begin();

        iterator end();

        void generate_index_lookup();

        parameter_collection(std::initializer_list<material_parameter> parameters);

        parameter_collection(const parameter_collection& rhs);

        parameter_collection& operator=(const parameter_collection& rhs);

        parameter_collection(parameter_collection&& rhs) noexcept;

        parameter_collection& operator=(parameter_collection&& rhs) noexcept;

        ~parameter_collection();

        parameter_collection();

        const material_parameter& operator[](size_t index) const;

        material_parameter& operator[](size_t index);

        material_parameter& operator[](const std::string& name);

        size_t size() const;
    };
} // namespace moka
