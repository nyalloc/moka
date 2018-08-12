#pragma once

namespace moka
{
    class base_importer
    {
    public:
        virtual ~base_importer() = default;
    };

    template<typename T>
    class asset_importer : public base_importer
    {
        // basic asset_importer definition.
        // provide specialisations for each resource type
    };
}
