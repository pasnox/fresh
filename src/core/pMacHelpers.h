/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pMacHelpers.h
** Date      : 2011-02-20T00:44:21
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
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
#ifndef PMACHELPERS_H
#define PMACHELPERS_H

/*!
    \file pMacHelpers.h
    \brief A central place for mac os x helpers.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QVariant>

#import <CoreFoundation/CFDictionary.h>
#import <CoreFoundation/CFArray.h>
#import <CoreFoundation/CFString.h>
#import <CoreFoundation/CFNumber.h>
#import <CoreFoundation/CFData.h>
#import <CoreFoundation/CFURL.h>
#import <CoreFoundation/CFUUID.h>
#import <CoreFoundation/CFBundle.h>
#import <CoreFoundation/CFBase.h>

/*!
    \ingroup FreshCore
    \namespace pMacHelpers
    \brief A central place for mac os x helpers.
    \note This namespace is available on Mac OS X only.
*/
namespace pMacHelpers
{
    /*!
        Return a QVariantMap from \a dict.
    */
    FRESH_EXPORT QVariantMap toQVariantMap( CFDictionaryRef dict );
    /*!
        Return a QVariantList from \a array.
    */
    FRESH_EXPORT QVariantList toQVariantList( CFArrayRef array );
    /*!
        Return a QVariant from \a string.
    */
    FRESH_EXPORT QVariant toQVariant( CFStringRef string );
    /*!
        Return a QVariant from \a value.
    */
    FRESH_EXPORT QVariant toQVariant( CFBooleanRef value );
    /*!
        Return a QVariant from \a number.
    */
    FRESH_EXPORT QVariant toQVariant( CFNumberRef number );
    /*!
        Return a QVariant from \a data.
    */
    FRESH_EXPORT QVariant toQVariant( CFDataRef data );
    /*!
        Return a QVariant from \a url.
    */
    FRESH_EXPORT QVariant toQVariant( CFURLRef url );
    /*!
        Return a QVariant from \a uuid.
    */
    FRESH_EXPORT QVariant toQVariant( CFUUIDRef uuid );
    /*!
        Return a QVariant from \a bundle.
    */
    FRESH_EXPORT QVariant toQVariant( CFBundleRef bundle );
    /*!
        Return a QVariant from CFType \a ref.
    */
    FRESH_EXPORT QVariant toQVariant( CFTypeRef ref );
};

#endif // MACHELPERS_H
