#ifndef PSTRINGLISTEDITOR_H
#define PSTRINGLISTEDITOR_H

/*!
	\file pStringListEditor.h
	\date 2008-01-14T00:27:49
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
	\brief A widget that handle QStringList edition.
*/

#include "core/FreshExport.h"

#include <QGroupBox>

class QToolBar;
class QAction;
class QListView;
class QStringListModel;
class QVBoxLayout;
class QModelIndex;

/*!
	\brief A widget that handle QStringList edition.
	\details 
*/
class FRESH_EXPORT pStringListEditor : public QGroupBox
{
	Q_OBJECT
	
public:
	pStringListEditor( QWidget* paret = 0 );
	pStringListEditor( const QString& title, QWidget* paret = 0 );

	void setValues( const QStringList& values );
	QStringList values() const;
	
	virtual void appendValue( const QString& value );
	virtual void setSelectedIndexValue( const QString& value );

protected:
	QToolBar* tbActions;
	QAction* aAdd;
	QAction* aEdit;
	QAction* aRemove;
	QAction* aClear;
	QAction* aUp;
	QAction* aDown;
	QListView* lvValues;
	QStringListModel* slmValues;
	QVBoxLayout* vLayout;
	
	virtual void init();
	virtual QModelIndex selectedIndex() const;

protected slots:
	virtual void onSelectionChanged();
	virtual void onAddItem();
	virtual void onEditItem();
	virtual void onRemoveItem();
	virtual void onClearItems();
	virtual void onMoveUpItem();
	virtual void onMoveDownItem();

signals:
	void modified();
};

#endif // PSTRINGLISTEDITOR_H
