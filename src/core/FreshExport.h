#ifndef FRESH_EXPORT_H
#define FRESH_EXPORT_H

/*!
    \file FreshExport.h
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include <qglobal.h>

/*! 
    \def FRESH_EXPORT
    \details This macro allow symbols to be exported/imported for Windows OS
*/

#if defined( FRESH_CORE_BUILD )
#define FRESH_EXPORT Q_DECL_EXPORT
#else
#define FRESH_EXPORT Q_DECL_IMPORT
#endif

#endif // FRESH_EXPORT_H
