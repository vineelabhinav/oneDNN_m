#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "DNNL::dnnl" for configuration "Release"
set_property(TARGET DNNL::dnnl APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(DNNL::dnnl PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libdnnl.so.3.2"
  IMPORTED_SONAME_RELEASE "libdnnl.so.3"
  )

list(APPEND _cmake_import_check_targets DNNL::dnnl )
list(APPEND _cmake_import_check_files_for_DNNL::dnnl "${_IMPORT_PREFIX}/lib/libdnnl.so.3.2" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
