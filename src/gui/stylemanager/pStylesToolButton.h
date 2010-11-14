#ifndef PSTYLESTOOLBUTTON_H
#define PSTYLESTOOLBUTTON_H

#include "core/FreshExport.h"

#include <QToolButton>

class QMenu;
class pStylesActionGroup;

class FRESH_EXPORT pStylesToolButton : public QToolButton
{
	Q_OBJECT

public:
	pStylesToolButton( QWidget* parent = 0 );
	pStylesToolButton( const QString& textFormat, QWidget* parent = 0 );
	
	bool isCheckableActions() const;
	QString currentStyle() const;
	
	QAction* systemAction() const;
	QAction* applicationAction() const;
	
	QString systemStyle() const;
	QString applicationStyle() const;

public slots:
	void setCheckableActions( bool checkable );
	void setCurrentStyle( const QString& style );

protected:
	QMenu* mMenu;
	pStylesActionGroup* mActions;
	
	void init( const QString& textFormat );

signals:
	void styleSelected( const QString& style );
};

#endif // PSTYLESTOOLBUTTON_H
