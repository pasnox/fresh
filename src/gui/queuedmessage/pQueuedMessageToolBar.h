/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pQueuedMessageToolBar.h
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
#ifndef PQUEUEDMESSAGETOOLBAR_H
#define PQUEUEDMESSAGETOOLBAR_H

/*!
	\file pQueuedMessageToolBar.h
	\brief A toolbar that handle a pQueuedMessageWidget mimicing the chromium yellow toolbar.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pQueuedMessageWidget.h"

#include <QToolBar>

/*!
	\ingroup FreshGui
	\class pQueuedMessageToolBar
	\brief A toolbar that handle a pQueuedMessageWidget mimicing the chromium yellow toolbar.
*/
class FRESH_EXPORT pQueuedMessageToolBar : public QToolBar
{
	Q_OBJECT
	
public:
	/*!
		Create an instance of message toolbar having parent \a parent.
	*/
	pQueuedMessageToolBar( QWidget* parent = 0 );
	/*!
		Return a pointer to the internal pQueuedMessageWidget widget.
	*/
	pQueuedMessageWidget* queuedMessageWidget() const;

protected:
	pQueuedMessageWidget* mQueuedWidget;
	
	/*!
		Reimplemented.
	*/
	void changeEvent( QEvent* event );
	/*!
		Reimplemented.
	*/
	void paintEvent( QPaintEvent* event );

public slots:
	/*!
		\sa pQueuedMessageWidget::append( const QString&, int )
	*/
	pQueuedMessage appendMessage( const QString& message, int milliSeconds = -1 );
	/*!
		\sa pQueuedMessageWidget::append( const pQueuedMessage& )
	*/
	void appendMessage( const pQueuedMessage& message );
	/*!
		\sa pQueuedMessageWidget::remove( const pQueuedMessage& )
	*/
	void removeMessage( const pQueuedMessage& message );

protected slots:
	void messageShown( const pQueuedMessage& message );
	void messageFinished();
};

#endif // PQUEUEDMESSAGETOOLBAR_H
