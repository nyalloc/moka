#include <graphics/material/material.hpp>

namespace moka
{
    material::material() = default;
    material::~material() = default;
    material::material(const material& rhs) = default;
    material& material::operator=(const material& rhs) = default;

    material::material(material&& rhs) noexcept
        : alpha_mode_(rhs.alpha_mode_),
          program_(rhs.program_),
          parameters_(std::move(rhs.parameters_)),
          blend_(rhs.blend_),
          culling_(rhs.culling_),
          polygon_mode_(rhs.polygon_mode_),
          depth_test_(rhs.depth_test_),
          scissor_test_(rhs.scissor_test_)
    {
    }

    material& material::operator=(material&& rhs) noexcept
    {
        alpha_mode_ = rhs.alpha_mode_;
        program_ = rhs.program_;
        parameters_ = std::move(rhs.parameters_);
        blend_ = rhs.blend_;
        culling_ = rhs.culling_;
        polygon_mode_ = rhs.polygon_mode_;
        depth_test_ = rhs.depth_test_;
        scissor_test_ = rhs.scissor_test_;
        return *this;
    }

    material::material(
        program_handle program_handle,
        parameter_collection&& parameters,
        alpha_mode alpha_mode,
        const blend& blend,
        const culling& culling,
        const polygon_mode& polygon_mode,
        bool depth_test,
        bool scissor_test)
        : alpha_mode_(alpha_mode),
          program_(program_handle),
          parameters_(std::move(parameters)),
          blend_(blend),
          culling_(culling),
          polygon_mode_(polygon_mode),
          depth_test_(depth_test),
          scissor_test_(scissor_test)
    {
    }

    material::material(program_handle program_handle) : program_(program_handle)
    {
    }

    alpha_mode material::get_alpha_mode() const
    {
        return alpha_mode_;
    }

    program_handle material::get_program() const
    {
        return program_;
    }

    size_t material::size() const
    {
        return parameters_.size();
    }

    bool material::get_scissor_test() const
    {
        return scissor_test_;
    }

    bool material::get_depth_test() const
    {
        return depth_test_;
    }

    material::const_iterator material::begin() const
    {
        return parameters_.begin();
    }

    material::const_iterator material::end() const
    {
        return parameters_.end();
    }

    material::iterator material::begin()
    {
        return parameters_.begin();
    }

    material::iterator material::end()
    {
        return parameters_.end();
    }

    const material_parameter& material::operator[](const size_t index) const
    {
        return parameters_[index];
    }

    material_parameter& material::operator[](const size_t index)
    {
        return parameters_[index];
    }

    material_parameter& material::operator[](const std::string& name)
    {
        return parameters_[name];
    }

    const blend& material::get_blend() const
    {
        return blend_;
    }

    const culling& material::get_culling() const
    {
        return culling_;
    }

    const polygon_mode& material::get_polygon_mode() const
    {
        return polygon_mode_;
    }
} // namespace moka
