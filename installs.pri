# Fresh Install Project File

!build_pass:!no_fresh_install {
    mac:universal {
        UNIVERSAL_TEXT  = "(universal)"
    }

    FRESH_INSTALL_HEADERS   = $$[QT_INSTALL_HEADERS]
    FRESH_INSTALL_LIBS  = $$[QT_INSTALL_LIBS]
    FRESH_INSTALL_TRANSLATIONS  = $$[QT_INSTALL_TRANSLATIONS]
    FRESH_INSTALL_FEATURES  = $$[QMAKE_MKSPECS]/features

    cb_win32 {
        FRESH_INSTALL_HEADERS   = $(QT_WIN32_PATH)/include
        FRESH_INSTALL_LIBS  = $(QT_WIN32_PATH)/lib
        FRESH_INSTALL_TRANSLATIONS  = $(QT_WIN32_PATH)/translations
        FRESH_INSTALL_FEATURES  = $(QT_WIN32_PATH)/mkspecs/features
    }

    message( "Configuring Fresh..." )
    message( "Build mode: $${FRESH_BUILD_MODE}" )
    message( "Build type: $${FRESH_BUILD_TYPE} $${UNIVERSAL_TEXT}" )
    message( "Install headers: $${FRESH_INSTALL_HEADERS}" )
    message( "Install libs: $${FRESH_INSTALL_LIBS}" )
    message( "Install translations: $${FRESH_INSTALL_TRANSLATIONS}" )
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
    fresh_core_headers.path = $${FRESH_INSTALL_HEADERS}/FreshCore
    fresh_core_headers.files    = include/FreshCore/*
    win32:!cb_win32:fresh_core_headers.files    *= $$system( for /R src/core %i in (*.h) do @echo %i )
    else:fresh_core_headers.files   *= $$system( find src/core -name '*.h' )
    fresh_core_headers.files = $$replace( fresh_core_headers.files, $${Q_BACK_SLASH}, $${Q_SLASH} )

    # gui headers
    fresh_gui_headers.path  = $${FRESH_INSTALL_HEADERS}/FreshGui
    fresh_gui_headers.files = include/FreshGui/*
    win32:!cb_win32:fresh_gui_headers.files *= $$system( for /R src/gui %i in (*.h) do @echo %i )
    else:fresh_gui_headers.files    *= $$system( find src/gui -name '*.h' )
    fresh_gui_headers.files = $$replace( fresh_gui_headers.files, $${Q_BACK_SLASH}, $${Q_SLASH} )

    # generate fresh.prf
    # remove old one if needed
    prf_file = $${OUT_PWD}/fresh.prf
    win32:!cb_win32:prf_file = $$replace( prf_file, $${Q_SLASH}, $${Q_BACK_SLASH} )
    
    exists( $${prf_file} ) {
        win32:!cb_win32 {
            system( "del $${prf_file}" )
        } else {
            system( "rm $${prf_file}" )
        }
    }

    # create target directory if needed
    path = $$dirname( prf_file )
    
    !isEmpty( path ):!exists( $${path} ) {
        win32:!cb_win32 {
            system( "mkdir $${path}" )
        } else {
            system( "mkdir -p $${path}" )
        }
    }
    
    # create new one based on build type
    isEqual( FRESH_BUILD_TYPE, static ) {
        win32:!cb_win32 {
            system( "copy fresh_static.prf $${prf_file}" )
        } else {
            system( "cp fresh_static.prf $${prf_file}" )
        }
    } else {
        win32:!cb_win32 {
            system( "copy fresh_shared.prf $${prf_file}" )
        } else {
            system( "cp fresh_shared.prf $${prf_file}" )
        }
    }

    # features
    features.path = $${FRESH_INSTALL_FEATURES}
    features.files = $${prf_file}

    # translations
    translations.path   = $${FRESH_INSTALL_TRANSLATIONS}
    translations.files  = translations/*.ts
    translations.files  *= translations/*.qm

    # library
    target.path = $${FRESH_INSTALL_LIBS}

    INSTALLS    = fresh_core_headers fresh_gui_headers features  translations target
}
