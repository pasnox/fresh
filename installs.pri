# Fresh Install Project File

!build_pass {
	mac:universal {
		UNIVERSAL_TEXT	= "(universal)"
	}
	
	FRESH_INSTALL_HEADERS	= $$[QT_INSTALL_HEADERS]
	FRESH_INSTALL_LIBS	= $$[QT_INSTALL_LIBS]
	FRESH_INSTALL_TRANSLATIONS	= $$[QT_INSTALL_TRANSLATIONS]
	FRESH_INSTALL_FEATURES	= $$[QMAKE_MKSPECS]/features
	
	message( "Configuring Fresh..." )
	message( "Build mode: $$FRESH_BUILD_MODE" )
	message( "Build type: $$FRESH_BUILD_TYPE $$UNIVERSAL_TEXT" )
	message( "Instal headers: $$FRESH_INSTALL_HEADERS" )
	message( "Instal libs: $$FRESH_INSTALL_LIBS" )
	message( "Instal translations: $$FRESH_INSTALL_TRANSLATIONS" )
	message( "You can change the build mode:" )
	message( "    qmake \"build_mode = release\"" )
	message( "    qmake \"build_mode = debug\"" )
	message( "You can change the build type:" )
	message( "    qmake \"build_type = static\"" )
	message( "    qmake \"build_type = shared\"" )
	mac:!universal {
		message( "You can create a universal build:" )
		message( "    qmake \"CONFIG *= universal\"" )
	}
	message( "Multiple options can be use at same time" )
	message( "Configuration complete" )

	# core headers
	fresh_core_headers.path	= $$FRESH_INSTALL_HEADERS/FreshCore
	fresh_core_headers.files	= include/FreshCore/*
	win32:fresh_core_headers.files	*= $$system( for /R src/core %i in (*.h) do @echo %i )
	else:fresh_core_headers.files	*= $$system( find src/core -name '*.h' )
	
	fresh_core_headers.files = $$replace( fresh_core_headers.files, "\\\\", "/" )
	
	# gui headers
	fresh_gui_headers.path	= $$FRESH_INSTALL_HEADERS/FreshGui
	fresh_gui_headers.files	= include/FreshGui/*
	win32:fresh_gui_headers.files	*= $$system( for /R src/gui %i in (*.h) do @echo %i )
	else:fresh_gui_headers.files	*= $$system( find src/gui -name '*.h' )
	
	fresh_gui_headers.files = $$replace( fresh_gui_headers.files, "\\\\", "/" )
	
	# features
	features.path = $$FRESH_INSTALL_FEATURES
	features.files = fresh_shared.prf \
		fresh_static.prf
	
	# translations
	translations.path	= $$FRESH_INSTALL_TRANSLATIONS
	translations.files	= translations/*.qm
	
	# library
	target.path	= $$FRESH_INSTALL_LIBS

	INSTALLS	= fresh_core_headers fresh_gui_headers features  translations target
}
