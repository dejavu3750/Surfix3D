# ---------------------------------------------------------------------------
# AUTO-COPIED to SURFix3DPublic by scripts/<os>/publish.*
# SOURCE OF TRUTH: <private-repo>/publish/  -- edit here, then re-run publish.
# Do NOT edit the copy inside SURFix3DPublic/ ; it gets overwritten.
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)

# Public distribution of SURFix3D: OPEN editor source + PRE-BUILT engine/UI.
# core/io/render/ui (and their runtime deps glad/glfw) are consumed as binaries
# from prebuilt/ - there is no engine or UI source in this repository.
project(SURFix3D
    VERSION 0.1.0
    DESCRIPTION "SURFix3D - open editor + pre-built engine"
    LANGUAGES C CXX)

if(MSVC)
    enable_language(RC)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_DEBUG_POSTFIX d)

# Enable IDE solution folders (Visual Studio filters).
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# --- Locate the pre-built engine + UI shipped in prebuilt/ ---
set(SURFix3D_DIR "${CMAKE_SOURCE_DIR}/prebuilt/lib/cmake/SURFix3D"
    CACHE PATH "Pre-built SURFix3D engine package")
find_package(SURFix3D CONFIG REQUIRED)

# --- Editor application (open source) ---
add_subdirectory(editor)

# Make the editor the default startup project in Visual Studio.
if(TARGET editor)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT editor)
endif()

# --- Make the pre-built DLLs usable at runtime next to the editor ---
# add_custom_command(TARGET ...) only works in the directory that created the
# target, so use a separate target that runs after the editor is built.
if(WIN32)
    add_custom_target(editor_runtime ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/prebuilt/bin" "$<TARGET_FILE_DIR:editor>"
        COMMENT "Copying pre-built engine DLLs next to the editor")
    add_dependencies(editor_runtime editor)
else()
    # Let the editor find the pre-built .so files at run time.
    set_target_properties(editor PROPERTIES
        BUILD_RPATH "${CMAKE_SOURCE_DIR}/prebuilt/lib")
endif()