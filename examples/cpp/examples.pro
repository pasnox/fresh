###########################################################################################
##		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
##
##	Author    : You Name <your@email.org>
##	Project   : examples
##	FileName  : examples.pro
##	Date      : 2010-08-01T20:57:21
##	License   : GPL
##	Comment   : Creating using Monkey Studio RAD
##	Home Page   : http://www.mydomain.org
##
##	This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##	WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################
XUP.OTHERS_PLATFORM_TARGET_RELEASE	= examples
XUP.OTHERS_PLATFORM_TARGET_DEBUG	= examples_debug

FRESH_PATH = ../..

# include functions file
include( $${FRESH_PATH}/functions.pri )

EXAMPLES_BUILD_MODE	= release
EXAMPLES_BUILD_PATH	= build

TEMPLATE	= app
LANGUAGE	= C++/Qt4
CONFIG	-= debug_and_release release debug warn_off warn_on
CONFIG	*= warn_on $${EXAMPLES_BUILD_MODE}
DESTDIR	= $$PWD

setTarget( examples )
setTemporaryDirectories( $${EXAMPLES_BUILD_PATH} )
isEqual( EXAMPLES_BUILD_MODE, debug ):CONFIG	*= console

unix {
	UNIX_RAM_DISK	= /media/ramdisk
	exists( $${UNIX_RAM_DISK} ) {
		EXAMPLES_BUILD_PATH	= $${UNIX_RAM_DISK}/$$targetForMode( fresh, $${EXAMPLES_BUILD_MODE} )/$${TARGET}
		DESTDIR	= $${EXAMPLES_BUILD_PATH}
		setTemporaryDirectories( $${EXAMPLES_BUILD_PATH} )
	}
}

fresh {
	!build_pass:message( "Using system fresh library." )
} else {
	!build_pass:message( "Using bundled fresh library." )

	FRESH_TARGET	= $$targetForMode( fresh, $${EXAMPLES_BUILD_MODE} )
	FRESH_BUILD_PATH = $${FRESH_PATH}/build
	FRESH_SOURCES_PATHS	= $$getFolders( $${FRESH_PATH}/src )

	DEPENDPATH *= $${FRESH_PATH}/include/FreshCore \
		$${FRESH_PATH}/include/FreshGui

	INCLUDEPATH	*= $${FRESH_PATH}/include

	DEPENDPATH	*= $${FRESH_SOURCES_PATHS}
	INCLUDEPATH	*= $${FRESH_SOURCES_PATHS}

	PRE_TARGETDEPS	*= $${FRESH_PATH}

	unix {
		!exists( $${FRESH_BUILD_PATH} ) {
			FRESH_BUILD_PATH = $${UNIX_RAM_DISK}/$${FRESH_TARGET}
		}
	}

	QT	*= xml network
	QMAKE_RPATHDIR *= $${FRESH_BUILD_PATH}
	macx:LIBS	*= -F$${FRESH_BUILD_PATH}
	LIBS	*= -L$${FRESH_BUILD_PATH}
	LIBS	*= $${FRESH_BUILD_PATH}/lib$${FRESH_TARGET}.a
}

exists( ../../../QtSolutions/modeltest-0.2/modeltest.pri ) {
	message( "Using QtSolution ModelTest." )
	DEFINES	*= QT_MODELTEST
	include(../../../QtSolutions/modeltest-0.2/modeltest.pri )
}

RESOURCES	*= ../../resources/fresh.qrc

HEADERS	*= src/MainWindow.h

SOURCES	*= src/main.cpp \
	src/MainWindow.cpp