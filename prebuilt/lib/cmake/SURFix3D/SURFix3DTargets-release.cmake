#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SURFix3D::core" for configuration "Release"
set_property(TARGET SURFix3D::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SURFix3D::core PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/core.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/core.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::core )
list(APPEND _cmake_import_check_files_for_SURFix3D::core "${_IMPORT_PREFIX}/lib/core.lib" "${_IMPORT_PREFIX}/bin/core.dll" )

# Import target "SURFix3D::io" for configuration "Release"
set_property(TARGET SURFix3D::io APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SURFix3D::io PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/io.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "SURFix3D::core"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/io.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::io )
list(APPEND _cmake_import_check_files_for_SURFix3D::io "${_IMPORT_PREFIX}/lib/io.lib" "${_IMPORT_PREFIX}/bin/io.dll" )

# Import target "SURFix3D::render" for configuration "Release"
set_property(TARGET SURFix3D::render APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SURFix3D::render PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/render.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "SURFix3D::core;SURFix3D::glad"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/render.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::render )
list(APPEND _cmake_import_check_files_for_SURFix3D::render "${_IMPORT_PREFIX}/lib/render.lib" "${_IMPORT_PREFIX}/bin/render.dll" )

# Import target "SURFix3D::ui" for configuration "Release"
set_property(TARGET SURFix3D::ui APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SURFix3D::ui PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/ui.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "SURFix3D::core;SURFix3D::io;SURFix3D::render;SURFix3D::glad;SURFix3D::glfw"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/ui.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::ui )
list(APPEND _cmake_import_check_files_for_SURFix3D::ui "${_IMPORT_PREFIX}/lib/ui.lib" "${_IMPORT_PREFIX}/bin/ui.dll" )

# Import target "SURFix3D::glad" for configuration "Release"
set_property(TARGET SURFix3D::glad APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SURFix3D::glad PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/glad.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/glad.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::glad )
list(APPEND _cmake_import_check_files_for_SURFix3D::glad "${_IMPORT_PREFIX}/lib/glad.lib" "${_IMPORT_PREFIX}/bin/glad.dll" )

# Import target "SURFix3D::glfw" for configuration "Release"
set_property(TARGET SURFix3D::glfw APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SURFix3D::glfw PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/glfw3dll.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/glfw3.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::glfw )
list(APPEND _cmake_import_check_files_for_SURFix3D::glfw "${_IMPORT_PREFIX}/lib/glfw3dll.lib" "${_IMPORT_PREFIX}/bin/glfw3.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
