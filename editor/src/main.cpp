// === SURFix3D public copy - DO NOT EDIT HERE. Edit editor\ in the private repo, then run publish. ===
// main.cpp - SURFix3D editor entry point.
// The whole application (window, ImGui, panels) lives in the ui module.
#include "surfix3d/ui/ui.hpp"

// Force the discrete GPU on hybrid laptops (Optimus / PowerXpress).
// Must be exported from the EXECUTABLE that owns the GL context.
#ifdef _WIN32
extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;          // NVIDIA
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;            // AMD
}
#endif

int main(int argc, char** argv) {
    return surfix3d::ui::run(argc, argv);
}