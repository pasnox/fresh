# fresh lib project file

# include qmake-extensions file
include( qmake-extensions.git/qmake-extensions.pri )

isEmpty( build_mode ):FRESH_BUILD_MODE = release
else:FRESH_BUILD_MODE = $${build_mode}

isEmpty( build_type ):FRESH_BUILD_TYPE = static
else:FRESH_BUILD_TYPE = $${build_type}

FRESH_BUILD_PATH = build
FRESH_DESTDIR = build

TEMPLATE = lib
CONFIG -= release debug debug_and_release warn_on warn_off ppc ppc64 x86 x86_64
CONFIG *= warn_on thread x11 windows qt $${FRESH_BUILD_TYPE} $${FRESH_BUILD_MODE}
QT *= network xml
greaterThan( QT_MAJOR_VERSION, 4 ): QT *= widgets

# Mac universal build from 10.3 & up
macx:universal {
    SDK_PATH = $$(MAC_SDKS_PATH)
    isEmpty( SDK_PATH ):SDK_PATH = /Developer/SDKs
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.3
    QMAKE_MAC_SDK = $${SDK_PATH}/MacOSX10.6.sdk
    CONFIG *= app_bundle
    #CONFIG *= x86_64
    CONFIG *= x86
    CONFIG *= ppc
    # this link is required for building the ppc port to avoid the undefined __Unwind_Resume symbol
    CONFIG( ppc ):LIBS *= -lgcc_eh
}

macx {
    LIBS *= "-framework CoreFoundation"
}

setTarget( fresh )

setTemporaryDirectories( $${FRESH_BUILD_PATH} )
isEqual( FRESH_BUILD_MODE, debug ):CONFIG *= console

DESTDIR = $${FRESH_DESTDIR}

isEqual( FRESH_BUILD_TYPE, shared ) {
    win32:DLLDESTDIR = $${FRESH_DESTDIR}
}

include( defines.pri )

isShadowBuild() {
    FRESH_SOURCES_ROOT_PATH = "$${PWD}/src"
} else {
    FRESH_SOURCES_ROOT_PATH = src
}

FRESH_SOURCES_PATHS = $$getFolders( "$${FRESH_SOURCES_ROOT_PATH}" )
DEPENDPATH *= $${FRESH_SOURCES_PATHS}
INCLUDEPATH *= $${FRESH_SOURCES_PATHS}
RESOURCES *= resources/fresh.qrc

XUP.TRANSLATIONS_BASENAME = fresh
XUP.TRANSLATIONS_DIRECTORY = translations

TRANSLATIONS *= \
    translations/fresh_ru_RU.ts \
    translations/fresh_sl_SI.ts \
    translations/fresh_pl_PL.ts \
    translations/fresh_zh_CN.ts \
    translations/fresh_he_IL.ts \
    translations/fresh_da_DK.ts \
    translations/fresh_sk_SK.ts \
    translations/fresh_ja_JP.ts \
    translations/fresh_uk_UA.ts \
    translations/fresh_cs_CZ.ts \
    translations/fresh_ca_ES.ts \
    translations/fresh_ar_SA.ts \
    translations/fresh_fr_FR.ts \
    translations/fresh_zh_TW.ts \
    translations/fresh_es_ES.ts \
    translations/fresh_de_DE.ts \
    translations/fresh_pt_PT.ts \
    translations/fresh_sv_SE.ts

FORMS *=  \
    src/gui/actionmanager/pActionsShortcutEditor.ui \
    src/gui/pTranslationDialog.ui \
    src/gui/environmentvariablemanager/pEnvironmentVariableEditor.ui \
    src/gui/environmentvariablemanager/pEnvironmentVariablesEditor.ui \
    src/gui/gc-updatechecker/pUpdateCheckerDialog.ui

CORE_HEADERS =  \
    src/core/FreshExport.h \
    src/core/pCoreUtils.h \
    src/core/pGetOpt.h \
    src/core/pSettings.h \
    src/core/pTranslationManager.h \
    src/core/pVersion.h \
    src/core/pNetworkAccessManager.h \
    src/core/pGenericTableModel.h \
    src/core/pLocaleModel.h \
    src/core/Fresh.h

GUI_HEADERS *=  \
    src/gui/pColorButton.h \
    src/gui/pFontButton.h \
    src/gui/pGuiUtils.h \
    src/gui/pIconManager.h \
    src/gui/pKeySequenceEdit.h \
    src/gui/pLineEdit.h \
    src/gui/pMainWindow.h \
    src/gui/pToolButton.h \
    src/gui/actionmanager/pActionsMenuBar.h \
    src/gui/actionmanager/pActionsModel.h \
    src/gui/actionmanager/pActionsShortcutEditor.h \
    src/gui/dockmanager/pDockToolBar.h \
    src/gui/dockmanager/pDockToolBarManager.h \
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
    src/gui/pComboBox.h \
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
    src/gui/pCheckComboBox.h

HEADERS *=  \
    $${CORE_HEADERS} \
    $${GUI_HEADERS}

CORE_SOURCES *=  \
    src/core/pCoreUtils.cpp \
    src/core/pGetOpt.cpp \
    src/core/pSettings.cpp \
    src/core/pTranslationManager.cpp \
    src/core/pVersion.cpp \
    src/core/pNetworkAccessManager.cpp \
    src/core/pGenericTableModel.cpp \
    src/core/pLocaleModel.cpp \
    src/core/Fresh.cpp

GUI_SOURCES *=  \
    src/gui/pColorButton.cpp \
    src/gui/pFontButton.cpp \
    src/gui/pGuiUtils.cpp \
    src/gui/pIconManager.cpp \
    src/gui/pKeySequenceEdit.cpp \
    src/gui/pLineEdit.cpp \
    src/gui/pMainWindow.cpp \
    src/gui/pToolButton.cpp \
    src/gui/actionmanager/pActionsMenuBar.cpp \
    src/gui/actionmanager/pActionsModel.cpp \
    src/gui/actionmanager/pActionsShortcutEditor.cpp \
    src/gui/dockmanager/pDockToolBar.cpp \
    src/gui/dockmanager/pDockToolBarManager.cpp \
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
    src/gui/pComboBox.cpp \
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
    src/gui/pCheckComboBox.cpp

SOURCES *=  \
    $${CORE_SOURCES} \
    $${GUI_SOURCES}

win32 {
    HEADERS *= src/core/pWinHelpers.h
}

macx {
    HEADERS *= src/core/pMacHelpers.h
    OBJECTIVE_SOURCES *= src/core/pMacHelpers.mm
}

# include installs file
include( installs.pri )
