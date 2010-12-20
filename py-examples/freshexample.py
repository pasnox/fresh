#!/usr/bin/env python
"""Test for Fresh framework.
Shows example of usage of some classes

It is only Fresh lib example, don't use it as example of good programming in
Python
"""


import sys

from PyQt4.QtCore import QObject, QProcess, qrand, QSize, QStringList, qsrand, \
                         Qt, QTime, QTimer, QUrl
from PyQt4.QtGui import qApp, QApplication, QAction, QActionGroup, \
                        QBoxLayout, QDialogButtonBox, QDockWidget, \
                        QFileDialog, \
                        QGridLayout, QIcon, QInputDialog, QKeySequence, \
                        QLineEdit, QMessageBox, \
                        QPlainTextEdit, QPushButton, QStyleFactory, \
                        QTabWidget, QTreeView, \
                        QWidget

from PyQt4.fresh import pActionsNode, pActionsNodeModel, \
                        pActionsNodeShortcutEditor, \
                        pColorButton, pConsole, pConsoleCommand, \
                        pDrawingUtils, pDockWidget, pDockToolBarManager, \
                        pFileDialog, \
                        pFileListEditor, \
                        pEnvironmentVariablesEditor, \
                        pEnvironmentVariablesManager, \
                        pEnvironmentVariablesModel, \
                        pIconManager, \
                        pMainWindow, pNetworkAccessManager, \
                        pPathListEditor, pPaypalButton, pSettings, \
                        pQueuedMessage, \
                        pStringListEditor, \
                        pStylesActionGroup, pStylesToolButton, pToolButton, \
                        pTreeComboBox, \
                        pUpdateChecker, \
                        pVersion

import freshresource

def main():
    """Function, executed on start
    """
    
    # create application
    app = QApplication (sys.argv)
    app.setApplicationName( "fresh-examples" )
    
    qsrand( QTime( 0, 0, 0 ).secsTo( QTime.currentTime() ) )
    
    pSettings.setDefaultProperties(pSettings.Properties(app.applicationName(), \
                                   "1.0.0",
                                   pSettings.Portable))

    window = MainWindow()
    window.setWindowTitle( app.applicationName() )
    window.show()

    # connection
    app.lastWindowClosed.connect(app.quit)

    # start application
    result = app.exec_()
    del window
    
    return result

class ConsoleCommands(pConsoleCommand):
    def __init__(self, mainWindow, console):
        pConsoleCommand.__init__(self, QStringList())
        self.mMainWindow = mainWindow
        
        self.setDescription( 
                "ls",
                pConsole.tr(console, 
                            "Execute 'dir' on windows else 'ls' command" ) )
        self.setDescription( "echo", 
                             pConsole.tr(console, "Simple echo command" ) )
        self.setDescription( "quit",
                             pConsole.tr(console, "Quit the application" ) )
        self.setDescription(
                "style", 
                pConsole.tr(console, "Change the application style" ) )
    
    def __del__(self):
        pConsoleCommand.__del(self)
    
    def interpret(self, command):
        """Interprets command
        Returns turple (output, exitCode)
        """
        print '~~~~~~~~~ interpret'
        ec = pConsoleCommand.NotFound
        output, ec = pConsoleCommand.interpret( command)
        parts = self.parseCommands( command )
        if parts:
            cmd = parts.takeFirst()
        else:
            cmd = ''
        
        if ec != pConsoleCommand.NotFound :            # nothing to do
            pass
        elif  cmd == "ls" :
            if sys.platform.startswith('win'):
                cmd = "dir %s" % \
                " ".join(pConsoleCommand.quotedStringList(parts)).trim()
            else:
                cmd = "dir %s" % " ".join( \
                    pConsoleCommand.quotedStringList(parts)).trimmed()
            
            process = QProcess()
            process.setProcessChannelMode( QProcess.MergedChannels )
            process.start( cmd )
            process.waitForStarted()
            process.waitForFinished()
            
            output = process.readAll().trimmed()
            ec = process.exitCode()
        elif  cmd == "echo" :
            if parts:
                output = "\n".join(parts)
                ec = pConsoleCommand.Error
            else:
                output = pConsole.tr(console, "No argument given" )
                ec = pConsoleCommand.Success
        elif  cmd == "quit":
            output = pConsole.tr(console, "Quitting the application..." )
            ec = pConsoleCommand.Success
            
            QTimer.singleShot(1000, qApp.quit() )
        elif  cmd == "style" :
            if  parts.count() != 1 :
                output = pConsole.tr(console, "%s take only 1 parameter, %d given"  %
                                        (cmd, len(parts)) )
                ec = pConsoleCommand.Error
            elif  parts[-1] == "list" :
                output = pConsole.tr(console, "Available styles:\n%s" % 
                                        '\n'.join(QStyleFactory.keys()) )
                ec = pConsoleCommand.Success
            else:
                styleExists = parts[-1].lower() in \
                            [key.lower() for key in QStyleFactory.keys()]
                if styleExists:
                    output = pConsole.tr(console, "Setting style to %s..." % parts[-1])
                    self.mMainWindow.setCurrentStyle( parts[-1] )
                    ec = pConsoleCommand.Success
                else:
                    output = pConsole.tr(console, "This style does not exists" )
                    ec = pConsoleCommand.Error
        
        return (output, ec)

    
    def usage(self, command ):
        parts = self.parseCommands( command )
        if parts:
            cmd = parts[0]
        else:
            cmd = ''
        
        if  cmd == "style" :
            usage = []
            
            usage.append( "Usage: %s [list | STYLE]"  % cmd )
            usage.append(self.description( cmd ))
            usage.append('')
            usage.append(QObject.tr( "%s list\t\tShow the available styles" % 
                                      cmd ))
            usage.append(QObject.tr( 
                    "%s STYLE\t\tChange the application style to STYLE" % cmd ))
            
            return '\n'.join(usage)

        return pConsoleCommand.usage( command )


class MainWindow(pMainWindow):
    def __init__(self, parentWidget=None):
        pMainWindow.__init__(self, parentWidget)
        self.createGui()

    def saveState(self):
        pMainWindow.saveState(self)
        
        self.settings().setValue( "MainWindow/Style",
                                  self.agStyles.currentStyle() )
        
        evm = pEnvironmentVariablesManager()
        evm.setVariables( self.eveVariables.variables() )
        evm.save()
        
        self.settings().setValue("UpdateChecker/Last Updated" ,
                                  self.ucMkS.lastUpdated())
        self.settings().setValue("UpdateChecker/Last Checked" ,
                                  self.ucMkS.lastChecked())

    def restoreState(self):
        pMainWindow.restoreState(self)
        
        self.agStyles.setCurrentStyle(
                        self.settings().value( "MainWindow/Style" ).toString() )
        
        variables = []
        evm = pEnvironmentVariablesManager()
        evm.load()
        variables = evm.variables()
        evm.mergeNewVariables( variables )
        
        self.eveVariables.setVariables( variables, True )
        
        self.ucMkS.setLastUpdated( 
            self.settings().value( "UpdateChecker/Last Updated" ).toDateTime() )
        self.ucMkS.setLastChecked( 
            self.settings().value( "UpdateChecker/Last Checked" ).toDateTime() )

    def createGui(self):
        pNetworkAccessManager.instance().setCacheDirectory(
            qApp.applicationDirPath().append( "/tmp" ) )
        
        self.twPages = QTabWidget( self )
        self.setCentralWidget( self.twPages )
        
        # initialize gui
        self.createMenuBar()
        self.createPlainTextEdit()
        self.createActionsTreeView()
        self.createConsole()
        self.createVersionsTests()
        self.createListEditors()
        self.createEnvironmentVariablesEditor()
        self.createCustomWidgets()
        self.createUpdateChecker()
        
        QTimer.singleShot( 3000, self.ucMkS.silentCheck )
        
        # create fake dock widget for testing
        '''for ( i = 0; i < 10; i++ )        dw = QDockWidget( self )
            dw.setObjectName( QString( "Dock%1" ).arg( i ) )
            dw.toggleViewAction().setObjectName( 
                        QString( "DockViewAction%1" ).arg( i ) )
            self.dockToolBar( Qt.LeftToolBarArea ).addDockWidget( 
                    dw, QString( "Dock %1" ).arg( i ), 
        QIcon( scaledPixmap( 
            ":/fresh/country-flags/ad.png", QSize( 96, 96 ) ) ) )
        }'''
        
        # list dock widgets in the menu
        for dockWidget in self.findChildren(QDockWidget):
            #dockWidget.setAttribute( Qt.WA_DeleteOnClose )
            action = dockWidget.toggleViewAction()
            self.menuBar().addAction( "mView/mDockWidgets/%s" % \
                        action.objectName() , action )
            node = self.mActionsModel.actionToNode( action )
            node.setDefaultShortcut( 
                QKeySequence( "Ctrl+%s" % chr( max( 32, qrand() % 127 ) ) ) )

    def createMenuBar(self):
        # set menu bar model
        self.mActionsModel = pActionsNodeModel( self )
        self.menuBar().setModel( self.mActionsModel )
        
        # create menus and sub menus
        self.menuBar().addMenu( "mFile" ).setText( self.tr( "&File" ) )
        self.menuBar().addMenu( "mEdit" ).setText( self.tr( "&Edit" ) )
        self.menuBar().addMenu( "mView" ).setText( self.tr( "&View" ) )
        self.menuBar().addMenu( "mView/mStyle" ).setText( self.tr( "&Style" ) )
        self.menuBar().addMenu( "mView/mMode" ).setText( self.tr( "&Mode" ) )
        self.menuBar().addMenu( "mView/mDockToolBarManager" ).setText(
                        self.tr( "&Dock ToolBar Manager" ) )
        self.menuBar().addMenu( "mView/mDockWidgets" ).setText(
                        self.tr( "Dock &Widgets" ) )
        
        # create actions
        aQuit = self.menuBar().addAction( "mFile/aQuit", self.tr( "&Quit" ) )
        
        aClassic = self.menuBar().addAction(
                        "mView/mMode/aShowClassic", self.tr( "Classic" ) )
        aClassic.setCheckable( True )
        
        aModern = self.menuBar().addAction(
                        "mView/mMode/aShowModern", self.tr( "Modern" ) )
        aModern.setCheckable( True )
        
        # style actions
        self.agStyles = pStylesActionGroup( self )
        self.agStyles.installInMenuBar( self.menuBar(), "mView/mStyle" )
        
        # action group
        agDockToolBarManagerMode = QActionGroup( self )
        agDockToolBarManagerMode.addAction( aClassic )
        agDockToolBarManagerMode.addAction( aModern )
        
        # add dock toolbar manager actions
        for dockToolBar in self.dockToolBarManager().dockToolBars():
            action = dockToolBar.toggleViewAction()
            self.menuBar().addAction(
                "mView/mDockToolBarManager/%s" % action.objectName() , action )
            
            action = dockToolBar.toggleExclusiveAction()
            self.menuBar().addAction(
                "mView/mDockToolBarManager/%s" % action.objectName(), action )
        
        # connections
        aQuit.triggered.connect(self.close)
        self.agStyles.styleSelected.connect(self.setCurrentStyle)
        self.dockToolBarManager().modeChanged.connect(
                            self.dockToolBarManagerModeChanged)
        aClassic.triggered.connect(self.dockToolBarManagerClassic)
        aModern.triggered.connect(self.dockToolBarManagerModern)

    def createPlainTextEdit(self):
        self.pteLog = QPlainTextEdit( self )
        self.pteLog.setReadOnly( True )
        self.pteLog.setTabStopWidth( 40 )
        self.twPages.addTab( self.pteLog, self.tr( "Log" ) )

    def createActionsTreeView(self):
        self.tvActions = QTreeView( self )
        self.tvActions.setModel( self.menuBar().model() )
        
        self.menuBar().addMenu( "mEdit/mActions" ).setText(
            self.tr( "&Actions" ) )
        
        aAddAction = self.menuBar().addAction( "mEdit/mActions/aAddAction",
                                           self.tr( "&Add action" ) )
        aRemoveAction = self.menuBar().addAction( "mEdit/mActions/aRemoveAction",
                                        self.tr( "&Remove selected action" ) )
        aEditTextNode = self.menuBar().addAction( "mEdit/mActions/aEditTextNode",
                                        self.tr( "&Edit selected node text" ) )
        aEditShortcuts = self.menuBar().addAction( "mEdit/mActions/aEditShortcuts",
                                    self.tr( "Edit the actions &shortcuts" ) )
        
        aAddAction.triggered.connect(self.aAddAction_triggered)
        aRemoveAction.triggered.connect(self.aRemoveAction_triggered)
        aEditTextNode.triggered.connect(self.aEditTextNode_triggered)
        aEditShortcuts.triggered.connect(self.aEditShortcuts_triggered)
        
        self.twPages.addTab( self.tvActions, self.tr( "Actions" ) )

    def createConsole(self):
        cShell = pConsole( "Shell:/> ", self )
        self.commands = ConsoleCommands( self, self )
        cShell.addAvailableCommand( self.commands )
        
        self.dwShell = pDockWidget( self )
        self.dwShell.setObjectName( "Shell" )
        self.dwShell.setWidget( cShell )
        self.dwShell.toggleViewAction().setObjectName( "ShellViewAction" )
        self.dockToolBar( Qt.BottomToolBarArea ).addDockWidget(
               self.dwShell, 
               self.tr( "Shell" ),
               QIcon(pDrawingUtils.scaledPixmap( ":/fresh/country-flags/ro.png",
                      QSize( 96, 96 ) ) ) )

    def createEnvironmentVariablesEditor(self):
        self.eveVariables = pEnvironmentVariablesEditor( self )
        
        self.dwEnvironmentVariablesEditor = pDockWidget( self )
        self.dwEnvironmentVariablesEditor.setObjectName(
                                        "EnvironmentVariablesEditor" )
        self.dwEnvironmentVariablesEditor.setWidget( self.eveVariables )
        self.dwEnvironmentVariablesEditor.toggleViewAction().setObjectName(
                                    "EnvironmentVariablesEditorViewAction" )
        self.dockToolBar( Qt.TopToolBarArea ).addDockWidget(
            self.dwEnvironmentVariablesEditor,
            self.tr( "Environment Variables Editor" ),
            QIcon( pDrawingUtils.scaledPixmap(":/fresh/country-flags/it.png",
                                              QSize( 96, 96 ) ) ) )

    def createVersionsTests(self):
        v1 = pVersion ( "1.5.4" )
        v2 = pVersion ( "1.5.4a" )
        
        self.pteLog.appendPlainText( 
            self.tr( "Testing versions %s & %s:\n" % 
                    (v1.toString(), v2.toString() ) ))
        
        # test 1
        test1 = ''
        for op in [">", "<", ">=", "<=", "==", "!="]:
            test1 += '%s\t%s\t%s: %s\n' % \
                (v1.toString(),
                op,
                v2.toString(),
                str(bool(eval("v1 %s v2" % op))))
            
        # test 2
        test2 = ''
        for op in [">", "<", ">=", "<=", "==", "!="]:
            test2 += '%s\t%s\t%s: %s\n' % \
                     (v1.toString(),
                     op,
                     v1.toString(),
                     str(bool(eval("v1 %s v1" % op))))
        
        self.pteLog.appendPlainText( self.tr( "Test 1:\n%s\n" % test1 ))
        self.pteLog.appendPlainText( self.tr( "Test 2:\n%s" % test2 ))

    def createListEditors(self):
        # list editor dock
        self.dwListEditor = pDockWidget( self )
        twListEditors = QTabWidget( self.dwListEditor )
        self.dwListEditor.setObjectName( "DockListEditor" )
        self.dwListEditor.setWidget( twListEditors )
        self.dwListEditor.toggleViewAction().setObjectName(
                "DockListEditorViewAction" )
        self.dockToolBar( Qt.RightToolBarArea ).addDockWidget(
              self.dwListEditor,
              self.tr( "List Editor" ),
              QIcon(pDrawingUtils.scaledPixmap( ":/fresh/country-flags/fr.png", 
                                                QSize( 96, 96 ))))
        
        twListEditors.addTab( 
              pStringListEditor( '', self ), self.tr( "Edit strings" ) )
        twListEditors.addTab(
              pPathListEditor( '', ".", self ), self.tr( "Edit paths" ) )
        twListEditors.addTab(
              pFileListEditor('', ".", "*.png", self ), self.tr( "Edit files" ))

    def createCustomWidgets(self):
        self.dwWidgets = pDockWidget( self )
        self.dwWidgets.setObjectName( "dwWidgets" )
        self.dwWidgets.toggleViewAction().setObjectName( "dwWidgetsViewAction" )
        self.dockToolBar( Qt.LeftToolBarArea ).addDockWidget(
              self.dwWidgets,
              self.tr( "Custom Widgets" ),
              QIcon( pDrawingUtils.scaledPixmap( ":/fresh/country-flags/es.png",
              QSize( 96, 96 ) ) ) )
        
        self.dwWidgetsContents = QWidget( self )
        self.dwWidgetsContentsLayout = QGridLayout( self.dwWidgetsContents )
        self.dwWidgets.setWidget( self.dwWidgetsContents )
        
        pbOpenFile = QPushButton( self.tr( "Get open file names" ), self )
        self.dwWidgetsContentsLayout.addWidget( pbOpenFile, 0, 0 )
        pbOpenFile.clicked.connect(self.openFileDialog)
        
        pbOpenDirectory = QPushButton( self.tr( "Get open directory name" ),
                                       self )
        self.dwWidgetsContentsLayout.addWidget( pbOpenDirectory, 1, 0 )
        pbOpenDirectory.clicked.connect(self.openDirectoryDialog)
        
        pbQueuedMessage = QPushButton( self.tr( "Add queued message" ) )
        self.dwWidgetsContentsLayout.addWidget( pbQueuedMessage, 2, 0 )
        pbQueuedMessage.clicked.connect(self.addQueuedMessage)
        
        stylesButton = pStylesToolButton( self.dwWidgetsContents )
        stylesButton.setSizePolicy( pbQueuedMessage.sizePolicy() )
        stylesButton.setCheckableActions( False )
        self.dwWidgetsContentsLayout.addWidget( stylesButton, 3, 0 )
        stylesButton.styleSelected.connect(self.agStyles.setCurrentStyle)
        
        tcbActions = pTreeComboBox( self )
        tcbActions.setModel( self.mActionsModel )
        self.dwWidgetsContentsLayout.addWidget( tcbActions, 4, 0 )
        
        paypal = pPaypalButton( self )
        paypal.setBusinessId( "5R924WYXJ6BAW" )
        paypal.setItemName( "QWBFS Manager" )
        paypal.setItemId( "QWBFS-DONATION" )
        paypal.setCurrencyCode( "EUR" )
        self.dwWidgetsContentsLayout.addWidget( paypal, 5, 0 )
        
        toolButton1 = pToolButton( self.dwWidgetsContents )
        toolButton1.setToolButtonStyle( Qt.ToolButtonTextBesideIcon )
        toolButton1.setText( self.tr( "Bottom To Top" ) )
        toolButton1.setIcon( pIconManager.icon( "pt.png") )
        toolButton1.setDirection( QBoxLayout.BottomToTop )
        self.dwWidgetsContentsLayout.addWidget( toolButton1, 0, 1, 6, 1 )
        
        toolButton2 = pToolButton( self.dwWidgetsContents )
        toolButton2.setToolButtonStyle( Qt.ToolButtonTextBesideIcon )
        toolButton2.setText( self.tr( "Top To Bottom" ) )
        toolButton2.setIcon( pIconManager.icon( "br.png" ) )
        toolButton2.setDirection( QBoxLayout.TopToBottom )
        self.dwWidgetsContentsLayout.addWidget( toolButton2, 0, 2, 6, 1 )
        
        colorButton = pColorButton( self.dwWidgetsContents )
        self.dwWidgetsContentsLayout.addWidget( colorButton, 6, 0 )

    def createUpdateChecker(self):
        self.ucMkS = pUpdateChecker( self )
        self.ucMkS.setDownloadsFeedUrl( 
            QUrl("http:#code.google.com/feeds/p/monkeystudio/downloads/basic" ))
        self.ucMkS.setVersion( "1.6.0.0" )
        self.ucMkS.setVersionString( "1.6.0.0" )
        self.ucMkS.setVersionDiscoveryPattern( ".*mks_([0-9\\.]+).*" )
        
        self.menuBar().addMenu( "mHelp" ).setText( self.tr( "&Help" ) )
        self.menuBar().addAction( "mHelp/aUpdateChecker", self.ucMkS.menuAction() )

    def aAddAction_triggered(self):
        index = self.tvActions.selectionModel().selectedIndexes()[0]
        path = ''
        
        if index.isValid():
            node = self.mActionsModel.indexToNode( index )
            
            if node.type() == pActionsNode.Action:
                path = node.path().split('/')[:-2]
            else:
                path = node.path()
        
        if path:
            path += '/'
        
        path = QInputDialog.getText( 
                self,
                '',
                self.tr( "Enter the full path where to add " +
                         "the action (/some/path/to/add/the/actionName):" ),
                QLineEdit.Normal,
                path )
        
        if not "/" in path or path.replace("/", '').trim().isEmpty():
            return
        
        action = QAction( self )
        action.setText(path.split('/')[-1])
        
        if  not self.mActionsModel.addAction( path, action ) :
            del action
            QMessageBox.information(
                                self,
                                '',
                                self.tr( "Can't add action to '%s' % path" ))

    def aRemoveAction_triggered(self):
        index = self.tvActions.selectionModel().selectedIndexes()[0]
        
        if  index.isValid():
            node = self.mActionsModel.indexToNode( index )
            
            if not self.mActionsModel.removeAction( node.path() ):
                QMessageBox.information( self,
                                         '',
                                         self.tr( "Can't remove action '%s'" % 
                                                  node.path() ) )

    def aEditTextNode_triggered(self):
        index = self.tvActions.selectionModel().selectedIndexes()[0]
        
        if index.isValid():
            node = self.mActionsModel.indexToNode( index )
            text = QInputDialog.getText( self,
                                         '',
                                         self.tr( "Enter the node text:" ),
                                         QLineEdit.Normal,
                                         node.text())
            
            if text:
                node.setText( text )

    def aEditShortcuts_triggered(self):
        dlg = pActionsNodeShortcutEditor ( self.mActionsModel, self )
        dlg.exec_()

    def dockToolBarManagerModeChanged(self, mode ):
        if mode == pDockToolBarManager.Classic:
            self.menuBar().action( "mView/mMode/aShowClassic" ).trigger()
        elif mode == pDockToolBarManager.Modern:
            self.menuBar().action( "mView/mMode/aShowModern" ).trigger()
        else:
            assert(0)

    def dockToolBarManagerClassic(self):
        self.dockToolBarManager().setMode( pDockToolBarManager.Classic )

    def dockToolBarManagerModern(self):
        self.dockToolBarManager().setMode( pDockToolBarManager.Modern )

    def addQueuedMessage(self):
        ok, message = QInputDialog.getText( 
                                    self,
                                    self.tr( "Add a queued message" ),
                                    self.tr( "Enter the message to show:" ),
                                    QLineEdit.Normal,
                                    self.tr( "This is the default message" ))
        
        if  ok and message:
            msg = pQueuedMessage()
            msg.message = message
            msg.buttons[ QDialogButtonBox.Ok ] = ''
            msg.buttons[ QDialogButtonBox.Yes ] = self.tr( "Show QMessageBox" )
            msg.object = self
            msg.slot = "queuedMessageToolBarButtonClicked"
            
            queuedMessageToolBar().appendMessage( msg )

    def queuedMessageToolBarButtonClicked(self, button, message ):
        if button == QDialogButtonBox.Yes:
            QMessageBox.information( self, '', message.message )

    def setCurrentStyle(self, style ):
        QApplication.setStyle( style )

    def openFileDialog(self):
        caption = ''
        dir = ''
        filter = ''
        enabledTextCodec = True
        enabledOpenReadOnly = True
        selectedFilter = ''
        options = 0
        result = pFileDialog.getOpenFileNames(self, caption, dir,
                                              filter, enabledTextCodec,
                                              enabledOpenReadOnly,
                                              selectedFilter, options)
        
        self.pteLog.appendPlainText( '' )
        
        if result.isEmpty():
            self.pteLog.appendPlainText(
                    self.tr( "You canceled the open file dialog" ) )
        else:
            texts = []
            
            texts.append(self.tr( "You accepted the open file dialog" ))
            
            for type in result.keys():
                if type == pFileDialog.TextCodec:
                    texts.append(
                      self.tr( "TextCodec: %s" % result[ type ].toString() ))
                elif type == pFileDialog.OpenReadOnly:
                    texts.append(
                      self.tr( "OpenReadOnly: %s" % result[ type ].toString() ))
                elif type == pFileDialog.Directory:
                    texts.append(
                      self.tr( "Directory: %s" % result[ type ].toString() ))
                elif type == pFileDialog.FileName:
                    texts.append(
                      self.tr( "FileName: %s" % result[ type ].toString() ))
                elif type == pFileDialog.FileNames:
                    texts.append(
                      self.tr( "FileNames: %s" % 
                                ", ".join(result[ type ].toStringList())))
                elif type == pFileDialog.SelectedFilter:
                    texts.append(
                    self.tr( "SelectedFilter: %s" % result[ type ].toString() ))
            
            self.pteLog.appendPlainText( '\n'.join(texts))

    def openDirectoryDialog(self):
        caption = ''
        dir = ''
        filter = ''
        enabledTextCodec = True
        enabledOpenReadOnly = True
        selectedFilter = ''
        options = 0
        result = pFileDialog.getExistingDirectory( 
                                        self, caption, dir, 
                                        enabledTextCodec, 
                                        enabledOpenReadOnly,
                                        options | QFileDialog.ShowDirsOnly )
        
        self.pteLog.appendPlainText( '' )
        
        if not result:
            self.pteLog.appendPlainText(
                        self.tr( "You canceled the open directory dialog" ) )
        else:
            texts = []
            
            texts.append(self.tr( "You accepted the open directory dialog" ))
            
            for type in result.keys():
                if type == pFileDialog.TextCodec:
                    texts.append(
                        self.tr( "TextCodec: %s" % result[ type ].toString() ))
                elif type == pFileDialog.OpenReadOnly:
                    texts.append(
                        self.tr("OpenReadOnly: %s" % result[ type ].toString()))
                elif type == pFileDialog.Directory:
                    texts.append(
                        self.tr( "Directory: %s" % result[ type ].toString() ))
                elif type == pFileDialog.FileName:
                    texts.append(
                        self.tr( "FileName: %s" % result[ type ].toString() ))
                elif type == pFileDialog.FileNames:
                    texts.append(
                        self.tr( "FileNames: %s" % 
                                  ", ".join(result[ type ].toStringList())))
                elif type == pFileDialog.SelectedFilter:
                    texts.append(self.tr( "SelectedFilter: %s" % 
                                 result[ type ].toString() ))
            
            self.pteLog.appendPlainText( '\n'.join(texts))


if __name__ == '__main__':
    main()
