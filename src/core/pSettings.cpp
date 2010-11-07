#include "pSettings.h"

#include <QFile>
#include <QDir>
#include <QCoreApplication>

#if defined( QT_GUI_LIB )
#include <QDesktopServices>
#include <QMainWindow>
#endif

pSettings::Properties pSettings::mDefaultProperties = pSettings::Properties( QString::null, QString::null, pSettings::Invalid );

pSettings::Properties::Properties( const pSettings::Properties& other )
{
	operator=( other );
}

pSettings::Properties::Properties( const QString& _name, const QString& _version, pSettings::Type _type )
{
	name = _name;
	version = _version;
	type = _type;
}

QString pSettings::Properties::storageLocation() const
{
#if defined( QT_GUI_LIB )
	return QDesktopServices::storageLocation( QDesktopServices::DataLocation );
#else
	#warning May need a better way for major platforms
	return QDir::homePath().append( QString( ".%1" ).arg( name ) );
#endif
}

QString pSettings::Properties::settingsFilePath() const
{
	QString fn;
	
	switch ( type ) {
		case pSettings::Invalid:
			Q_ASSERT( 0 );
			qFatal( qPrintable( QString( "%1: Invalid call" ).arg( Q_FUNC_INFO ) ) );
			break;
		case pSettings::Normal:
			fn = QString( "%1/%2-%3.ini" )
				.arg( storageLocation() )
				.arg( name )
				.arg( version );
			break;
		case pSettings::Portable:
#ifdef Q_OS_MAC
			fn = QString( "%1/../%2-%3.ini" )
				.arg( QCoreApplication::applicationDirPath() )
				.arg( name )
				.arg( version );
#else
			fn = QString( "%1/%2-%3.ini" )
				.arg( QCoreApplication::applicationDirPath() )
				.arg( name )
				.arg( version );
#endif
			break;
	}
	
	return QDir::cleanPath( fn );
}

QString pSettings::Properties::settingsFilePath( const QString& name, const QString& version ) const
{
	return QDir::cleanPath( QString( "%1/%2-%3.ini" )
		.arg( QFileInfo( settingsFilePath() ).absolutePath() )
		.arg( name )
		.arg( version ) );
}

/*!
	\details Construct a new pSettings
	\param parent The object parent
	\param properties The properties used
*/
pSettings::pSettings( QObject* parent, const pSettings::Properties& properties )
	: QSettings( properties.settingsFilePath(), QSettings::IniFormat, parent )
{
	mProperties = properties;
}

/*!
	\details Construct a new pSettings
	\param parent The object parent
	\param name The settings name
	\param version The settings version.
	\param properties The properties used.
*/
pSettings::pSettings( QObject* parent, const QString& name, const QString& version, const pSettings::Properties& properties )
	: QSettings( properties.settingsFilePath( name, version ), QSettings::IniFormat, parent )
{
	mProperties.name = name;
	mProperties.version = version;
	mProperties.type = properties.type;
}

/*!
	\details Return the properties
*/
pSettings::Properties pSettings::properties() const
{
	return mProperties;
}

#if defined( QT_GUI_LIB )
/*!
	\details Restore a main winow state
	\param window The main window to restore
*/
void pSettings::restoreState( QMainWindow* window )
{
	if ( window ) {
		//window->restoreGeometry( value( "MainWindow/Geometry" ).toByteArray() );
		window->setGeometry( value( "MainWindow/Geometry", window->geometry() ).toRect() );
		window->restoreState( value( "MainWindow/State" ).toByteArray() );
		
		//if ( value( "MainWindow/Geometry" ).toByteArray().isEmpty() ) {
		if ( value( "MainWindow/Geometry" ).toRect().isNull() ) {
			window->showMaximized();
		}
	}
}

/*!
	\details Save a main winow state
	\param window The main window to save
*/
void pSettings::saveState( QMainWindow* window )
{
	if ( window ) {
		//setValue( "MainWindow/Geometry", window->saveGeometry() );
		setValue( "MainWindow/Geometry", window->geometry() );
		setValue( "MainWindow/State", window->saveState() );
	}
}
#endif

/*!
	\details A virtual member that you can reimplement for creating a default settings member
*/
void pSettings::setDefaultSettings()
{
}

/*!
	\details Initialize the default settings name and version
	\param name The settings name
	\param version The settings version
*/
void pSettings::setDefaultProperties( const pSettings::Properties& properties )
{
	mDefaultProperties = properties;
}

/*!
	\details Return the default properties
*/
pSettings::Properties pSettings::defaultProperties()
{
	return mDefaultProperties;
}
