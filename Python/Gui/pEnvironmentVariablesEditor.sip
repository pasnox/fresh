class pEnvironmentVariablesEditor : QWidget
{
%TypeHeaderCode
#include <gui/environmentvariablemanager/pEnvironmentVariablesEditor.h>
%End

public:
	pEnvironmentVariablesEditor( QWidget* parent = 0 );
	virtual ~pEnvironmentVariablesEditor();

	pEnvironmentVariablesModel::Variables variables() const;
	pEnvironmentVariablesModel::Variables defaultVariables() const;
	QStringList variables( bool keepDisabled ) const;
	pEnvironmentVariablesModel::Variable variable( const QString& name ) const;
	bool contains( const QString& name ) const;
	bool isEmpty() const;

public slots:
	void setVariables( const pEnvironmentVariablesModel::Variables& variables, bool setDefault );
	void setDefaultVariables( const pEnvironmentVariablesModel::Variables& variables );
	void setVariables( const QStringList& variables, bool setDefault );
	void setVariable( const QString& name, const pEnvironmentVariablesModel::Variable& variable );
	void removeVariable( const QString& name );
	void clearVariables();
	void resetVariablesToDefault();
	void resetVariablesToSystem( bool setDefault );


protected slots:
	void model_view_changed();
	void on_aAdd_triggered();
	void on_aEdit_triggered();
	void on_aRemove_triggered();
	void on_aClear_triggered();
	void on_aResetDefault_triggered();
	void on_aResetSystem_triggered();
	void on_tvVariables_activated( const QModelIndex& index );
};
