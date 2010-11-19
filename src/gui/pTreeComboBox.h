#ifndef PTREECOMBOBOX_H
#define PTREECOMBOBOX_H

#include "core/FreshExport.h"

#include <QWidget>
#include <QPersistentModelIndex>
#include <QWeakPointer>

class QFrame;
class QVBoxLayout;
class QTreeView;
class QAbstractItemModel;
class QStyleOptionComboBox;

class FRESH_EXPORT pTreeComboBox : public QWidget
{
	Q_OBJECT

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
	
	int count() const;

protected slots:
	void internal_activated( const QModelIndex& index );
	void internal_clicked( const QModelIndex& index );
	void internal_currentChanged( const QModelIndex& current, const QModelIndex& previous );

protected:
	QSize mIconSize;
	int mModelColumn;
	int mMaxVisibleItems;
	QFrame* mFrame;
	QVBoxLayout* mFrameLayout;
	QWeakPointer<QTreeView> mView;
	QWeakPointer<QAbstractItemModel> mModel;
	QPersistentModelIndex mCurrentIndex;
	bool mForce;

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

#endif // PTREECOMBOBOX_H
