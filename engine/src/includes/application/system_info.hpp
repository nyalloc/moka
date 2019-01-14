#pragma once

#include <string>
#include <memory>

namespace moka
{
    /**
     * \brief The processor architecture of the installed operating system. 
     */
    enum class cpu_architecture
    {
        x64,
        arm,
        arm64,
        ia_64,
        x86,
        unknown
    };

    /**
     * \brief Access system and hardware information.
     *        Use this class to figure out capabilities of the underlying platform and hardware. 
     */
    class system_info
    {
    public:
        system_info();
        ~system_info();

		system_info(const system_info& rhs) = delete;
		system_info(system_info&& rhs) = default;
		system_info& operator=(const system_info& rhs) = delete;
		system_info& operator=(system_info&& rhs) = default;

        size_t number_of_processors() const;

        size_t installed_memory() const;

        float physical_memory_in_use() const;

        cpu_architecture architecture() const;

        std::string architecture_string() const;

        std::string operating_system_name() const;

        float processor_frequency() const;

        std::string cpuid() const;

        std::string operating_system_version() const;

        friend std::ostream &operator << (std::ostream& stream, const system_info& obj);

    private:
        class impl;
        std::unique_ptr<impl> impl_;
    };
}
