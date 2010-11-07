q_os	= $$lower( $$QMAKE_HOST.os )

# return the list of directories recursively from a given path ($$1)
# Forbidden filters can be given ($$2)
defineReplace( getFolders ) {
	q_paths	= $$1
	q_filters	= $$2 .svn
	q_folders	=

	for( q_path, q_paths ) {
		command	= "ls -RQ1 \"$$q_path\" | grep \":\" | sed \"s/://g\" | sed \"s/'/\\\\\\'/g\""
		win32:isEqual( q_os, windows ):command	= "for /D /R \"$$q_path\" %i in (*) do @echo \"%i\""
		_q_folders	= $$system( $$command )
		win32:_q_folders *= $$1

		for( q_folder, _q_folders ) {
			filtered = false

			for( q_filter, q_filters ) {
				result = $$find( q_folder, $$q_filter )
				!isEmpty( result ):filtered = true
			}

			isEqual( filtered, false ):exists( $$q_folder ) {
				q_folders	*= $$q_folder
			}
		}
	}

	return( $$q_folders )
}

# return the project build mode
defineReplace( buildMode ) {
	CONFIG( debug, debug|release ) {
		return( debug )
	} else {
		return( release )
	}
}

# return the target name ($$1) according to build mode($$2)
defineReplace( targetForMode ) {
	q_target	= $$1
	q_mode	= $$2
	isEmpty( q_mode ):q_mode	= $$buildMode()
	
	isEqual( q_mode, release ) {
		q_target	= $$quote( $$q_target )
	} else {
		unix:q_target	= $$quote( $$join( q_target, , , _debug ) )
		else:q_target	= $$quote( $$join( q_target, , , d ) )
	}
	
	return( $$q_target )
}

# this function set the target name ($$1) according to the build mode ($$2)
defineTest( setTarget ) {
	TARGET	= $$targetForMode( $$1, $$2 )
	
	export( TARGET )
}

defineTest( setTemporaryDirectories ) {
	q_build_path	= $$1
	q_mode	= $$buildMode()

	OBJECTS_DIR	= $${q_build_path}/$${q_mode}/$${q_os}/obj
	UI_DIR	= $${q_build_path}/$${q_mode}/$${q_os}/ui
	MOC_DIR	= $${q_build_path}/$${q_mode}/$${q_os}/moc
	RCC_DIR	= $${q_build_path}/$${q_mode}/$${q_os}/rcc

	export( OBJECTS_DIR )
	export( UI_DIR )
	export( MOC_DIR )
	export( RCC_DIR )
}
