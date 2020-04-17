#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sentry_crashpad::handler" for configuration "Release"
set_property(TARGET sentry_crashpad::handler APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sentry_crashpad::handler PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/crashpad_handler.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS sentry_crashpad::handler )
list(APPEND _IMPORT_CHECK_FILES_FOR_sentry_crashpad::handler "${_IMPORT_PREFIX}/bin/crashpad_handler.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
