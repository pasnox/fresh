/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsShortcutEditor.cpp
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
#include "pActionsShortcutEditor.h"
#include "ui_pActionsShortcutEditor.h"
#include "pActionsModel.h"
#include "models/pRecursiveSortFilterProxyModel.h"
#include "pKeySequenceEdit.h"

#include <QSortFilterProxyModel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

pActionsShortcutEditor::pActionsShortcutEditor( pActionsModel* model, QWidget* parent )
    : QDialog( parent )
{
    Q_ASSERT( model );
    ui = new Ui_pActionsShortcutEditor;
    mModel = model;
    mProxy = new pRecursiveSortFilterProxyModel( this );

    mProxy->setSourceModel( mModel );
    mProxy->setFilterCaseSensitivity( Qt::CaseInsensitive );
    mProxy->setSortCaseSensitivity( Qt::CaseInsensitive );

    ui->setupUi( this );
    ui->leFilter->setSearchButtonVisible( false );
    ui->leFilter->setPromptText( tr( "Text filter..." ) );
    ui->tvActions->setModel( mProxy );
#if QT_VERSION < 0x050000
    ui->tvActions->header()->setResizeMode( 0, QHeaderView::Stretch );
    ui->tvActions->header()->setResizeMode( 1, QHeaderView::ResizeToContents );
    ui->tvActions->header()->setResizeMode( 2, QHeaderView::ResizeToContents );
#else
    ui->tvActions->header()->setSectionResizeMode( 0, QHeaderView::Stretch );
    ui->tvActions->header()->setSectionResizeMode( 1, QHeaderView::ResizeToContents );
    ui->tvActions->header()->setSectionResizeMode( 2, QHeaderView::ResizeToContents );
#endif
    ui->tvActions->expandAll();

    // connections
    connect( ui->tvActions->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( tvActions_selectionModel_selectionChanged() ) );

    tvActions_selectionModel_selectionChanged();
}

pActionsShortcutEditor::~pActionsShortcutEditor()
{
    delete ui;
}

QAction* pActionsShortcutEditor::selectedAction() const
{
    const QModelIndex proxyIndex = ui->tvActions->selectionModel()->selectedIndexes().value( 0 );
    const QModelIndex index = mProxy->mapToSource( proxyIndex );
    QAction* action = mModel->action( index );
    return action && !action->menu() ? action : 0;
}

void pActionsShortcutEditor::setShortcut( QAction* action, const QString& shortcut )
{
    if ( !mOriginalShortcuts.contains( action ) ) {
        mOriginalShortcuts[ action ] = action->shortcut();
    }

    QString error;

    if ( mModel->setShortcut( action, shortcut, &error ) ) {
        tvActions_selectionModel_selectionChanged();
    }
    else {
        QMessageBox::information( this, QString::null, error );
    }
}

void pActionsShortcutEditor::on_leFilter_textChanged( const QString& text )
{
    mProxy->setFilterWildcard( text );
    ui->tvActions->expandAll();
}

void pActionsShortcutEditor::tvActions_selectionModel_selectionChanged()
{
    QAction* action = selectedAction();

    if ( action ) {
        ui->kseShortcut->setText( action->shortcut().toString() );
    }
    else {
        ui->kseShortcut->clear();
    }

    ui->kseShortcut->setEnabled( action );
    ui->tbSet->setEnabled( false );
    ui->tbClear->setEnabled( action && !action->shortcut().isEmpty() );
    ui->dbbButtons->button( QDialogButtonBox::Reset )->setEnabled( false );
    ui->dbbButtons->button( QDialogButtonBox::RestoreDefaults )->setEnabled( action && action->shortcut() != mModel->defaultShortcut( action ) );

    ui->kseShortcut->setFocus();
}
void pActionsShortcutEditor::on_kseShortcut_textChanged( const QString& text )
{
    Q_UNUSED( text );
    QAction* action = selectedAction();

    ui->tbSet->setEnabled( action && !ui->kseShortcut->text().isEmpty() );
    ui->dbbButtons->button( QDialogButtonBox::Reset )->setEnabled( true );
    ui->dbbButtons->button( QDialogButtonBox::RestoreDefaults )->setEnabled( action && action->shortcut() != mModel->defaultShortcut( action ) );
}

void pActionsShortcutEditor::on_tbSet_clicked()
{
    QAction* action = selectedAction();

    if ( action && !ui->kseShortcut->text().isEmpty() ) {
        setShortcut( action, ui->kseShortcut->text() );
    }
}

void pActionsShortcutEditor::on_tbClear_clicked()
{
    QAction* action = selectedAction();

    if ( action ) {
        setShortcut( action, QString::null );
    }
}

void pActionsShortcutEditor::on_dbbButtons_clicked( QAbstractButton* button )
{
    switch ( ui->dbbButtons->standardButton( button ) ) {
        case QDialogButtonBox::Reset: {
            tvActions_selectionModel_selectionChanged();
            break;
        }
        case QDialogButtonBox::RestoreDefaults: {
            QAction* action = selectedAction();

            if ( action ) {
                setShortcut( action, mModel->defaultShortcut( action ).toString() );
            }

            break;
        }
        case QDialogButtonBox::Ok: {
            accept();
            break;
        }
        case QDialogButtonBox::Cancel: {
            foreach ( QAction* action, mOriginalShortcuts.keys() ) {
                action->setShortcut( mOriginalShortcuts.value( action ) );
            }

            reject();
            break;
        }
        default:
            break;
    }
}
