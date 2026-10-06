<<<<<<< HEAD
> **Note - generated file.** Copied from `publish/README.md` in the private repo by the `publish` script. Edit it there and re-run publish; changes made directly in `SURFix3DPublic/` are overwritten.
=======
## Demo
<img width="1613" height="1071" alt="Image" src="https://github.com/user-attachments/assets/27843c42-ff0a-4d48-9cb2-4a06131389bb" />

### Credits

The car model shown in the demo:

- **Model:** [Free_1975_porsche_911_930_turbo](https://sketchfab.com/3d-models/free-1975-porsche-911-930-turbo-cc56531bdab148b5b646e46c7006dc97)
- **Author:** [rar.z]
- **License:** [CC Attribution - Creative Commons Attribution](https://creativecommons.org/licenses/by/4.0/)
  > Author must be credited. Commercial use is allowed.

The model is used unmodified, for demonstration purposes only. It is **not**
distributed with this repository.
>>>>>>> 1b42395a9f9bd68127fa65280f71982518a024b4

# SURFix3D (Public Distribution) 🚀

This is the **public** distribution of SURFix3D. The **editor launcher is open
source**. The **engine (`core` / `io` / `render`) and the application layer
(`ui`)** ship pre-built under `prebuilt/` as binaries plus public headers only.
This repository does not contain the engine or UI source.

The editor is a thin executable. It provides `main()`, the Windows resources
(icon, version info) and the runtime assets, then hands control to the
pre-built UI through its public C API (`surfix3d/ui/ui_c_api.h`):

```cpp
#include "surfix3d/ui/ui.hpp"

int main(int argc, char** argv) {
    return surfix3d::ui::run(argc, argv);
}
```

To learn what the engine does and how it is built, see the project website and docs.

## Requirements

- **CMake ≥ 3.16**
- A C++17 compiler. On Windows, use **Visual Studio 2022 (x64)**.
- An OpenGL 3.3 capable GPU / driver

You do not need submodules or any third-party source. Every dependency the
editor needs at link time and at run time is in `prebuilt/`.

## Build

```sh
# 1) Clone
git clone <repo-url>
cd SURFix3DPublic

# 2) One-click
scripts/linux/all.sh          # or  scripts\windows\all.bat
```

You can also run the steps one at a time:

| Task  | Windows                      | Linux                    |
|-------|------------------------------|--------------------------|
| Setup | `scripts\windows\setup.bat`  | `scripts/linux/setup.sh` |
| Build | `scripts\windows\build.bat`  | `scripts/linux/build.sh` |
| All   | `scripts\windows\all.bat`    | `scripts/linux/all.sh`   |
| Clean | `scripts\windows\clean.bat`  | `scripts/linux/clean.sh` |

- `setup` generates the build system into `build/`. On Windows, open
  `build\SURFix3D.sln` in Visual Studio. `editor` is the startup project.
- `build` builds **Release** by default. To build Debug, set `CONFIG=Debug` first
  (`set CONFIG=Debug` on Windows).
- `clean` removes `build/`, `bin/` and `lib/`. It never touches `prebuilt/`.
- On Windows, the scripts pause at the end so you can read the log. Set
  `NOPAUSE=1` to turn this off for CI or other scripted runs.

The build places the executable (`SURFix3D.exe` / `SURFix3D`), the pre-built
engine DLLs and `assets/` together in `bin/<Config>/`, for example
`bin/Release/`. The program runs from there and needs no `PATH` changes.

## Layout

```plaintext
SURFix3DPublic/
├── editor/          # open-source launcher: main() + resources (.rc / icon)
├── assets/          # icons, fonts, shaders... copied next to the executable
├── prebuilt/        # pre-built package (Debug + Release, stripped, no PDB)
│   ├── bin/         #   core, io, render, ui, glad, glfw3 (.dll; Debug = *d.dll)
│   ├── lib/         #   import libs + cmake/SURFix3D/ (find_package config)
│   └── include/     #   public C API headers (surfix3d/{core,io,render,ui}/...)
├── scripts/         # setup / build / clean / all  (Windows + Linux)
└── CMakeLists.txt   # find_package(SURFix3D) from prebuilt/, builds the editor
```

## How it fits together

```plaintext
editor (SURFix3D.exe)  ──links──▶  SURFix3D::ui
                                       │  (private, inside the DLLs)
                                       ├─▶ SURFix3D::core / io / render
                                       └─▶ glad, glfw, imgui (static in ui)
```

The editor only links `SURFix3D::ui`. Its dependencies are internal to the
pre-built DLLs, so the editor needs no glm, glfw, imgui or glad headers.

## Updating

Do not edit files in this repository by hand. They are regenerated from the
private repo:

1. `scripts\windows\package.bat` builds and strips the engine and UI, then writes them to `dist\`.
2. `scripts\windows\publish.bat` copies `editor/`, `assets/`, `scripts/` and `dist\` (as `prebuilt/`) into this repository.
<<<<<<< HEAD
3. Commit and push here.
=======
3. Commit and push here.

## License

- Source in this repository (`editor/`, `scripts/`, build files): **MIT** - see [LICENSE](LICENSE).
- Pre-built engine and UI (`prebuilt/`): **free to use, proprietary** - see [prebuilt/LICENSE.txt](prebuilt/LICENSE.txt).
- Third-party components: see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
>>>>>>> 1b42395a9f9bd68127fa65280f71982518a024b4
