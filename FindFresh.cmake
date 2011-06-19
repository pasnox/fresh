# Try to find Fresh library
# Once done this will define
#  FRESH_FOUND - System has Fresh
#  FRESH_INCLUDE_DIR - The Fresh include directory
#  FRESH_LIBRARY - The libraries needed to use Fresh
#  FRESH_TRANSLATIONS - Fresh translation files

find_package (Qt4)

find_path(FRESH_INCLUDE_DIR FreshCore PATHS ${QT_HEADERS_DIR})
find_library(FRESH_LIBRARY NAMES fresh PATHS ${QT_LIBRARY_DIR})
file(GLOB_RECURSE FRESH_TRANSLATIONS ${QT_TRANSLATIONS_DIR}/fresh*.ts)


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set FRESH_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Fresh  DEFAULT_MSG
                                  FRESH_LIBRARY FRESH_INCLUDE_DIR)

mark_as_advanced(FRESH_INCLUDE_DIR FRESH_LIBRARY)
