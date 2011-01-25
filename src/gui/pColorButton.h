#ifndef PCOLORBUTTON_H
#define PCOLORBUTTON_H

/*!
	\file pColorButton.h
	\brief A QToolButton that allow to choose a QColor.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QToolButton>

/*!
	\ingroup Gui
	\class pColorButton
	\brief A QToolButton that allow to choose a QColor.
*/
class FRESH_EXPORT pColorButton : public QToolButton
{
	Q_OBJECT
	/*!
	\property pColorButton::color
	\brief the color shown on the button.

	If the button has no color, the color() function will return an anvalid QColor.

	The default color is Qt::black.
	*/
	Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
	
public:
	/*!
		Create an instance of button having parent \a parent.
	*/
	pColorButton( QWidget* parent = 0 );
	/*!
		Create an instance of button having parent \a parent
		and initialized with \a color.
	*/
	pColorButton( const QColor& color, QWidget* parent = 0 );
	/*!
		Return the color.
	*/
	QColor color() const;

protected:
	QColor mColor;
	
	void init( const QColor& color );

public slots:
	/*!
		Set the color to \a color.
	*/
	void setColor( const QColor& color );

protected slots:
	void _q_clicked();

signals:
	/*!
		This signal is emited when the color has changed.
	*/
	void colorChanged( const QColor& color );
};

#endif // PCOLORBUTTON_H
