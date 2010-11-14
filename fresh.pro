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
QMAKE_TARGET_COMPANY	= "The Fresh Team"
QMAKE_TARGET_PRODUCT	= "Fresh Framework"
QMAKE_TARGET_DESCRIPTION	= "Qt Extension Framework"
QMAKE_TARGET_COPYRIGHT	= "(C) 2005 - 2010 Filipe AZEVEDO and $$QMAKE_TARGET_COMPANY"

# make library exportable
DEFINES	*= FRESH_CORE_BUILD

FRESH_SOURCES_PATHS	= $$getFolders( ./src )
DEPENDPATH	*= $${FRESH_SOURCES_PATHS}
INCLUDEPATH	*= ./src

RESOURCES	*= resources/fresh.qrc

FORMS	*=  \
	src/gui/actionmanager/pActionsNodeShortcutEditor.ui \
	src/gui/pTranslationDialog.ui

HEADERS	*=  \
	src/core/FreshExport.h \
	src/core/pFileSystemUtils.h \
	src/core/pGetOpt.h \
	src/core/pGroupPath.h \
	src/core/pSettings.h \
	src/core/pSingleton.h \
	src/core/pTranslationManager.h \
	src/core/pVersion.h \
	src/gui/pColorButton.h \
	src/gui/pDrawingUtils.h \
	src/gui/pIconManager.h \
	src/gui/pKeySequenceEdit.h \
	src/gui/pLineEdit.h \
	src/gui/pMainWindow.h \
	src/gui/pToolButton.h \
	src/gui/actionmanager/pActionsNode.h \
	src/gui/actionmanager/pActionsNodeMenuBar.h \
	src/gui/actionmanager/pActionsNodeModel.h \
	src/gui/actionmanager/pActionsNodeShortcutEditor.h \
	src/gui/dockmanager/pDockToolBar.h \
	src/gui/dockmanager/pDockToolBarManager.h \
	src/gui/dockmanager/pDockToolBarManagerModernWidget.h \
	src/gui/listeditor/pFileListEditor.h \
	src/gui/listeditor/pPathListEditor.h \
	src/gui/listeditor/pStringListEditor.h \
	src/gui/models/pRecursiveSortFilterProxyModel.h \
	src/gui/pTranslationDialog.h \
	src/gui/queuedmessage/pQueuedMessageToolBar.h \
	src/gui/queuedmessage/pQueuedMessageWidget.h

SOURCES	*=  \
	src/core/pFileSystemUtils.cpp \
	src/core/pGetOpt.cpp \
	src/core/pGroupPath.cpp \
	src/core/pSettings.cpp \
	src/core/pSingleton.cpp \
	src/core/pTranslationManager.cpp \
	src/core/pVersion.cpp \
	src/gui/pColorButton.cpp \
	src/gui/pDrawingUtils.cpp \
	src/gui/pIconManager.cpp \
	src/gui/pKeySequenceEdit.cpp \
	src/gui/pLineEdit.cpp \
	src/gui/pMainWindow.cpp \
	src/gui/pToolButton.cpp \
	src/gui/actionmanager/pActionsNode.cpp \
	src/gui/actionmanager/pActionsNodeMenuBar.cpp \
	src/gui/actionmanager/pActionsNodeModel.cpp \
	src/gui/actionmanager/pActionsNodeShortcutEditor.cpp \
	src/gui/dockmanager/pDockToolBar.cpp \
	src/gui/dockmanager/pDockToolBarManager.cpp \
	src/gui/dockmanager/pDockToolBarManagerModernWidget.cpp \
	src/gui/listeditor/pFileListEditor.cpp \
	src/gui/listeditor/pPathListEditor.cpp \
	src/gui/listeditor/pStringListEditor.cpp \
	src/gui/models/pRecursiveSortFilterProxyModel.cpp \
	src/gui/pTranslationDialog.cpp \
	src/gui/queuedmessage/pQueuedMessageToolBar.cpp \
	src/gui/queuedmessage/pQueuedMessageWidget.cpp