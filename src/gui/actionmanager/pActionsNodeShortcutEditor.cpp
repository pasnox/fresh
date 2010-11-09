#include "pActionsNodeShortcutEditor.h"
#include "ui_pActionsNodeShortcutEditor.h"
#include "pActionsNodeModel.h"
#include "gui/models/pRecursiveSortFilterProxyModel.h"
#include "gui/pKeySequenceEdit.h"

#include <QSortFilterProxyModel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

pActionsNodeShortcutEditor::pActionsNodeShortcutEditor( pActionsNodeModel* model, QWidget* parent )
	: QDialog( parent )
{
	Q_ASSERT( model );
	ui = new Ui_pActionsNodeShortcutEditor;
	mModel = model;
	mProxy = new pRecursiveSortFilterProxyModel( this );
	
	mProxy->setSourceModel( mModel );
	mProxy->setFilterCaseSensitivity( Qt::CaseInsensitive );
	mProxy->setSortCaseSensitivity( Qt::CaseInsensitive );
	
	ui->setupUi( this );
	ui->leFilter->setSearchButtonVisible( false );
	ui->leFilter->setPromptText( tr( "Text filter..." ) );
	ui->tvActions->setModel( mProxy );
	ui->tvActions->header()->setResizeMode( 0, QHeaderView::Stretch );
	ui->tvActions->header()->setResizeMode( 1, QHeaderView::ResizeToContents );
	ui->tvActions->header()->setResizeMode( 2, QHeaderView::ResizeToContents );
	ui->tvActions->expandAll();

	// connections
	connect( ui->tvActions->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( tvActions_selectionModel_selectionChanged() ) );
	
	tvActions_selectionModel_selectionChanged();
}

pActionsNodeShortcutEditor::~pActionsNodeShortcutEditor()
{
	delete ui;
}

pActionsNode pActionsNodeShortcutEditor::selectedActionNode() const
{
	const QModelIndex proxyIndex = ui->tvActions->selectionModel()->selectedIndexes().value( 0 );
	const QModelIndex index = mProxy->mapToSource( proxyIndex );
	const pActionsNode node = mModel->indexToNode( index );
	return node.isValid() && node.type() == pActionsNode::Action ? node : pActionsNode();
}

void pActionsNodeShortcutEditor::setNodeShortcut( pActionsNode& node, const QString& shortcut, const QString& error )
{
	if ( !mOriginalShortcuts.contains( node ) ) {
		mOriginalShortcuts[ node ] = node.shortcut();
	}
	
	if ( node.setShortcut( shortcut ) ) {
		tvActions_selectionModel_selectionChanged();
	}
	else {
		QMessageBox::information( this, QString::null, error );
	}
}

void pActionsNodeShortcutEditor::on_leFilter_textChanged( const QString& text )
{
	mProxy->setFilterWildcard( text );
	ui->tvActions->expandAll();
}

void pActionsNodeShortcutEditor::tvActions_selectionModel_selectionChanged()
{
	const pActionsNode node = selectedActionNode();
	
	if ( node.isValid() ) {
		ui->kseShortcut->setText( node.shortcut().toString() );
	}
	else {
		ui->kseShortcut->clear();
	}
	
	ui->kseShortcut->setEnabled( node.isValid() );
	ui->tbSet->setEnabled( false );
	ui->tbClear->setEnabled( node.isValid() && !node.shortcut().isEmpty() );
	ui->dbbButtons->button( QDialogButtonBox::Reset )->setEnabled( false );
	ui->dbbButtons->button( QDialogButtonBox::RestoreDefaults )->setEnabled( node.isValid() && node.shortcut() != node.defaultShortcut() );
	
	ui->kseShortcut->setFocus();
}
void pActionsNodeShortcutEditor::on_kseShortcut_textChanged( const QString& text )
{
	Q_UNUSED( text );
	const pActionsNode node = selectedActionNode();
	
	ui->tbSet->setEnabled( node.isValid() && !ui->kseShortcut->text().isEmpty() );
	ui->dbbButtons->button( QDialogButtonBox::Reset )->setEnabled( true );
	ui->dbbButtons->button( QDialogButtonBox::RestoreDefaults )->setEnabled( node.isValid() && node.shortcut() != node.defaultShortcut() );
}

void pActionsNodeShortcutEditor::on_tbSet_clicked()
{
	pActionsNode node = selectedActionNode();
	
	if ( node.isValid() && !ui->kseShortcut->text().isEmpty() ) {
		setNodeShortcut( node, ui->kseShortcut->text(), tr( "Can't set shortcut, it's maybe already used by another action." ) );
	}
}

void pActionsNodeShortcutEditor::on_tbClear_clicked()
{
	pActionsNode node = selectedActionNode();
	
	if ( node.isValid() ) {
		setNodeShortcut( node, QString::null, QString::null );
	}
}

void pActionsNodeShortcutEditor::on_dbbButtons_clicked( QAbstractButton* button )
{
	switch ( ui->dbbButtons->standardButton( button ) ) {
		case QDialogButtonBox::Reset: {
			tvActions_selectionModel_selectionChanged();
			break;
		}
		case QDialogButtonBox::RestoreDefaults: {
			pActionsNode node = selectedActionNode();
			
			if ( node.isValid() ) {
				setNodeShortcut( node, node.defaultShortcut(), tr( "Can't restore default shortcut, it's maybe already used by another action." ) );
			}
			
			break;
		}
		case QDialogButtonBox::Ok: {
			accept();
			break;
		}
		case QDialogButtonBox::Cancel: {
			foreach ( pActionsNode node, mOriginalShortcuts.keys() ) {
				node.action()->setShortcut( mOriginalShortcuts.value( node ) );
			}
			
			reject();
			break;
		}
		default:
			break;
	}
}
