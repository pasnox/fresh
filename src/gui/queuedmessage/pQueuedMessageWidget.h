/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pQueuedMessageWidget.h
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#ifndef PQUEUEDMESSAGEWIDGET_H
#define PQUEUEDMESSAGEWIDGET_H

/*!
	\file pQueuedMessageWidget.h
	\brief A widget that show non blocking queued message.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QWidget>
#include <QHash>
#include <QDialogButtonBox>
#include <QWeakPointer>
#include <QVariant>

class QLabel;
class QDialogButtonBox;

/*!
	\ingroup Gui
	\struct pQueuedMessage
	\brief The is the \a message structure used by the pQueuedMessageWidget class.

	It allow to configure the message to show.
	Each variable is directly accessible and mostly optionnal except \c message.
	A default \a Ok button is added if \a buttons is empty.
*/
struct FRESH_EXPORT pQueuedMessage
{
	pQueuedMessage();
	
	/*!
		Return true if message are same.
	*/
	bool operator==( const pQueuedMessage& other ) const;
	
	/*! The message to show. It can contains plain text or html. */
	QString message;
	/*! The millisecond time to wait before the message is auto closed. Use -1 for the default timeout or 0 for unlimited time. */
	int milliSeconds;
	/*! The pixmap to show before the message. */
	QPixmap pixmap;
	/*! The brush to use as background. */
	QBrush background;
	/*! The brush to use as foreground. */
	QBrush foreground;
	/*! A hash representing a StandardButton role, and it's optionnal text overridding the default StandardButton text. */
	QHash<QDialogButtonBox::StandardButton, QString> buttons; // StandardButton, Button Text ( null text for standard text )
	/*! The object that is used to invoke \a Slot. */
	QWeakPointer<QObject> object;
	/*!
		If \a object is not null, it will invoke \a slot, the slot must take 2 parameters :
		\param QDialogButtonBox::StandardButton The StandardButton clicked.
		\param pQueuedMessage The message from where the button was clicked.
	*/
	const char* slot;
	/*! A place to stock custom user data. */
	QVariant userData;
};

/*!
	\ingroup Gui
	\class pQueuedMessageWidget
	\brief A widget that show non blocking queued message.

	The messages are queued until they are closed by user or elapsed time timeouts.
*/
class FRESH_EXPORT pQueuedMessageWidget : public QWidget
{
	Q_OBJECT
	friend class pQueuedMessageToolBar;
	
public:
	/*!
		Create an instance of queued message widget having parent \a parent.
	*/
	pQueuedMessageWidget( QWidget* parent = 0 );
	/*!
		Reimplemented.
	*/
	virtual QSize sizeHint() const;
	/*!
		Return true if external links are opened automatically.
	*/
	bool openExternalLinks() const;
	/*!
		Return the default timeout of messages.
	*/
	int defaultTimeout() const;
	/*!
		Return the default pixmap of messages.
	*/
	QPixmap defaultPixmap() const;
	/*!
		Return the default background of messages.
	*/
	QBrush defaultBackground() const;
	/*!
		Return the default foreground of messages.
	*/
	QBrush defaultForeground() const;
	/*!
		Return the current message data. Pointers can be null if u don't need the information.
	*/
	void currentMessageInformations( QPixmap* pixmap, QBrush* background, QBrush* foreground ) const;
	/*!
		Return the pending messages count. The current displayed one is counted.
	*/
	int pendingMessageCount() const;
	/*!
		Return the currently shown message.
	*/
	pQueuedMessage currentMessage() const;
	/*!
		Append string \a message to show with auto close at \a milliSeconds and return the corresponding pQueuedMessage.
		If the exact same message exists nothing is append.
		The message will be immediatly shown if the messages list is empty, else it's pended.
		\note Use -1 for the default timeout or 0 for unlimited time.
	*/
	pQueuedMessage append( const QString& message, int milliSeconds = -1 );

public slots:
	/*!
		Set auto external links opening according to \a open.
	*/
	void setOpenExternalLinks( bool open );
	/*!
		Set the default timeout to \a timeout.
	*/
	void setDefaultTimeout( int timeout );
	/*!
		Set the default pixmap to \a pixmap.
	*/
	void setDefaultPixmap( const QPixmap& pixmap );
	/*!
		Set the default background to \a brush.
	*/
	void setDefaultBackground( const QBrush& brush );
	/*!
		Set the default foreground to \a brush.
	*/
	void setDefaultForeground( const QBrush& brush );
	/*!
		Append \a message.
	*/
	void append( const pQueuedMessage& message );
	/*!
		Remove pending \a message if it's not the current current viewev one.
	*/
	void remove( const pQueuedMessage& message );
	/*!
		Clear all messages. The current viewed one is cleared too.
	*/
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
	
	/*!
		Helper to get current message pixmap.
	*/
	QPixmap currentMessagePixmap() const;
	/*!
		Helper to get current message background.
	*/
	QBrush currentMessageBackground() const;
	/*!
		Helper to get current message foreground.
	*/
	QBrush currentMessageForeground() const;
	/*!
		Reimplemented.
	*/
	void paintEvent( QPaintEvent* event );

protected slots:
	void buttonClicked( QAbstractButton* button );
	/*!
		Helper that update the pixmap / messages / buttons of the current message.
	*/
	void showMessage();
	/*!
		Helper that close the current message. If there is still pending message the next one will be shown.
	*/
	void closeMessage();
	/*!
		Clear the view (remove pixmap / message / buttons ).
	*/
	void clearMessage();

signals:
	/*!
		This signal is emited when \a message is make visible to user.
	*/
	void shown( const pQueuedMessage& message );
	/*!
		This signal is emited when \a message is closed. A message is closed when the user click one of the buttons
		of a message or when closeMessage() is called.
		\sa clearMessage()
	*/
	void closed( const pQueuedMessage& message );
	/*!
		This signal is emited when clear() is called.
		\sa clear();
	*/
	void cleared();
	/*!
		This message is emited when the last pending message is closed.
	*/
	void finished();
	/*!
		This signal is emited when a link in a message is clicked,
		\a link being the clicked link.
	*/
	void linkActivated( const QString& link );
	/*!
		This signal is emited when a link in a message is hovered,
		\a link being the hovered link.
	*/
	void linkHovered( const QString& link );
};

#endif // PQUEUEDMESSAGEWIDGET_H
