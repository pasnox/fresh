/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pFileListEditor.h
** Date      : 2011-02-20T00:41:09
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
#ifndef PFILELISTEDITOR_H
#define PFILELISTEDITOR_H

/*!
    \file pFileListEditor.h
    \brief a pStringListEditor that handle files name.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pStringListEditor.h"

/*!
    \ingroup FreshGui
    \class pFileListEditor
    \brief a pStringListEditor that handle files name.
*/
class FRESH_EXPORT pFileListEditor : public pStringListEditor
{
    Q_OBJECT
    
public:
    /*!
        Create an editor instance having parent \a parent.
    */
    pFileListEditor( QWidget* parent = 0 );
    /*!
        Create an editor instance having parent \a parent and title \a title.
        The path and filter for the file dialog will be \a path and \a filter.
    */
    pFileListEditor( const QString& title, const QString& path, const QString& filter, QWidget* parent = 0 );
    
    /*!
        Return the path used in files dialogs.
    */
    QString path() const;
    /*!
        Set the path used in files dialogs to \a path.
    */
    void setPath( const QString& path );
    /*!
        Return the filter applied in files dialogs.
    */
    QString filter() const;
    /*!
        Set the filter used in files dialogs.
    */
    void setFilter( const QString& filter );
    
protected:
    QString mPath;
    QString mFilter;
    
    virtual void init();
    
protected slots:
    virtual void onAddItem();
    virtual void onEditItem();
};

#endif // PFILELISTEDITOR_H
