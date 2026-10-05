// ui_c_api.h - SURFix3D UI module, stable C ABI.
#pragma once
#include "surfix3d/ui/ui_export.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct sfx_ui_config_t {
        int         width;        // initial window width  (0 -> default 1280)
        int         height;       // initial window height (0 -> default 720)
        const char* title;        // window title (NULL -> "SURFix3D")
        const char* assets_dir;   // assets root (NULL -> "<exe dir>/assets")
        int         argc;         // forwarded command line (files to open, ...)
        char** argv;
    } sfx_ui_config_t;

    // Runs the whole application (window, GL context, ImGui, main loop).
    // Blocks until the window is closed. Returns the process exit code.
    UI_API int sfx_ui_run(const sfx_ui_config_t* cfg);

#ifdef __cplusplus
}
#endif