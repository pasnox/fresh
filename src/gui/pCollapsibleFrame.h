/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pCollapsibleFrame.h
** Date      : 2011-02-20T00:41:35
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
#ifndef PCOLLAPSIBLEFRAME_H
#define PCOLLAPSIBLEFRAME_H

/*!
    \file pCollapsibleFrame.h
    \brief A QFrame that can collapse its internal widget.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QFrame>

class QIcon;

/*!
    \ingroup FreshGui
    \class pCollapsibleFrame
    \brief A QFrame that can collapse its internal widget.
*/
class FRESH_EXPORT pCollapsibleFrame : public QFrame
{
    Q_OBJECT
    friend class pCollapsibleFramePrivate;
    class pCollapsibleFramePrivate* d;

public:
    /*!
        Create an instance of pCollapsibleFrame having parent \a parent.
    */
    pCollapsibleFrame( QWidget* parent = 0 );
    /*!
        Reimplemented.
    */
    virtual QSize minimumSizeHint() const;
    /*!
        Return the widget managed by the collapsible frame.
    */
    QWidget* widget() const;
    /*!
        Remove the managed widget from this collapsible frame and return it.
        The widget is reparented with parent to \a 0
    */
    QWidget* takeWidget();
    /*!
        Set the managed widget to \a widget.
    */
    void setWidget( QWidget* widget );
    /*!
        Return the title icon used then the frame is collapsed.
    */
    QIcon collapsedIcon() const;
    /*!
        Return the title icon when the frame is expanded.
    */
    QIcon expandedIcon() const;
    /*!
        Return the title text when the frame is collapsed.
    */
    QString collapsedText() const;
    /*!
        Return the title text when the frame is expanded.
    */
    QString expandedText() const;
    /*!
        Return true if the frame is collapsed else false.
    */
    bool isCollapsed() const;
    /*!
        Return true if the frame is expanded else false.
    */
    bool isExpanded() const;
    /*!
        Return the indentation for the managed widget compared to its title.
        The default is 0.
    */
    int indentation() const;

public slots:
    /*!
        Set the title icon when the frame is collapsed to \a icon.
    */
    void setCollapsedIcon( const QIcon& icon );
    /*!
        Set the title icon when the frame is expanded to \a icon.
    */
    void setExpandedIcon( const QIcon& icon );
    /*!
        Set the title icon to \a icon for both expanded and collapsed state.
    */
    void setIcon( const QIcon& icon );
    /*!
        Set the title text when the frame is collapsed to \a text.
    */
    void setCollapsedText( const QString& text );
    /*!
        Set the title text when the frame is expanded to \a text.
    */
    void setExpandedText( const QString& text );
    /*!
        Set the title text to \a text for both expanded and collapsed state.
    */
    void setText( const QString& text );
    /*!
        Set the frame to be collapsed according to \a collapsed.
    */
    void setCollapsed( bool collapsed );
    /*!
        Set the frame to be expanded according to \a collapsed.
    */
    void setExpanded( bool expanded );
    /*!
        Set the managed widget indentation to \a indentation.
    */
    void setIndentation( int indentation );

protected:
    /*!
        Reimplemented.
    */
    virtual void childEvent( QChildEvent * event );

signals:
    /*!
        This signal is emited just before the frame collapse.
    */
    void aboutToCollapse();
    /*!
        This signal is emited just after the frame has collapsed.
    */
    void collapsed();
    /*!
        This signal is emited just before the frame expand.
    */
    void aboutToExpand();
    /*!
        This signal is emited just after the frame has expanded.
    */
    void expanded();
};

#endif // PCOLLAPSIBLEFRAME_H
