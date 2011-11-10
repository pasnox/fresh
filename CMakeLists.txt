cmake_minimum_required(VERSION 2.8)

option(shared "Build shared library" y)

if (shared)
	message("*** Building SHARED library. Use `cmake -Dshared=0` for build static.")
else (shared)
	message("*** Building STATIC library. Use `cmake -Dshared=1` for build shared.")
endif (shared)

#message("*** Library and headers will be installed to " ${CMAKE_INSTALL_PREFIX})
#message("*** Use `cmake -DCMAKE_INSTALL_PREFIX=/your/install/path` for set another path")
message("")

set (PROJECT fresh)
set (VERSION 1.1.0)
 
project (${PROJECT})

set(BUILD_SHARED_LIBS ${shared})

# Find Qt4
find_package (Qt4 REQUIRED)
set(QT_USE_QTXML true)
set(QT_USE_QTNETWORK true)
include (${QT_USE_FILE})

# Set headers and sources list of the library
file(GLOB_RECURSE FORMS src/*.ui)
file(GLOB_RECURSE HEADERS ./src/*.h)
file(GLOB_RECURSE CORE_HEADERS include/FreshCore/* ./src/core/*.h)
file(GLOB_RECURSE GUI_HEADERS include/FreshGui/* ./src/gui/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE TS_FILES translations/*.ts)
file(GLOB_RECURSE QM_FILES translations/*.qm)
if (shared)
	set(PRF_FILE fresh_shared.prf)
else (shared)
	set(PRF_FILE fresh_static.prf)
endif (shared)

# Process MOC, UI and TS files
qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
qt4_wrap_ui( UI_HEADERS ${FORMS} )
# qt4_add_translation(TRANSLATIONS ${TS_FILES}) it seems, we don't need generate translations for the lib
qt4_add_resources(RESOURCES resources/fresh.qrc)

# Build main library
add_library (${PROJECT} ${UI_HEADERS} ${HEADERS} ${SOURCES} ${RESOURCES} ${MOC_SOURCES} ${TRANSLATIONS})
if(CMAKE_COMPILER_IS_GNUCC)
    set_target_properties(${PROJECT} PROPERTIES
			    COMPILE_FLAGS -fPIC)
endif(CMAKE_COMPILER_IS_GNUCC)

set_target_properties(${PROJECT} PROPERTIES
                         VERSION ${VERSION}
                         SOVERSION ${VERSION}
                     )

include_directories (   "include"
                        "src"
                        "src/core"
                        "src/gui"  
                        "src/gui/dockwidget"
                        "src/gui/dockmanager"
                        "src/gui/environmentvariablemanager"
                        "src/gui/listeditor"
                        "src/gui/models"
                        "src/gui/stylemanager"
                        "src/gui/queuedmessage"
                        "src/gui/gc-updatechecker"
                        "src/gui/consolemanager"
                        "src/gui/actionmanager" )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} ) # for .ui
target_link_libraries (${PROJECT} ${QT_LIBRARIES})

file(COPY ${CMAKE_SOURCE_DIR}/${PRF_FILE} DESTINATION ${CMAKE_BINARY_DIR})
file(RENAME ${CMAKE_BINARY_DIR}/${PRF_FILE} ${CMAKE_BINARY_DIR}/fresh.prf)

# Install main library
install(TARGETS fresh LIBRARY DESTINATION ${QT_LIBRARY_DIR})
install(FILES ${CORE_HEADERS} DESTINATION ${QT_HEADERS_DIR}/FreshCore)
install(FILES ${GUI_HEADERS} DESTINATION ${QT_HEADERS_DIR}/FreshGui)
install(FILES ${CMAKE_BINARY_DIR}/fresh.prf DESTINATION ${QT_MKSPECS_DIR}/features)
install(FILES ${TS_FILES} DESTINATION ${QT_TRANSLATIONS_DIR})
install(FILES ${QM_FILES} DESTINATION ${QT_TRANSLATIONS_DIR})
install(FILES ./FindFresh.cmake DESTINATION ${CMAKE_ROOT}/Modules)

add_subdirectory(Python ${CMAKE_CURRENT_BINARY_DIR}/python)
