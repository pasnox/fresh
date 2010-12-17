/*!
    \file pVersion.h
    \brief This class is an helper for comparing string based application versions.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

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

class pVersion
{
%TypeHeaderCode
#include <../src/core/pVersion.h>
%End

public:
	/*!
		\brief Create a pVersion initialized with the string version \a version.
		\param version A QString representing the version number.
	*/
	pVersion( const QString& version );
	/*!
		\brief Return the string based version number.
		\return A QString containing the string version number.
	*/
	QString toString() const;
	/*!
		\brief Check if this version is equal to \a other.
		\param other The version to compare to.
		\return Return true if this version is equal to \a other else false.
	*/
	bool operator==( const pVersion& other ) const;
	/*!
		\brief Check if this version is different from \a other.
		\param other The version to compare to.
		\return Return true if this version is different from \a other else false.
	*/
	bool operator!=( const pVersion& other ) const;
	/*!
		\brief Check if this version is smaller than \a other.
		\param other The version to compare to.
		\return Return true if this version is smaller than \a other else false.
	*/
	bool operator<( const pVersion& other ) const;
	/*!
		\brief Check if this version is bigger than \a other.
		\param other The version to compare to.
		\return Return true if this version is bigger than \a other else false.
	*/
	bool operator>( const pVersion& other ) const;
	/*!
		\brief Check if this version is smaller or equal to \a other.
		\param other The version to compare to.
		\return Return true if this version is smaller or equal to \a other else false.
	*/
	bool operator<=( const pVersion& other ) const;
	/*!
		\brief Check if this version is bigger or equal to \a other.
		\param other The version to compare to.
		\return Return true if this version is bigger or equal to \a other else false.
	*/
	bool operator>=( const pVersion& other ) const;

};
