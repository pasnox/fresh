#ifndef PSTRINGLISTEDITOR_H
#define PSTRINGLISTEDITOR_H

/*!
	\file pStringListEditor.h
	\brief A widget that handle QStringList edition.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
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
	\ingroup Gui
	\class pStringListEditor
	\brief A widget that handle QStringList edition.
*/
class FRESH_EXPORT pStringListEditor : public QGroupBox
{
	Q_OBJECT
	
public:
	/*!
		Create an editor instance having parent \a parent.
	*/
	pStringListEditor( QWidget* paret = 0 );
	/*!
		Create an editor instance having parent \a parent and title \a title.
	*/
	pStringListEditor( const QString& title, QWidget* paret = 0 );
	/*!
		Set the editor values to \a values.
	*/
	void setValues( const QStringList& values );
	/*!
		Return the values of the editor.
	*/
	QStringList values() const;
	
	virtual void insert( int index, const QString& value );
	void append( const QString& value );
	
	void selectRow( int row );
	int selectedRow() const;
	
	void selectValue( const QString& value );
	QString selectedValue() const;
	
	void setValue( const QString& value );
	void editValue( const QString& value );
	void editValue( int row );

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
	QModelIndex selectedIndex() const;
	QModelIndex indexForValue( const QString& value ) const;
	QModelIndex indexForRow( int row ) const;
	int rowForValue( const QString& value ) const;

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
