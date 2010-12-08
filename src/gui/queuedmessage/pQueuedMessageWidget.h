#ifndef PQUEUEDMESSAGEWIDGET_H
#define PQUEUEDMESSAGEWIDGET_H

#include "core/FreshExport.h"

#include <QWidget>
#include <QHash>
#include <QDialogButtonBox>
#include <QWeakPointer>
#include <QVariant>

class QLabel;
class QDialogButtonBox;

/*!
	\brief The is the \c message structure used by the pQueuedMessageWidget class.
	\details It allow to configure the message to show.
	\details Each variable is directly accessible and mostly optionnal except \c message.
	\details A default \c Ok button is added if \c buttons is empty.
*/
struct FRESH_EXPORT pQueuedMessage
{
	pQueuedMessage()
	{
		milliSeconds = -1;
		background = QBrush( Qt::NoBrush );
		foreground = QBrush( Qt::NoBrush );
		slot = 0;
	}
	
	bool operator==( const pQueuedMessage& other ) const
	{
		return
			message == other.message
			&& milliSeconds == other.milliSeconds
			&& ( pixmap.cacheKey() == other.pixmap.cacheKey() || ( pixmap.isNull() && other.pixmap.isNull() ) )
			&& background == other.background
			&& foreground == other.foreground
			&& buttons == other.buttons
			&& object == other.object
			&& slot == other.slot
			&& userData == other.userData
			;
	}
	
	/*! \details The message to show */
	QString message;
	/*! \details The millisecond time to wait before the message is auto closed. Use 0 for unilimited time */
	int milliSeconds;
	/*! \details The pixmap to show before the message */
	QPixmap pixmap;
	/*! \details The brush to use as background */
	QBrush background;
	/*! \details The brush to use as foreground */
	QBrush foreground;
	/*! \details A hash representing a StandardButton role, and it's optionnal text overridding the default StandardButton text */
	QHash<QDialogButtonBox::StandardButton, QString> buttons; // StandardButton, Button Text ( null text for standard text )
	/*! \details The object that is used to invoke \c Slot */
	QWeakPointer<QObject> object;
	/*!
		\details If \c object is not null, it will invoke this \c slot, the slot must take 2 parameters :
		\details QDialogButtonBox::StandardButton : The StandardButton clicked
		\details pQueuedMessage : The message from where the button was clicked
	*/
	const char* slot;
	/*! \details A place to stock custom user data */
	QVariant userData;
};

/*!
	\brief A QMessageBox like Widget, that can show queued message.
	\details The messages are queued until they are closed by user or elapsed time is timeout
*/
class FRESH_EXPORT pQueuedMessageWidget : public QWidget
{
	Q_OBJECT
	friend class pQueuedMessageToolBar;
	
public:
	pQueuedMessageWidget( QWidget* parent = 0 );
	
	virtual QSize sizeHint() const;
	
	bool openExternalLinks() const;
	int defaultTimeout() const;
	QPixmap defaultPixmap() const;
	QBrush defaultBackground() const;
	QBrush defaultForeground() const;
	void currentMessageInformations( QPixmap* pixmap, QBrush* background, QBrush* foreground ) const;
	
	int pendingMessageCount() const;
	pQueuedMessage currentMessage() const;
	pQueuedMessage append( const QString& message, int milliSeconds = -1 );

public slots:
	void setOpenExternalLinks( bool open );
	void setDefaultTimeout( int timeout );
	void setDefaultPixmap( const QPixmap& pixmap );
	void setDefaultBackground( const QBrush& brush );
	void setDefaultForeground( const QBrush& brush );
	void append( const pQueuedMessage& message );
	void remove( const pQueuedMessage& message );
	void clear();

protected:
	int mDefaultTimeout;
	QPixmap mDefaultPixmap;
	QBrush mDefaultBackground;
	QBrush mDefaultForeground;
	QLabel* lPixmap;
	QLabel* lMessage;
	QDialogButtonBox* dbbButtons;
	QList<pQueuedMessage> mMessages;
	
	QPixmap currentMessagePixmap() const;
	QBrush currentMessageBackground() const;
	QBrush currentMessageForeground() const;
	
	void paintEvent( QPaintEvent* event );

protected slots:
	void buttonClicked( QAbstractButton* button );
	void showMessage();
	void closeMessage();
	void clearMessage();

signals:
	void shown( const pQueuedMessage& message );
	void closed( const pQueuedMessage& message );
	void cleared();
	void finished();
	void linkActivated( const QString& link );
	void linkHovered( const QString& link );
};

#endif // PQUEUEDMESSAGEWIDGET_H
