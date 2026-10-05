// ui.hpp - header-only C++ convenience wrapper over ui_c_api.h.
#pragma once
#include "surfix3d/ui/ui_c_api.h"

namespace surfix3d::ui {

    inline int run(int argc, char** argv, const char* title = "SURFix3D") {
        sfx_ui_config_t cfg{};
        cfg.title = title;
        cfg.argc = argc;
        cfg.argv = argv;
        return sfx_ui_run(&cfg);
    }

} // namespace surfix3d::ui