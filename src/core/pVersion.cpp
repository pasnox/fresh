/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pVersion.cpp
** Date      : 2011-02-20T00:44:25
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "pVersion.h"

#include <QStringList>

pVersion::pVersion( const QString& version )
{
	mVersion = version.trimmed();
	const QStringList parts = mVersion.split( "." );
	const int index = parts.value( 3 ).indexOf( QRegExp( "[A-Z|a-z]" ) );

	major = parts.value( 0 ).toInt();
	minor = parts.value( 1 ).toInt();
	patch = parts.value( 2 ).toInt();

	if ( index != -1 ) {
		build = parts.value( 3 ).mid( 0, index ).toInt();
		extra = parts.value( 3 ).mid( index );
	}
	else {
		build = parts.value( 3 ).toInt();
	}
}

QString pVersion::toString() const
{
	return mVersion;
}

bool pVersion::operator==( const pVersion& other ) const
{
	return major == other.major
		&& minor == other.minor
		&& patch == other.patch
		&& build == other.build
		&& extra == other.extra;
}

bool pVersion::operator!=( const pVersion& other ) const
{
	return !operator==( other );
}

bool pVersion::operator<( const pVersion& other ) const
{
	if ( *this == other ) {
		return false;
	}
	else if ( major != other.major ) {
		return major < other.major;
	}
	else if ( minor != other.minor ) {
		return minor < other.minor;
	}
	else if ( patch != other.patch ) {
		return patch < other.patch;
	}
	else if ( build != other.build ) {
		return build < other.build;
	}
	else if ( extra.isEmpty() && !other.extra.isEmpty() ) {
		return false;
	}
	else if ( !extra.isEmpty() && other.extra.isEmpty() ) {
		return true;
	}
	
	return extra < other.extra; // not the best but afaik ;)
}

bool pVersion::operator>( const pVersion& other ) const
{
	if ( *this == other ) {
		return false;
	}
	else if ( major != other.major ) {
		return major > other.major;
	}
	else if ( minor != other.minor ) {
		return minor > other.minor;
	}
	else if ( patch != other.patch ) {
		return patch > other.patch;
	}
	else if ( build != other.build ) {
		return build > other.build;
	}
	else if ( extra.isEmpty() && !other.extra.isEmpty() ) {
		return true;
	}
	else if ( !extra.isEmpty() && other.extra.isEmpty() ) {
		return false;
	}

	return extra > other.extra; // not the best but afaik ;)
}

bool pVersion::operator<=( const pVersion& other ) const
{
	return operator<( other ) || operator==( other );
}

bool pVersion::operator>=( const pVersion& other ) const
{
	return operator>( other ) || operator==( other );
}
