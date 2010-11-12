#ifndef PACTIONSNODE_H
#define PACTIONSNODE_H

#include "core/FreshExport.h"

#include <QWeakPointer>
#include <QSharedData>
#include <QExplicitlySharedDataPointer>
#include <QAction>

class pActionsNodeModel;

class FRESH_EXPORT pActionsNode
{
	friend class pActionsNodeModel;
	
public:
	enum Type {
		Invalid,
		Path,
		Action
	};
	
	enum Role {
		ShortcutRole = Qt::UserRole,
		DefaultShortcutRole
	};
	
	pActionsNode();
	pActionsNode( const pActionsNode& other );
	pActionsNode( pActionsNode::Type type, const QString& path );
	
	bool operator==( const pActionsNode& other ) const;
	bool operator!=( const pActionsNode& other ) const;
	
	bool isValid() const;
	bool hasChildren() const;
	pActionsNode::Type type() const;
	QString path() const;
	QAction* action() const;
	QIcon icon() const;
	QString text() const;
	pActionsNode parent() const;
	QList<pActionsNode> children() const;
	
	void setIcon( const QIcon& icon );
	void setText( const QString& text );
	
	QKeySequence shortcut() const;
	bool setShortcut( const QKeySequence& shortcut );
	
	QKeySequence defaultShortcut() const;
	void setDefaultShortcut( const QKeySequence& shortcut );
	
	static pActionsNode pathNode( const QString& path, const QString& text = QString::null, const QIcon& icon = QIcon() );
	static pActionsNode actionNode( const QString& path, QAction* action );

protected:
	class Data : public QSharedData
	{
	public:
		Data();
		virtual ~Data();
		
		bool operator==( const pActionsNode::Data& other ) const;
		bool operator!=( const pActionsNode::Data& other ) const;
		
		pActionsNode::Type type;
		QString path;
		QWeakPointer<QAction> action;
		QIcon icon;
		QString text;
		
		pActionsNodeModel* model;
		pActionsNode* parent;
		QList<pActionsNode> children;
	};
	
	QExplicitlySharedDataPointer<pActionsNode::Data> d;
};

uint qHash( const pActionsNode& node );

#endif // PACTIONSNODE_H
