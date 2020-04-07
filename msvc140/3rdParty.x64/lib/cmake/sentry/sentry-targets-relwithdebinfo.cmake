#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sentry::crashpad_handler" for configuration "RelWithDebInfo"
set_property(TARGET sentry::crashpad_handler APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(sentry::crashpad_handler PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/crashpad_handler.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS sentry::crashpad_handler )
list(APPEND _IMPORT_CHECK_FILES_FOR_sentry::crashpad_handler "${_IMPORT_PREFIX}/bin/crashpad_handler.exe" )

# Import target "sentry::sentry" for configuration "RelWithDebInfo"
set_property(TARGET sentry::sentry APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(sentry::sentry PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/sentry.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/sentry.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS sentry::sentry )
list(APPEND _IMPORT_CHECK_FILES_FOR_sentry::sentry "${_IMPORT_PREFIX}/lib/sentry.lib" "${_IMPORT_PREFIX}/bin/sentry.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
