#include <string>
#include <windows.h>
#include <sysinfoapi.h>
#include <application/system_info.hpp>
#include <future>
#include <atlstr.h>

namespace moka
{

    CString get_string_from_reg(const HKEY key_parent, const CString& key_name, const CString& key_val_name)
    {
        CRegKey key;
        CString out;
        if (key.Open(key_parent, key_name, KEY_READ) == ERROR_SUCCESS)
        {
            ULONG len = 256;
            key.QueryStringValue(key_val_name, out.GetBuffer(256), &len);
            out.ReleaseBuffer();
            key.Close();
        }
        return out;
    }

    /**
    * \brief Access system and hardware information.
    *        Use this class to figure out capabilities of the underlying platform and hardware.
    */
    class system_info::impl
    {
    public:
        static size_t number_of_processors()
        {
            SYSTEM_INFO info;
            GetSystemInfo(&info);
            return static_cast<size_t>(info.dwNumberOfProcessors);
        }

        static std::string cpuid()
        {
            int cpu_info[4] = { -1 };
            char cpu_brand_string[0x40];
            // Get the information associated with each extended ID.
            __cpuid(cpu_info, 0x80000000);
            const unsigned n_ex_ids = cpu_info[0];
            for (auto i = 0x80000000; i <= n_ex_ids; ++i)
            {
                __cpuid(cpu_info, i);
                // Interpret CPU brand string
                if (i == 0x80000002)
                    memcpy(cpu_brand_string, cpu_info, sizeof(cpu_info));
                else if (i == 0x80000003)
                    memcpy(cpu_brand_string + 16, cpu_info, sizeof(cpu_info));
                else if (i == 0x80000004)
                    memcpy(cpu_brand_string + 32, cpu_info, sizeof(cpu_info));
            }
            //string includes manufacturer, model and clockspeed
            return cpu_brand_string;
        }

        static size_t installed_memory_ram()
        {
            ULONGLONG memory{};
            GetPhysicallyInstalledSystemMemory(&memory);
            return static_cast<size_t>(memory);
        }

        static float physical_memory_in_use()
        {
            return {};
        }

        static cpu_architecture architecture()
        {
            return cpu_architecture::x64;
        }

        static std::string architecture_string()
        {
            return "x86-64";
        }

        static float processor_frequency()
        {
            return {};
        }

        static std::string operating_system_name()
        {
            return get_string_from_reg(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"ProductName").GetString();
        }

        static std::string operating_system_version()
        {
            return get_string_from_reg(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentVersion").GetString();
        }
    };

    std::ostream &operator << (std::ostream& stream, const system_info& obj)
    {
        stream << "System Information:" << std::endl;
        stream << "- OS:                     " << obj.operating_system_name() << " " << obj.operating_system_version() << std::endl;
        stream << "- CPU:                    " << obj.cpuid() << std::endl;
        stream << "- Architecture:           " << obj.architecture_string() << std::endl;
        stream << "- Cores:                  " << obj.number_of_processors() << std::endl;
        stream << "- Installed memory (RAM): " << obj.installed_memory() / 1000000 << " GB" << std::endl;
        return stream;
    }

	system_info::system_info()
        : impl_{ std::make_unique<impl>() }
    {}

    system_info::~system_info() = default;

    size_t system_info::number_of_processors() const
    {
        return impl_->number_of_processors();
    }

    size_t system_info::installed_memory() const
    {
        return impl_->installed_memory_ram();
    }

    float system_info::physical_memory_in_use() const
    {
        return impl_->physical_memory_in_use();
    }

    cpu_architecture system_info::architecture() const
    {
        return impl_->architecture();
    }

    std::string system_info::architecture_string() const
    {
        return impl_->architecture_string();
    }

    std::string system_info::operating_system_name() const
    {
        return impl_->operating_system_name();
    }

    float system_info::processor_frequency() const
    {
        return impl_->processor_frequency();
    }

    std::string system_info::cpuid() const
    {
        return impl_->cpuid();
    }

    std::string system_info::operating_system_version() const
    {
        return impl_->operating_system_version();
    }
}
