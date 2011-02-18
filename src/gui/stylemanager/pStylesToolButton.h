#ifndef PSTYLESTOOLBUTTON_H
#define PSTYLESTOOLBUTTON_H

/*!
	\file pStylesToolButton.h
	\brief A special QToolButton that has a QMenu of available styles.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QToolButton>

class QMenu;
class pStylesActionGroup;

/*!
	\ingroup Gui
	\class pStylesToolButton
	\brief A special QToolButton that has a QMenu of available styles.
*/
class FRESH_EXPORT pStylesToolButton : public QToolButton
{
	Q_OBJECT

public:
	/*!
		Create an instance of the tool button having parent \a parent.
	*/
	pStylesToolButton( QWidget* parent = 0 );
	/*!
		Create an instance of the tool button having parent \a parent and action text format \a textFormat.
		\sa pStylesActionGroup::pStylesActionGroup( const QString&, QObject* )
	*/
	pStylesToolButton( const QString& textFormat, QWidget* parent = 0 );
	/*!
		\sa pStylesActionGroup::isCheckable()
	*/
	bool isCheckableActions() const;
	/*!
		\sa pStylesActionGroup::currentStyle()
	*/
	QString currentStyle() const;
	/*!
		\sa pStylesActionGroup::systemAction()
	*/
	QAction* systemAction() const;
	/*!
		\sa pStylesActionGroup::applicationAction()
	*/
	QAction* applicationAction() const;
	/*!
		\sa pStylesActionGroup::systemStyle()
	*/
	QString systemStyle() const;
	/*!
		\sa pStylesActionGroup::applicationStyle()
	*/
	QString applicationStyle() const;

public slots:
	/*!
		\sa pStylesActionGroup::setCheckable( bool )
	*/
	void setCheckableActions( bool checkable );
	/*!
		\sa pStylesActionGroup::setCurrentStyle( const QString& )
	*/
	void setCurrentStyle( const QString& style );

protected:
	QMenu* mMenu;
	pStylesActionGroup* mActions;
	
	void init( const QString& textFormat );

signals:
	/*!
		\sa pStylesActionGroup::styleSelected( const QString& );
	*/
	void styleSelected( const QString& style );
};

#endif // PSTYLESTOOLBUTTON_H
