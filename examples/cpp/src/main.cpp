#include <QApplication>
#include <QDebug>

#include <FreshCore/pSettings>
#include <FreshCore/pNetworkAccessManager>

#include "MainWindow.h"

int main( int argc, char** argv )
{
    // create application
    QApplication app( argc, argv );
    app.setApplicationName( "fresh-examples" );

    qsrand( QTime( 0, 0, 0 ).secsTo( QTime::currentTime() ) );

    pSettings::setDefaultProperties( pSettings::Properties( app.applicationName(), "1.0.0", pSettings::Auto ) );
    pNetworkAccessManager::instance()->setCacheDirectory( app.applicationDirPath().append( "/tmp" ) );

    MainWindow w;
    w.setWindowTitle( app.applicationName() );
    w.show();

    // connection
    QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

    // start application
    const int result = app.exec();

    return result;
}
