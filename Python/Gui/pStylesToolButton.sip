class pStylesToolButton : QToolButton
{
%TypeHeaderCode
#include <gui/stylemanager/pStylesToolButton.h>
%End

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
	void init( const QString& textFormat );

signals:
	void styleSelected( const QString& style );
};
