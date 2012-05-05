/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsMenuBar.h
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
#ifndef PACTIONSMENUBAR_H
#define PACTIONSMENUBAR_H

/*!
    \file pActionsMenuBar.h
    \brief An extended QMenuBar having menu/actions exposed by a pActionsModel.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QMenuBar>

class pActionsModel;

/*!
    \ingroup FreshGui
    \class pActionsMenuBar
    \brief An extended QMenuBar having menu/actions exposed by a pActionsModel.
*/
class FRESH_EXPORT pActionsMenuBar : public QMenuBar
{
    Q_OBJECT
    
public:
    /*!
        Create a pActionsMenuBar having parent \a parent.
    */
    pActionsMenuBar( QWidget* parent = 0 );
    /*!
        Set the model associated with this menu bar.
    */
    void setModel( pActionsModel* model );
    /*!
        Return the model associated with this menu bar.
    */
    pActionsModel* model() const;
    
protected:
    pActionsModel* mModel;

protected slots:
    void model_actionInserted( QAction* action );
    void model_actionsCleared();
};

#endif // PACTIONSMENUBAR_H
