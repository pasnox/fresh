# fresh project include file

# include functions file
include( functions.pri )

# include path
INCLUDEPATH	*= $${PWD}/src

# dependency
PRE_TARGETDEPS	*= $${PWD}

# library integration
contains( TEMPLATE, .*app ) {
	FRESH_TARGET	= fresh
	LIBS	*= -L$${FRESH_LIBRARY_PATH}
	LIBS	*= -l$$targetForMode( $${FRESH_TARGET} )
}
