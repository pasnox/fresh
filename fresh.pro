# fresh lib project file

# include functions file
include( functions.pri )

isEmpty( build_mode ):FRESH_BUILD_MODE	= release
else:FRESH_BUILD_MODE	= $$build_mode

isEmpty( build_type ):FRESH_BUILD_TYPE	= static
else:FRESH_BUILD_TYPE	= $$build_type

FRESH_BUILD_PATH	= build
FRESH_DESTDIR	= build

TEMPLATE	= lib
CONFIG	-= debug_and_release release debug warn_off warn_on
CONFIG	*= qt warn_on thread x11 windows $$FRESH_BUILD_MODE $$FRESH_BUILD_TYPE
QT	*= xml network

# Mac universal build from 10.3 to up to 10.5
mac:universal {
	QMAKE_MACOSX_DEPLOYMENT_TARGET	= 10.3
	QMAKE_MAC_SDK	= /Developer/SDKs/MacOSX10.4u.sdk
	CONFIG	*= x86 ppc
}

setTarget( fresh )
setTemporaryDirectories( $$FRESH_BUILD_PATH )
isEqual( FRESH_BUILD_MODE, debug ):CONFIG	*= console

isEqual( FRESH_BUILD_TYPE, static ) {
	DESTDIR	= $$FRESH_DESTDIR
} else {
	win32:DLLDESTDIR	= $$FRESH_DESTDIR
	else:DESTDIR	= $$FRESH_DESTDIR
}

# some library infos
QMAKE_TARGET_COMPANY	= "The Fresh Team"
QMAKE_TARGET_PRODUCT	= "Fresh Framework"
QMAKE_TARGET_DESCRIPTION	= "Qt Extension Framework"
QMAKE_TARGET_COPYRIGHT	= "(C) 2005 - 2011 Filipe AZEVEDO and $$QMAKE_TARGET_COMPANY"
VERSION	= 1.0.0

# make library exportable
DEFINES	*= FRESH_CORE_BUILD

FRESH_SOURCES_PATHS	= $$getFolders( ./src )
DEPENDPATH	*= $${FRESH_SOURCES_PATHS}
INCLUDEPATH	*= $${FRESH_SOURCES_PATHS}

RESOURCES	*= resources/fresh.qrc

TRANSLATIONS	*= translations/fresh-ru_RU.ts \
	translations/fresh-sl_SI.ts \
	translations/fresh-pl_PL.ts \
	translations/fresh-zh_CN.ts \
	translations/fresh-he_IL.ts \
	translations/fresh-da_DK.ts \
	translations/fresh-sk_SK.ts \
	translations/fresh-ja_JP.ts \
	translations/fresh-uk_UA.ts \
	translations/fresh-cs_CZ.ts \
	translations/fresh-ca_ES.ts \
	translations/fresh-ar_SA.ts \
	translations/fresh-fr_FR.ts \
	translations/fresh-zh_TW.ts \
	translations/fresh-es_ES.ts \
	translations/fresh-de_DE.ts \
	translations/fresh-pt_PT.ts \
	translations/fresh-sv_SE.ts

FORMS	*=  \
	src/gui/actionmanager/pActionsNodeShortcutEditor.ui \
	src/gui/pTranslationDialog.ui \
	src/gui/environmentvariablemanager/pEnvironmentVariableEditor.ui \
	src/gui/environmentvariablemanager/pEnvironmentVariablesEditor.ui \
	src/gui/gc-updatechecker/pUpdateCheckerDialog.ui

HEADERS	*=  \
	src/core/FreshExport.h \
	src/core/pCoreUtils.h \
	src/core/pGetOpt.h \
	src/core/pSettings.h \
	src/core/pTranslationManager.h \
	src/core/pVersion.h \
	src/gui/pColorButton.h \
	src/gui/pGuiUtils.h \
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
	src/gui/queuedmessage/pQueuedMessageWidget.h \
	src/gui/stylemanager/pStylesActionGroup.h \
	src/gui/stylemanager/pStylesToolButton.h \
	src/gui/pFileDialog.h \
	src/gui/pTreeComboBox.h \
	src/gui/consolemanager/pConsole.h \
	src/gui/consolemanager/pConsoleCommand.h \
	src/gui/dockwidget/pDockWidget.h \
	src/gui/dockwidget/pDockWidgetTitleBar.h \
	src/gui/environmentvariablemanager/pEnvironmentVariableEditor.h \
	src/gui/environmentvariablemanager/pEnvironmentVariablesEditor.h \
	src/gui/environmentvariablemanager/pEnvironmentVariablesManager.h \
	src/gui/environmentvariablemanager/pEnvironmentVariablesModel.h \
	src/gui/gc-updatechecker/pUpdateChecker.h \
	src/gui/gc-updatechecker/pUpdateCheckerDialog.h \
	src/gui/pPaypalButton.h \
	src/core/pNetworkAccessManager.h \
	src/core/Fresh.h

SOURCES	*=  \
	src/core/pCoreUtils.cpp \
	src/core/pGetOpt.cpp \
	src/core/pSettings.cpp \
	src/core/pTranslationManager.cpp \
	src/core/pVersion.cpp \
	src/gui/pColorButton.cpp \
	src/gui/pGuiUtils.cpp \
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
	src/gui/queuedmessage/pQueuedMessageWidget.cpp \
	src/gui/stylemanager/pStylesActionGroup.cpp \
	src/gui/stylemanager/pStylesToolButton.cpp \
	src/gui/pFileDialog.cpp \
	src/gui/pTreeComboBox.cpp \
	src/gui/consolemanager/pConsole.cpp \
	src/gui/consolemanager/pConsoleCommand.cpp \
	src/gui/dockwidget/pDockWidget.cpp \
	src/gui/dockwidget/pDockWidgetTitleBar.cpp \
	src/gui/environmentvariablemanager/pEnvironmentVariableEditor.cpp \
	src/gui/environmentvariablemanager/pEnvironmentVariablesEditor.cpp \
	src/gui/environmentvariablemanager/pEnvironmentVariablesManager.cpp \
	src/gui/environmentvariablemanager/pEnvironmentVariablesModel.cpp \
	src/gui/gc-updatechecker/pUpdateChecker.cpp \
	src/gui/gc-updatechecker/pUpdateCheckerDialog.cpp \
	src/gui/pPaypalButton.cpp \
	src/core/pNetworkAccessManager.cpp \
	src/core/Fresh.cpp

# include installs file
include( installs.pri )