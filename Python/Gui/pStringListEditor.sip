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
	
	virtual void appendValue( const QString& value );
	virtual void setSelectedIndexValue( const QString& value );

protected:
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
