class pTreeComboBox : QWidget
{
%TypeHeaderCode
#include <gui/pTreeComboBox.h>
%End

public:
	pTreeComboBox( QWidget* parent = 0 );
	~pTreeComboBox();

	virtual bool eventFilter( QObject* object, QEvent* event );

	virtual QSize sizeHint() const;

	QSize iconSize() const;
	void setIconSize( const QSize& size );

	virtual void hidePopup();
	virtual void showPopup();

	QTreeView* view() const;
	virtual void setView( QTreeView* view );

	QAbstractItemModel* model() const;
	virtual void setModel( QAbstractItemModel* model );
	
	int modelColumn() const;
	void setColumnModel( int column );

	QModelIndex rootIndex() const;
	void setRootIndex( const QModelIndex& index );

	QModelIndex currentIndex() const;
	void setCurrentIndex( const QModelIndex& index );
	
	void expandAll();

protected slots:
	void internal_activated( const QModelIndex& index );
	void internal_clicked( const QModelIndex& index );
	void internal_currentChanged( const QModelIndex& current, const QModelIndex& previous );

protected:

	virtual void initStyleOption( QStyleOptionComboBox* option ) const;
	
	virtual bool event( QEvent* event );
	virtual void paintEvent( QPaintEvent* event );
	virtual void hideEvent( QHideEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );

	void calculPopupGeometry();
	QModelIndex modelColumnIndex( const QModelIndex& index ) const;

signals:
	void activated( const QModelIndex& index );
	void clicked( const QModelIndex& index );
	void currentIndexChanged( const QModelIndex& index );
	void highlighted( const QModelIndex& index );
};
