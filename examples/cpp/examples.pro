###########################################################################################
##      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
##
##  Author    : You Name <your@email.org>
##  Project   : examples
##  FileName  : examples.pro
##  Date      : 2010-08-01T20:57:21
##  License   : GPL
##  Comment   : Creating using Monkey Studio RAD
##  Home Page   : http://www.mydomain.org
##
##  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
##  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
###########################################################################################
# include qmake-extensions file
include( ../../qmake-extensions.git/qmake-extensions.pri )

EXAMPLES_BUILD_MODE = release
EXAMPLES_BUILD_PATH = ../../build/examples

TEMPLATE    = app
LANGUAGE    = C++/Qt4
CONFIG  -= debug_and_release release debug warn_off warn_on
CONFIG  *= warn_on $${EXAMPLES_BUILD_MODE}
DESTDIR = ../../bin

setTarget( examples )
setTemporaryDirectories( $${EXAMPLES_BUILD_PATH} )
isEqual( EXAMPLES_BUILD_MODE, debug ):CONFIG    *= console

FRESH_BUNDLE_PATH = ../..
FRESH_BUNDLE_LIB_PATH = ../..
include( ../../fresh-bundle.pri )

exists( ../../../QtSolutions/modeltest-0.2/modeltest.pri ) {
    message( "Using QtSolution ModelTest." )
    DEFINES *= QT_MODELTEST
    include(../../../QtSolutions/modeltest-0.2/modeltest.pri )
}

HEADERS *= src/MainWindow.h

SOURCES *= src/main.cpp \
    src/MainWindow.cpp