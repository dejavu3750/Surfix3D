// units.hpp - Length-unit helpers shared by io (import) and editor (display).
// Pure math: never modifies model data.
#ifndef SURFIX3D_CORE_UNITS_HPP
#define SURFIX3D_CORE_UNITS_HPP

#include "surfix3d/core/core_c_api.h"

namespace surfix3d::core {

    enum class LengthUnit : int {
        MM = SFX_UNIT_MM, CM = SFX_UNIT_CM, M = SFX_UNIT_M,
        IN = SFX_UNIT_IN, FT = SFX_UNIT_FT, Count = SFX_UNIT_COUNT
    };

    // Size of one unit in millimeters.
    constexpr double unitToMM(LengthUnit u) {
        switch (u) {
        case LengthUnit::MM: return 1.0;
        case LengthUnit::CM: return 10.0;
        case LengthUnit::M:  return 1000.0;
        case LengthUnit::IN: return 25.4;
        case LengthUnit::FT: return 304.8;
        default:             return 1.0;
        }
    }

    // Multiply a value expressed in `from` by this to get it in `to`.
    constexpr double unitFactor(LengthUnit from, LengthUnit to) {
        return unitToMM(from) / unitToMM(to);
    }

    constexpr const char* unitName(LengthUnit u) {
        switch (u) {
        case LengthUnit::MM: return "mm";
        case LengthUnit::CM: return "cm";
        case LengthUnit::M:  return "m";
        case LengthUnit::IN: return "in";
        case LengthUnit::FT: return "ft";
        default:             return "?";
        }
    }

} // namespace surfix3d::core

#endif // SURFIX3D_CORE_UNITS_HPP