#ifndef PSETTINGS_H
#define PSETTINGS_H

/*!
	\file pSettings.h
	\date 2008-01-14T00:27:38
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A class that handle ini files
*/

#include "FreshExport.h"

#include <QSettings>

#if defined( QT_GUI_LIB )
class QMainWindow;
#endif

/*!
	\brief A class that handle ini files
	\details This class is used for storing/retreiving data using ini files
*/
class FRESH_EXPORT pSettings : public QSettings
{
	Q_OBJECT

public:
	enum Type {
		Invalid = -1,
		Normal,
		Portable
	};
	
	struct FRESH_EXPORT Properties
	{
		Properties( const pSettings::Properties& other = pSettings::defaultProperties() );
		Properties( const QString& _name, const QString& _version, pSettings::Type _type );
		
		QString storageLocation() const;
		QString settingsFilePath() const;
		QString settingsFilePath( const QString& name, const QString& version ) const;
		
		QString name;
		QString version;
		pSettings::Type type;
	};
	
	pSettings( QObject* parent = 0, const pSettings::Properties& properties = defaultProperties() );
	pSettings( QObject* parent, const QString& name, const QString& version, const pSettings::Properties& properties = defaultProperties() );
	
	pSettings::Properties properties() const;

#if defined( QT_GUI_LIB )
	virtual void restoreState( QMainWindow* window );
	virtual void saveState( QMainWindow* );
#endif
	virtual void setDefaultSettings();
	
	static void setDefaultProperties( const pSettings::Properties& properties );
	static pSettings::Properties defaultProperties();

protected:
	static pSettings::Properties mDefaultProperties;
	pSettings::Properties mProperties;
};

#endif // PSETTINGS_H
