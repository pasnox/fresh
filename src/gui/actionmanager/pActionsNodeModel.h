#ifndef PACTIONSNODEMODEL_H
#define PACTIONSNODEMODEL_H

#include "core/FreshExport.h"

#include <QAbstractItemModel>

#include "pActionsNode.h"

class QAction;

class pActionsNodeModel : public QAbstractItemModel
{
	Q_OBJECT
	friend class pActionsNode::Data;
	friend class pActionsNodeMenuBar;
	
public:
	enum Column {
		Action = 0,
		Shortcut,
		DefaultShortcut
	};
	
	pActionsNodeModel( QObject* parent = 0 );
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;

	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	virtual	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
	
	pActionsNode rootNode() const;
	
	pActionsNode indexToNode( const QModelIndex& index ) const;
	pActionsNode pathToNode( const QString& path ) const;
	pActionsNode actionToNode( QAction* action ) const;
	QModelIndex nodeToIndex( const pActionsNode& node ) const;
	QModelIndex pathToIndex( const QString& path ) const;
	QModelIndex actionToIndex( QAction* action ) const;
	
	void clear();
	
	bool addAction( const QString& path, QAction* action );
	QAction* addAction( const QString& path, const QString& text, const QIcon& icon = QIcon() );
	pActionsNode addPath( const QString& path );
	
	bool removeAction( const QString& path, bool removeEmptyPath = false );
	bool removeAction( QAction* action, bool removeEmptyPath = false );
	bool removePath( const QString& path, bool removeEmptyPath = false );

protected:
	static int mColumnCount;
	pActionsNode mRootNode;
	QHash<QString, pActionsNode*> mNodes;
	
	bool isValid( const QModelIndex& index ) const;
	pActionsNode* nodePointer( const QModelIndex& index ) const;
	pActionsNode* createCompletePathNode( const QString& path );
	void removeCompleteEmptyPathNode( pActionsNode* node );
	
	static QString fixedPath( const QString& path );

protected slots:
	void actionDestroyed( QObject* object );

signals:
	void nodeInserted( const pActionsNode& node );
	void nodeChanged( const pActionsNode& node );
	void nodeRemoved( const pActionsNode& node );
	void nodesCleared();
};

#endif // PACTIONSNODEMODEL_H
