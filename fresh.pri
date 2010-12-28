# fresh project include file

# is it really needed here ?
DEFINES	*= FRESH_CORE_BUILD

# include functions file
include( functions.pri )

FRESH_SOURCES_PATHS	= $$getFolders( $$PWD/src )

# include path
INCLUDEPATH	*= $${PWD}/include \
	$${PWD}/src

# depend path
DEPENDPATH	*= $${FRESH_SOURCES_PATHS}

# dependency
PRE_TARGETDEPS	*= $${PWD}

# library integration
contains( TEMPLATE, .*app ) {
	FRESH_TARGET	= fresh
	QT	*= xml network
	LIBS	*= -L$${FRESH_LIBRARY_PATH}/build
	LIBS	*= -l$$targetForMode( $${FRESH_TARGET} )
}
