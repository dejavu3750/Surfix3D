#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SURFix3D::core" for configuration "Debug"
set_property(TARGET SURFix3D::core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SURFix3D::core PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/cored.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/cored.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::core )
list(APPEND _cmake_import_check_files_for_SURFix3D::core "${_IMPORT_PREFIX}/lib/cored.lib" "${_IMPORT_PREFIX}/bin/cored.dll" )

# Import target "SURFix3D::io" for configuration "Debug"
set_property(TARGET SURFix3D::io APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SURFix3D::io PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/iod.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "SURFix3D::core"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/iod.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::io )
list(APPEND _cmake_import_check_files_for_SURFix3D::io "${_IMPORT_PREFIX}/lib/iod.lib" "${_IMPORT_PREFIX}/bin/iod.dll" )

# Import target "SURFix3D::render" for configuration "Debug"
set_property(TARGET SURFix3D::render APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SURFix3D::render PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/renderd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "SURFix3D::core;SURFix3D::glad"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/renderd.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::render )
list(APPEND _cmake_import_check_files_for_SURFix3D::render "${_IMPORT_PREFIX}/lib/renderd.lib" "${_IMPORT_PREFIX}/bin/renderd.dll" )

# Import target "SURFix3D::ui" for configuration "Debug"
set_property(TARGET SURFix3D::ui APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SURFix3D::ui PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/uid.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "SURFix3D::core;SURFix3D::io;SURFix3D::render;SURFix3D::glad;SURFix3D::glfw"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/uid.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::ui )
list(APPEND _cmake_import_check_files_for_SURFix3D::ui "${_IMPORT_PREFIX}/lib/uid.lib" "${_IMPORT_PREFIX}/bin/uid.dll" )

# Import target "SURFix3D::glad" for configuration "Debug"
set_property(TARGET SURFix3D::glad APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SURFix3D::glad PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/gladd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/gladd.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::glad )
list(APPEND _cmake_import_check_files_for_SURFix3D::glad "${_IMPORT_PREFIX}/lib/gladd.lib" "${_IMPORT_PREFIX}/bin/gladd.dll" )

# Import target "SURFix3D::glfw" for configuration "Debug"
set_property(TARGET SURFix3D::glfw APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(SURFix3D::glfw PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/glfw3ddll.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/glfw3d.dll"
  )

list(APPEND _cmake_import_check_targets SURFix3D::glfw )
list(APPEND _cmake_import_check_files_for_SURFix3D::glfw "${_IMPORT_PREFIX}/lib/glfw3ddll.lib" "${_IMPORT_PREFIX}/bin/glfw3d.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
