# editor : open-source entry point. All UI lives in the (binary-shipped) ui
# module; this target only provides main() and the exe resources.

add_executable(editor)
set_target_properties(editor PROPERTIES OUTPUT_NAME SURFix3D)

set_target_properties(editor PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:editor>)

set(EDITOR_SOURCES
    src/main.cpp
    src/surfix3d.rc
)

target_sources(editor PRIVATE ${EDITOR_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SOURCES})

# Always sync assets (icons, fonts, shaders...) next to the exe, even when the
# editor itself did not relink (e.g. only a .glsl or the render lib changed).
add_custom_target(editor_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:editor>/assets
    COMMENT "Copying assets to output directory")
add_dependencies(editor_assets editor)

target_link_libraries(editor PRIVATE SURFix3D::ui)