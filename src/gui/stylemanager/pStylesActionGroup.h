#ifndef PSTYLESACTIONGROUP_H
#define PSTYLESACTIONGROUP_H

/*!
	\file pStylesActionGroup.h
	\brief A special QActionGroup for application styles.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QActionGroup>
#include <QMap>

class pActionsNodeMenuBar;

/*!
	\ingroup Gui
	\class pStylesActionGroup
	\brief A special QActionGroup for application styles.
*/
class FRESH_EXPORT pStylesActionGroup : public QActionGroup
{
	Q_OBJECT
	friend class pStylesToolButton;

public:
	/*!
		Create an instance of styles action group having parent \a parent.
	*/
	pStylesActionGroup( QObject* parent = 0 );
	/*!
		Create an instance of styles action group having parent \a parent and having actions text mask \a textFormat.
		\note \a textFormat must contains the %1 term which will be replaced by the style name.
	*/
	pStylesActionGroup( const QString& textFormat, QObject* parent = 0 );
	
	/*!
		Return true is actions are checkable else false.
	*/
	bool isCheckable() const;
	/*!
		Return the checked action style name if applicable, else system style name.
	*/
	QString currentStyle() const;
	/*!
		Return the action representing the system style.
	*/
	QAction* systemAction() const;
	/*!
		Return the action representing the application style.
	*/
	QAction* applicationAction() const;
	/*!
		Return the system style name.
		\note The system style is the style defined when the application start,
		which can be overrided by a call to 'binaryName -style plastique' call.
		So it may be different than the true desktop default style.
		Generally people does not use the -style parameter when starting an application so it should be appropriate for most cases.
	*/
	QString systemStyle() const;
	/*!
		Return the QApplication::style() name.
	*/
	QString applicationStyle() const;
	/*!
		Helper function for adding styles actions in \a menuBar at \a path.
	*/
	void installInMenuBar( pActionsNodeMenuBar* menuBar, const QString& path );
	/*!
		Helper function for adding styles actions in \a menu.
	*/
	void installInMenu( QMenu* menu );

public slots:
	/*!
		Set the action checkable according to \a checkable.
		Checkable actions are fine when inserted in QMenu so user can see current style.
		Non checkable actions are fine when putted in a QToolButton menu like the QDesigner form preview menu.
	*/
	void setCheckable( bool checkable );
	/*!
		Set the current style to \a style.
		\note the application style is not updated instead the styleSelected( const QString& ) signal is emited.
	*/
	void setCurrentStyle( const QString& style );

protected:
	QString mSystemStyle;
	bool mCheckable;
	QString mTextFormat;
	QMap<QString, QAction*> mActions;
	
	void init( const QString& textFormat );
	void updateActions();

protected slots:
	void actionToggled( bool checked );
	void actionTriggered( QAction* action );

signals:
	/*!
		This signal is emited when an action is triggered or toggled, the style clicked is \a style.
	*/
	void styleSelected( const QString& style );
};

#endif // PSTYLESACTIONGROUP_H
