#include <QApplication>
#include <QDebug>

#include <Core/pSettings>
#include <Gui/pActionsNode>

#include "MainWindow.h"

int main( int argc, char** argv )
{
	// create application
	QApplication app( argc, argv );
	app.setApplicationName( "fresh-examples" );
	
	qsrand( QTime( 0, 0, 0 ).secsTo( QTime::currentTime() ) );
	
	pSettings::setDefaultProperties( pSettings::Properties( app.applicationName(), "1.0.0", pSettings::Portable ) );

	MainWindow w;
	w.setWindowTitle( app.applicationName() );
	w.show();

	// connection
	QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );

	// start application
	const int result = app.exec();
	
	qWarning() << "Created" << pActionsNode::created;
	qWarning() << "Deleted" << pActionsNode::deleted;
	
	qWarning() << "Created1" << pActionsNode::Data::created1;
	qWarning() << "Deleted1" << pActionsNode::Data::deleted1;
	
	return result;
}
