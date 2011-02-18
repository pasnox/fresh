#ifndef PSTRINGLISTEDITOR_H
#define PSTRINGLISTEDITOR_H

/*!
	\file pStringListEditor.h
	\brief A widget that handle QStringList edition.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

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
	/*!
		Insert \a value at \a index.
		If \a index is inferior to 0 then it's prepend and if it's superior or equals to count() it's append.
	*/
	virtual void insert( int index, const QString& value );
	/*!
		Append \a value at the end of the list.
	*/
	void append( const QString& value );
	/*!
		Select the value at \a row.
	*/
	void selectRow( int row );
	/*!
		Return the row of the selected value.
	*/
	int selectedRow() const;
	/*!
		Select \a value. If the list contains multiple times the same value then the selected one could not be predicted.
	*/
	void selectValue( const QString& value );
	/*!
		Return the selected value. In fact it's the value of the selected row.
		\sa selectedRow()
	*/
	QString selectedValue() const;
	/*!
		Set the value of the selected value.
		\sa selectedValue()
	*/
	void setValue( const QString& value );
	/*!
		Enter in edit mode for \a value. If there is multiple time the same value which one will be edited is not predictable.
	*/
	void editValue( const QString& value );
	/*!
		Enter in edit mode for value at \a row.
	*/
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
	/*!
		This signal is emited when the values are modified.
	*/
	void modified();
};

#endif // PSTRINGLISTEDITOR_H
