#ifndef PVERSION_H
#define PVERSION_H

/*!
    \file pVersion.h
    \brief This class is an helper for comparing string based application versions.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QString>

/*!
	\ingroup Core
    \class pVersion
    \brief This class is an helper for comparing string based application versions.

    The pVersion class help you to compare string based versions number like "1.5.0b", "1.6.0rc" etc.
	All kind of operators or available like <, >, !=, == etc.
	\code
	const pVersion v1( "1.5.0" );
	const pVersion v2( "1.5.1" );
	
	if ( v1 < v2 ) {
		qWarning( "v1 is smaller version than v2 !" );
	}
	else {
		qWarning( "v1 is bigger than v2 !" );
	}
	\endcode
*/

class FRESH_EXPORT pVersion
{
public:
	/*!
		Create a pVersion initialized with the string version \a version.
	*/
	pVersion( const QString& version );
	/*!
		Return the string based version number.
	*/
	QString toString() const;
	/*!
		return true if this version is equal to \a other else false.
	*/
	bool operator==( const pVersion& other ) const;
	/*!
		Return true if this version is different than \a other else false.
	*/
	bool operator!=( const pVersion& other ) const;
	/*!
		Return true if this version is smaller than \a other else false.
	*/
	bool operator<( const pVersion& other ) const;
	/*!
		Return true if this version is bigger than \a other else false.
	*/
	bool operator>( const pVersion& other ) const;
	/*!
		Return true if this version is smaller or equal to \a other else false.
	*/
	bool operator<=( const pVersion& other ) const;
	/*!
		Return true if this version is bigger or equal to \a other else false.
	*/
	bool operator>=( const pVersion& other ) const;

protected:
	QString mVersion;
	int major;
	int minor;
	int patch;
	int build;
	QString extra;
};

#endif // PVERSION_H
