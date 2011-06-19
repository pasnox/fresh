/*!
	\file pStringListEditor.h
	\date 2008-01-14T00:27:49
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A widget that handle QStringList edition.
*/

/*!
	\brief A widget that handle QStringList edition.
	\details 
*/
class pStringListEditor : QGroupBox
{
%TypeHeaderCode
#include <gui/listeditor/pStringListEditor.h>
%End

	
public:
	pStringListEditor( QWidget* paret = 0 );
	pStringListEditor( const QString& title, QWidget* paret = 0 );

	void setValues( const QStringList& values );
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
