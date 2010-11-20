#include "pTreeComboBox.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QSizeGrip>
#include <QTreeView>
#include <QKeyEvent>
#include <QToolTip>
#include <QStylePainter>
#include <QStack>
#include <QHeaderView>
#include <QInputContext>
#include <QScrollBar>

//Windows and KDE allows menus to cover the taskbar, while GNOME and Mac don't
QRect popupGeometry(int screen)
{
	return QApplication::desktop()->screenGeometry(screen);
/*#ifdef Q_WS_WIN
    return QApplication::desktop()->screenGeometry(screen);
#elif defined Q_WS_X11
    if (X11->desktopEnvironment == DE_KDE)
        return QApplication::desktop()->screenGeometry(screen);
    else
        return QApplication::desktop()->availableGeometry(screen);
#else
        return QApplication::desktop()->availableGeometry(screen);
#endif*/
}

/*!
	\details Create a new pTreeComboBox object
	\param parent The parent widget
*/
pTreeComboBox::pTreeComboBox( QWidget* parent )
	: QWidget( parent )
{
	mIconSize = QSize( -1, -1 );
	mForce = false;
	mModelColumn = 0;
	mMaxVisibleItems = 10;
	mFrame = new QFrame( this );
	mFrameLayout = new QVBoxLayout( mFrame );
	
	mFrame->setWindowFlags( Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
	//mFrame->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
	mFrame->setVisible( false );
	mFrame->installEventFilter( this );
	
	mFrameLayout->setMargin( 3 );
	mFrameLayout->setSpacing( 1 );
	mFrameLayout->addWidget( new QSizeGrip( mFrame ), 0, Qt::AlignBottom | Qt::AlignRight );
	
	setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed ) );
	setAttribute( Qt::WA_Hover );
	setView( new QTreeView( this ) );
}

pTreeComboBox::~pTreeComboBox()
{
}

bool pTreeComboBox::eventFilter( QObject* object, QEvent* event )
{
	switch ( event->type() ) {
		case QEvent::WindowDeactivate: {
			if ( object == mFrame ) {
				if ( !rect().contains( mapFromGlobal( QCursor::pos() ) ) ) {
					hidePopup();
				}
			}
			
			break;
		}
		case QEvent::KeyPress: {
			if ( object == mView.data() ) {
				const QKeyEvent* ke = static_cast<QKeyEvent*>( event );
				
				if ( ke->key() == Qt::Key_Escape && ke->modifiers() == Qt::NoModifier ) {
					hidePopup();
				}
			}
			
			break;
		}
		case QEvent::Hide: {
			if ( object == mFrame ) {
				const QModelIndex index = modelColumnIndex( mCurrentIndex );
				
				if ( currentIndex() != index ) {
					mForce = false;
					mView.data()->clearSelection();
					mView.data()->setCurrentIndex( index );
					mForce = true;
				}
				
				update();
			}
			
			break;
		}
		case QEvent::MouseMove: {
			if ( mView.data() && object == mView.data()->viewport() ) {
				const QMouseEvent* me = static_cast<QMouseEvent*>( event );
				const QModelIndex index = modelColumnIndex( mView.data()->indexAt( me->pos() ) );
				
				if ( mView.data()->currentIndex() != index ) {
					emit highlighted( index );
					mForce = false;
					mView.data()->clearSelection();
					mView.data()->setCurrentIndex( index );
					mForce = true;
				}
			}
			
			break;
		}
		default:
			break;
	}
	
	return QWidget::eventFilter( object, event );
}

/*!
	\details Return the widget size hint
*/
QSize pTreeComboBox::sizeHint() const
{
	const QFontMetrics fm( font() );
	QSize size = mCurrentIndex.isValid()
		? mView.data()->sizeHintForIndex( mCurrentIndex )
		: QSize( style()->pixelMetric( QStyle::QStyle::PM_ComboBoxFrameWidth ), qMax( fm.lineSpacing(), 14 ) +2 )
		;
	QStyleOptionComboBox option;
	initStyleOption( &option );
	
	if ( !option.currentIcon.isNull() ) {
		size.setHeight( qMax( size.height(), option.iconSize.height() +2 ) );
	}
	
	size = style()->sizeFromContents( QStyle::CT_ComboBox, &option, size, this );
    return size.expandedTo( QApplication::globalStrut() );
}

/*!
	\details Return the iconSize
*/
QSize pTreeComboBox::iconSize() const
{
	return mIconSize;
}

/*!
	\details Set the icon size
*/
void pTreeComboBox::setIconSize( const QSize& s )
{
	if ( mIconSize != s ) {
		mIconSize = s;
		updateGeometry();
		update();
	}
}

int pTreeComboBox::modelColumn() const
{
	return mModelColumn;
}

void pTreeComboBox::setColumnModel( int column )
{
	if ( mModelColumn != column ) {
		mModelColumn = column;
		mCurrentIndex = modelColumnIndex( mCurrentIndex );
		updateGeometry();
		update();
	}
}

void pTreeComboBox::initStyleOption( QStyleOptionComboBox* option ) const
{
	// QStyleOption
	option->initFrom( this );
	
	// QStyleOptionComplex
	option->activeSubControls = 0;
	option->subControls = QStyle::SC_ComboBoxEditField | QStyle::SC_ComboBoxArrow | QStyle::SC_ComboBoxFrame;

	// QStyleOptionComboBox
	const QModelIndex index = currentIndex();
	const int iconSize = style()->pixelMetric( QStyle::PM_ButtonIconSize );
	option->currentIcon = index.data( Qt::DecorationRole ).value<QIcon>();
	option->iconSize = mIconSize == QSize( -1, -1 ) ? QSize( iconSize, iconSize ) : mIconSize;
	option->currentText = index.data( Qt::DisplayRole ).toString();
	option->editable = false;
	option->frame = true;
	option->popupRect = QRect(); // unused by Qt api Qt 4.7
	
	if ( mView ) {
		const QPoint pos = mapFromGlobal( QCursor::pos() );
		
		if ( !mView.data()->isVisible() && rect().contains( pos ) ) {
			option->state |= QStyle::State_MouseOver;
		}
		
		if ( mView.data()->isVisible() ) {
			option->state |= QStyle::State_On;
		}
	}
}

bool pTreeComboBox::event( QEvent* event )
{
	switch ( event->type() ) {
		case QEvent::ToolTip: {
			const QHelpEvent* he = static_cast<QHelpEvent*>( event );
			const QString text = toolTip().isEmpty() ? mCurrentIndex.data( Qt::ToolTipRole ).toString() : toolTip();
			QToolTip::showText( he->globalPos(), text, this );
			event->accept();
			return true;
		}
		default:
			return QWidget::event( event );
	}
}

void pTreeComboBox::paintEvent( QPaintEvent* event )
{
	Q_UNUSED( event );
	
	QStyleOptionComboBox option;
	initStyleOption( &option );
	
	QStylePainter painter( this );
	painter.drawComplexControl( QStyle::CC_ComboBox, option );
	painter.drawControl( QStyle::CE_ComboBoxLabel, option );
}

void pTreeComboBox::hideEvent( QHideEvent* event )
{
	hidePopup();
	QWidget::hideEvent( event );
}

void pTreeComboBox::mousePressEvent( QMouseEvent* event )
{
	if ( mView ) {
		mFrame->isVisible() ? hidePopup() : showPopup();
	}
	
	QWidget::mousePressEvent( event );
}

/*!
	\details Hide the popup
*/
void pTreeComboBox::hidePopup()
{
	if ( mFrame->isVisible() ) {
		mFrame->hide();
	}
}

/*!
	\details Show the popup
*/
void pTreeComboBox::showPopup()
{
	if ( mView && !mFrame->isVisible() ) {
		mCurrentIndex = modelColumnIndex( currentIndex() );
		calculPopupGeometry();
		update();
	}
}

QModelIndex pTreeComboBox::modelColumnIndex( const QModelIndex& index ) const
{
	return index.sibling( index.row(), mModelColumn );
}

void pTreeComboBox::calculPopupGeometry()
{
	if ( !mView ) {
		return;
	}
	
	QStyle * const style = this->style();

    // set current item and select it
    view()->selectionModel()->setCurrentIndex( mCurrentIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows );
    QFrame* container = mFrame;
    QStyleOptionComboBox opt;
    initStyleOption( &opt );
    QRect listRect( style->subControlRect( QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxListBoxPopup, this ) );
    QRect screen = popupGeometry( QApplication::desktop()->screenNumber( this ) );
    QPoint below = mapToGlobal( listRect.bottomLeft() );
    int belowHeight = screen.bottom() -below.y();
    QPoint above = mapToGlobal( listRect.topLeft() );
    int aboveHeight = above.y() -screen.y();
    bool boundToScreen = !window()->testAttribute( Qt::WA_DontShowOnScreen );
	
	listRect.moveTopLeft( mapToGlobal( rect().bottomLeft() ) );
	listRect.setSize( QSize( 
		qMax( qMax( view()->viewport()->width(), mFrame->width() ), width() )
		,
		qMax( view()->viewport()->height(), mFrame->height() )
	) );

    const bool usePopup = style->styleHint( QStyle::SH_ComboBox_Popup, &opt, this );
    {
        int listHeight = 0;
        int count = 0;
        QStack<QModelIndex> toCheck;
        toCheck.push( view()->rootIndex() );
#ifndef QT_NO_TREEVIEW
        QTreeView* treeView = qobject_cast<QTreeView*>( view() );
        if ( treeView && treeView->header() && !treeView->header()->isHidden() )
            listHeight += treeView->header()->height();
#endif
        while ( !toCheck.isEmpty() ) {
            QModelIndex parent = toCheck.pop();
            for ( int i = 0; i < model()->rowCount( parent ); ++i ) {
                QModelIndex idx = model()->index( i, mModelColumn, parent );
                if ( !idx.isValid() )
                    continue;
                listHeight += view()->visualRect( idx ).height();
#ifndef QT_NO_TREEVIEW
                if ( model()->hasChildren( idx ) && treeView && treeView->isExpanded( idx ) )
                    toCheck.push( idx );
#endif
                ++count;
                if ( !usePopup && count > mMaxVisibleItems ) {
                    toCheck.clear();
                    break;
                }
            }
        }
        listRect.setHeight( listHeight );
    }

    {
        // add the spacing for the grid on the top and the bottom;
        int heightMargin = 0;

        // add the frame of the container
        int marginTop, marginBottom;
        container->getContentsMargins( 0, &marginTop, 0, &marginBottom );
        heightMargin += marginTop +marginBottom;

        //add the frame of the view
        view()->getContentsMargins( 0, &marginTop, 0, &marginBottom );
        marginTop += 0/*static_cast<QAbstractScrollAreaPrivate *>(QObjectPrivate::get(view()))->top*/;
        marginBottom += 0/*static_cast<QAbstractScrollAreaPrivate *>(QObjectPrivate::get(view()))->bottom*/;
        heightMargin += marginTop +marginBottom;

        listRect.setHeight( listRect.height() +heightMargin );
    }

    // Add space for margin at top and bottom if the style wants it.
    if ( usePopup )
        listRect.setHeight( listRect.height() +style->pixelMetric( QStyle::PM_MenuVMargin, &opt, this ) *2 );

    // Make sure the popup is wide enough to display its contents.
    if ( usePopup ) {
        const int diff = sizeHint().width() /*d->computeWidthHint()*/ -width();
        if ( diff > 0 )
            listRect.setWidth( listRect.width() +diff );
    }

    //we need to activate the layout to make sure the min/maximum size are set when the widget was not yet show
    container->layout()->activate();
    //takes account of the minimum/maximum size of the container
    listRect.setSize( listRect.size().expandedTo(container->minimumSize())
                      .boundedTo(container->maximumSize()));

    // make sure the widget fits on screen
    if (boundToScreen) {
        if (listRect.width() > screen.width() )
            listRect.setWidth(screen.width());
        if (/*mapToGlobal(*/listRect/*.bottomRight())*/.x() > screen.right()) {
            below.setX(screen.x() + screen.width() - listRect.width());
            above.setX(screen.x() + screen.width() - listRect.width());
        }
        if (/*mapToGlobal(*/listRect/*.topLeft())*/.x() < screen.x() ) {
            below.setX(screen.x());
            above.setX(screen.x());
        }
    }

    if ( usePopup ) {
        // Position horizontally.
        listRect.moveLeft( above.x() );

        // Position vertically so the curently selected item lines up
        // with the combo box.
        /*const QRect currentItemRect = view()->visualRect( view()->currentIndex() );
        const int offset = listRect.top() -currentItemRect.top();
        listRect.moveTop( above.y() +offset -listRect.top() );*/

        // Clamp the listRect height and vertical position so we don't expand outside the
        // available screen geometry.This may override the vertical position, but it is more
        // important to show as much as possible of the popup.
        const int height = !boundToScreen ? listRect.height() : qMin(listRect.height(), screen.height());
        listRect.setHeight(height);
        if (boundToScreen) {
            if (listRect.top() < screen.top())
                listRect.moveTop(screen.top());
            if (listRect.bottom() > screen.bottom())
                listRect.moveBottom(screen.bottom());
        }
    } else if (!boundToScreen || listRect.height() <= belowHeight) {
        listRect.moveTopLeft(below);
    } else if (listRect.height() <= aboveHeight) {
        listRect.moveBottomLeft(above);
    } else if (belowHeight >= aboveHeight) {
        listRect.setHeight(belowHeight);
        listRect.moveTopLeft(below);
    } else {
        listRect.setHeight(aboveHeight);
        listRect.moveBottomLeft(above);
    }

#ifndef QT_NO_IM
    if ( QInputContext *qic = inputContext() )
        qic->reset();
#endif
    QScrollBar* sb = view()->horizontalScrollBar();
    Qt::ScrollBarPolicy policy = view()->horizontalScrollBarPolicy();
    bool needHorizontalScrollBar = ( policy == Qt::ScrollBarAsNeeded || policy == Qt::ScrollBarAlwaysOn ) && sb->minimum() < sb->maximum();
    if ( needHorizontalScrollBar ) {
        listRect.adjust( 0, 0, 0, sb->height() );
    }
	
    container->setGeometry( listRect );

#ifndef Q_WS_MAC
    const bool updatesEnabled = container->updatesEnabled();
#endif

#if defined( Q_WS_WIN ) && !defined( QT_NO_EFFECTS )
    bool scrollDown = ( listRect.topLeft() == below );
    if ( QApplication::isEffectEnabled( Qt::UI_AnimateCombo ) 
        && !style->styleHint( QStyle::SH_ComboBox_Popup, &opt, this ) && !window()->testAttribute( Qt::WA_DontShowOnScreen ) )
        qScrollEffect( container, scrollDown ? QEffects::DownScroll : QEffects::UpScroll, 150 );
#endif

// Don't disable updates on Mac OS X. Windows are displayed immediately on this platform,
// which means that the window will be visible before the call to container->show() returns.
// If updates are disabled at this point we'll miss our chance at painting the popup 
// menu before it's shown, causing flicker since the window then displays the standard gray 
// background.
#ifndef Q_WS_MAC
    container->setUpdatesEnabled( false );
#endif

    container->raise();
    container->show();
    //container->updateScrollers();
    view()->setFocus();

    view()->scrollTo( view()->currentIndex(), style->styleHint( QStyle::SH_ComboBox_Popup, &opt, this ) ? QAbstractItemView::PositionAtCenter : QAbstractItemView::EnsureVisible );

#ifndef Q_WS_MAC
    container->setUpdatesEnabled( updatesEnabled );
#endif

    container->update();
#ifdef QT_KEYPAD_NAVIGATION
    if ( QApplication::keypadNavigationEnabled() )
        view()->setEditFocus( true );
#endif
}



/*!
	\details Return the view widget
*/
QTreeView* pTreeComboBox::view() const
{
	return mView.data();
}

/*!
	\details Set the view widget
*/
void pTreeComboBox::setView( QTreeView* view )
{
	if ( mView.data() == view ) {
		return;
	}
	
	delete mView.data();
	mView = view;
	
	if ( mView ) {
		mFrameLayout->insertWidget( 0, mView.data() );
		mView.data()->setEditTriggers( QAbstractItemView::NoEditTriggers );
		mView.data()->setMouseTracking( true );
		mView.data()->installEventFilter( this );
		mView.data()->viewport()->installEventFilter( this );
		
		setModel( mModel.data() );
		
		connect( mView.data(), SIGNAL( activated( const QModelIndex& ) ), this, SLOT( internal_activated( const QModelIndex& ) ) );
		connect( mView.data(), SIGNAL( clicked( const QModelIndex& ) ), this, SLOT( internal_clicked( const QModelIndex& ) ) );
	}
}

/*!
	\details Return the used model
*/
QAbstractItemModel* pTreeComboBox::model() const
{
	return mModel.data();
}

/*!
	\details Set the model
*/
void pTreeComboBox::setModel( QAbstractItemModel* model )
{
	if ( mModel.data() != model ) {
		mModel = model;
	}
	
	if ( mView && mView.data()->model() != mModel.data() ) {
		mView.data()->setModel( mModel.data() );
		connect( mView.data()->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( internal_currentChanged( const QModelIndex&, const QModelIndex& ) ) );
	}
}

/*!
	\details Return the root index
*/
QModelIndex pTreeComboBox::rootIndex() const
{
	return mView ? mView.data()->rootIndex() : QModelIndex();
}

/*!
	\details Set the root index
*/
void pTreeComboBox::setRootIndex( const QModelIndex& index )
{
	if ( mView ) {
		mView.data()->setRootIndex( index );
	}
}

/*!
	\details Return the current index
*/
QModelIndex pTreeComboBox::currentIndex() const
{
	if ( mView ) {
		return mFrame->isVisible() ? QModelIndex( mCurrentIndex ) : mView.data()->currentIndex();
	}
	
	return QModelIndex();
}

/*!
	\details Set the current index
*/
void pTreeComboBox::setCurrentIndex( const QModelIndex& index )
{
	if ( mView && ( currentIndex() != index || !index.isValid() ) ) {
		mCurrentIndex = modelColumnIndex( index );
		mForce = true;
		mView.data()->clearSelection();
		mView.data()->setCurrentIndex( mCurrentIndex );
		mForce = false;
		update();
	}
}

/*!
	\details Expand all rows
*/
void pTreeComboBox::expandAll()
{
	if ( mView ) {
		mView.data()->expandAll();
	}
}

void pTreeComboBox::internal_activated( const QModelIndex& _index )
{
	const QModelIndex index = modelColumnIndex( _index );
	
	if ( !( index.flags() & Qt::ItemIsEnabled ) || !( index.flags() & Qt::ItemIsSelectable ) ) {
		return;
	}
	
	if ( mCurrentIndex != index ) {
		mCurrentIndex = index;
		emit currentIndexChanged( index );
	}
	
	emit activated( index );
	hidePopup();
}

void pTreeComboBox::internal_clicked( const QModelIndex& _index )
{
	const QModelIndex index = modelColumnIndex( _index );
	
	if ( !( index.flags() & Qt::ItemIsEnabled ) || !( index.flags() & Qt::ItemIsSelectable ) ) {
		return;
	}
	
	if ( mCurrentIndex != index ) {
		mCurrentIndex = index;
		emit currentIndexChanged( index );
	}
	
	emit clicked( index );
	hidePopup();
}

void pTreeComboBox::internal_currentChanged( const QModelIndex& current, const QModelIndex& previous )
{
	Q_UNUSED( previous );
	const QModelIndex index = modelColumnIndex( current );
	
	if ( mForce ) {
		emit currentIndexChanged( index );
	}
}
