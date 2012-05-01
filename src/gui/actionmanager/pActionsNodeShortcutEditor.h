/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsNodeShortcutEditor.h
** Date      : 2011-02-20T00:41:09
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
#ifndef PACTIONSNODESHORTCUTEDITOR_H
#define PACTIONSNODESHORTCUTEDITOR_H

/*!
	\file pActionsNodeShortcutEditor.h
	\brief This class allow to edit the pActionsNode shortcuts of a pActionsNodeModel.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pActionsNode.h"

#include <QDialog>

class Ui_pActionsNodeShortcutEditor;
class pActionsNodeModel;
class pRecursiveSortFilterProxyModel;

class QAbstractButton;

/*!
	\ingroup FreshGui
	\class pActionsNodeShortcutEditor
	\brief This class allow to edit the pActionsNode shortcuts of a pActionsNodeModel.
*/
class FRESH_EXPORT pActionsNodeShortcutEditor : public QDialog
{
	Q_OBJECT

public:
	/*!
		Create an editor for editing the model \a model and having parent \a parent.
	*/
	pActionsNodeShortcutEditor( pActionsNodeModel* model, QWidget* parent = 0 );
	/*!
		Destroys the editor.
	*/
	~pActionsNodeShortcutEditor();

protected:
	Ui_pActionsNodeShortcutEditor* ui;
	pActionsNodeModel* mModel;
	pRecursiveSortFilterProxyModel* mProxy;
	QHash<pActionsNode, QKeySequence> mOriginalShortcuts;
	/*!
		Return the selected node of type pActionsNode::Action.
	*/
	pActionsNode selectedActionNode() const;
	/*!
		Set the \a node \a shortcut.
		
		The \a error is shown if the shortcut can't be setted.
	*/
	void setNodeShortcut( pActionsNode& node, const QString& shortcut, const QString& error );

private slots:
	void on_leFilter_textChanged( const QString& text );
	void tvActions_selectionModel_selectionChanged();
	void on_kseShortcut_textChanged( const QString& text );
	void on_tbSet_clicked();
	void on_tbClear_clicked();
	void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // PACTIONSNODESHORTCUTEDITOR_H