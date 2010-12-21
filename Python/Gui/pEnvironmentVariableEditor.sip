class pEnvironmentVariableEditor : QDialog 
{
%TypeHeaderCode
#include <gui/environmentvariablemanager/pEnvironmentVariableEditor.h>
%End
public:
	pEnvironmentVariableEditor( QWidget* parent = 0, const QString& name = QString::null, const QString& value = QString::null );
	virtual ~pEnvironmentVariableEditor();
	
	pEnvironmentVariablesModel::Variable variable() const;

};
