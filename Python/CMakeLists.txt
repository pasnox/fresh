# Build Python bindings
cmake_minimum_required(VERSION 2.8)

PROJECT(pyfresh)

set(CMAKE_MODULE_PATH 
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})

find_package(PythonInterp REQUIRED)
find_package(PythonLibrary REQUIRED)
include(PythonMacros)

find_package (Qt4 REQUIRED)
set(QT_USE_QTXML true)
set(QT_USE_QTNETWORK true)
include (${QT_USE_FILE})

find_package(PyQt4 REQUIRED)

find_package(SIP REQUIRED)
include(SIPMacros)

# Set a a PYTHON_3 flag.
if(${PYTHON_SHORT_VERSION} MATCHES "^3")
    set(PYTHON_3 TRUE)
else(${PYTHON_SHORT_VERSION} MATCHES "^3")
    set(PYTHON_3 FALSE)
endif(${PYTHON_SHORT_VERSION} MATCHES "^3")

include_directories(
    ${PYTHON_INCLUDE_PATH}
    ${SIP_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/../src
    ${CMAKE_SOURCE_DIR}/../build
)

set(SIP_INCLUDES ${PYQT4_SIP_DIR} sip)

if(WIN32)
    set(SIP_TAGS ALL WS_WIN ${PYQT4_VERSION_TAG})
else(WIN32)
    set(SIP_TAGS ALL WS_X11 ${PYQT4_VERSION_TAG})
endif(WIN32)
set(SIP_DISABLE_FEATURES VendorID PyQt_NoPrintRangeBug)

# Use an extra option when compiling on Python 3.
if(PYTHON_3)
    set(SIP_EXTRA_OPTIONS -g -x Py_v3)
else(PYTHON_3)
    set(SIP_EXTRA_OPTIONS -g)
endif(PYTHON_3)

file(GLOB_RECURSE ALL_SIP_FILES *.sip)
set(SIP_EXTRA_FILES_DEPEND ${ALL_SIP_FILES})

# SIP macros installs module to the PYTHON_SITE_PACKAGES_INSTALL_DIR
# We need redefine this variable to install fresh to PyQt4 module
execute_process(COMMAND ${PYTHON_EXECUTABLE} 
				"-c"
				"import os; import PyQt4; print os.path.dirname(PyQt4.__file__)"
				OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_INSTALL_DIR
				OUTPUT_STRIP_TRAILING_WHITESPACE)


add_sip_python_module(fresh fresh.sip
					  fresh
					  ${QT_CORE_LIBRARY} 
					  ${QT_GUI_LIBRARY} 
					  ${QT_XML_LIBRARY} 
					  ${QT_NETWORK_LIBRARY})

message(STATUS
		"PYTHON_SITE_PACKAGES_INSTALL_DIR:${PYTHON_SITE_PACKAGES_INSTALL_DIR}")

# Install the .sip files for anyone that wants to build bindings on top of fresh
file(GLOB_RECURSE SIP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.sip)
install(FILES ${SIP_FILES}
		DESTINATION ${SIP_DEFAULT_SIP_DIR}/PyQt4/fresh)
