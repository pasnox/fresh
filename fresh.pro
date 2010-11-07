# fresh lib project file

# include functions file
include( functions.pri )

FRESH_BUILD_MODE	= debug
FRESH_BUILD_PATH	= build

TEMPLATE	= lib
CONFIG	-= debug_and_release release debug warn_off warn_on
CONFIG	*= qt staticlib warn_on thread x11 windows $$FRESH_BUILD_MODE
#QT	*= xml sql

# Mac universal build from 10.3 to up to 10.5
mac {
	QMAKE_MACOSX_DEPLOYMENT_TARGET	= 10.3
	QMAKE_MAC_SDK	= /Developer/SDKs/MacOSX10.4u.sdk
	CONFIG	*= x86 ppc
}

setTarget( fresh )
setTemporaryDirectories( $$FRESH_BUILD_PATH )
isEqual( FRESH_BUILD_MODE, debug ):CONFIG	*= console

# some library infos
QMAKE_TARGET_COMPANY	= "The MkS Team"
QMAKE_TARGET_PRODUCT	= "Fresh Framework"
QMAKE_TARGET_DESCRIPTION	= "Crossplatform Qt Extension Framework"
QMAKE_TARGET_COPYRIGHT	= "(C) 2005 - 2010 Filipe AZEVEDO and $$QMAKE_TARGET_COMPANY"

# make library exportable
DEFINES	*= FRESH_CORE_BUILD

FRESH_SOURCES_PATHS	= $$getFolders( ./src )
DEPENDPATH	*= $${FRESH_SOURCES_PATHS}
INCLUDEPATH	*= ./src

RESOURCES	*= resources/fresh.qrc

FORMS	*=  \
	src/gui/pActionsNodeShortcutEditor.ui

HEADERS	*= src/core/FreshExport.h \
	src/core/pGetOpt.h \
	src/core/pSettings.h \
	src/core/pSingleton.h \
	src/core/pVersion.h \
	src/core/pTranslationManager.h \
	src/core/pGroupPath.h \
	src/gui/pToolButton.h \
	src/gui/pKeySequenceEdit.h \
	src/gui/pDockToolBar.h \
	src/gui/pDockToolBarManager.h \
	src/gui/pMainWindow.h \
	src/gui/pActionsNode.h \
	src/gui/pActionsNodeModel.h \
	src/gui/pActionsNodeMenuBar.h \
	src/gui/pActionsNodeShortcutEditor.h \
	src/gui/pRecursiveSortFilterProxyModel.h \
	src/gui/pLineEdit.h \
	src/gui/pIconManager.h \
	src/gui/pPathListEditor.h \
	src/gui/pStringListEditor.h \
	src/gui/pFileListEditor.h \
	src/core/FileSystemUtils.h

SOURCES	*= src/core/pGetOpt.cpp \
	src/core/pSettings.cpp \
	src/core/pSingleton.cpp \
	src/core/pVersion.cpp \
	src/core/pTranslationManager.cpp \
	src/core/pGroupPath.cpp \
	src/gui/pToolButton.cpp \
	src/gui/pKeySequenceEdit.cpp \
	src/gui/pDockToolBar.cpp \
	src/gui/pDockToolBarManager.cpp \
	src/gui/pMainWindow.cpp \
	src/gui/pActionsNode.cpp \
	src/gui/pActionsNodeModel.cpp \
	src/gui/pActionsNodeMenuBar.cpp \
	src/gui/pActionsNodeShortcutEditor.cpp \
	src/gui/pRecursiveSortFilterProxyModel.cpp \
	src/gui/pLineEdit.cpp \
	src/gui/pIconManager.cpp \
	src/gui/pPathListEditor.cpp \
	src/gui/pStringListEditor.cpp \
	src/gui/pFileListEditor.cpp \
	src/core/FileSystemUtils.cpp