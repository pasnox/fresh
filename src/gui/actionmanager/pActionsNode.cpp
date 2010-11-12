#include "pActionsNode.h"
#include "pActionsNodeModel.h"

uint qHash( const pActionsNode& node )
{
	return qHash( node.path() );
}

// pActionsNode::Data

pActionsNode::Data::Data()
{
	type = pActionsNode::Invalid;
	model = 0;
	parent = 0;
}

pActionsNode::Data::~Data()
{
	if ( model ) {
		model->mNodes.remove( path );
	}
	
	if ( action.data() ) {
		action.data()->deleteLater();
	}
}

bool pActionsNode::Data::operator==( const pActionsNode::Data& other ) const
{
	return type == other.type
		&& path == other.path
		&& action == other.action
		&& icon.cacheKey() == other.icon.cacheKey()
		&& text == other.text
		
		&& model == other.model
		&& parent == other.parent
		&& children == other.children;
}

bool pActionsNode::Data::operator!=( const pActionsNode::Data& other ) const
{
	return !operator==( other );
}

// pActionsNode

pActionsNode::pActionsNode()
{
	d = new pActionsNode::Data;
}

pActionsNode::pActionsNode( const pActionsNode& other )
{
	d = other.d;
}

pActionsNode::pActionsNode( pActionsNode::Type type, const QString& path )
{
	d = new pActionsNode::Data;
	d->type = type;
	d->path = path;
}

bool pActionsNode::operator==( const pActionsNode& other ) const
{
	return d == other.d;
}

bool pActionsNode::operator!=( const pActionsNode& other ) const
{
	return !operator==( other );
}

bool pActionsNode::isValid() const
{
	return d->type != pActionsNode::Invalid;
}

bool pActionsNode::hasChildren() const
{
	return !d->children.isEmpty();
}

pActionsNode::Type pActionsNode::type() const
{
	return d->type;
}

QString pActionsNode::path() const
{
	return d->path;
}

QAction* pActionsNode::action() const
{
	return d->action.data();
}

QIcon pActionsNode::icon() const
{
	switch ( d->type ) {
		case pActionsNode::Action:
			return d->action ? d->action.data()->icon() : QIcon();
		case pActionsNode::Path:
			return d->icon;
		default:
			return QIcon();
	}
}

QString pActionsNode::text() const
{
	switch ( d->type ) {
		case pActionsNode::Action:
			return d->action ? d->action.data()->text() : QString::null;
		case pActionsNode::Path:
			return d->text;
		default:
			return QString::null;
	}
}

pActionsNode pActionsNode::parent() const
{
	return d->parent ? *d->parent : pActionsNode();
}

QList<pActionsNode> pActionsNode::children() const
{
	return d->children;
}

void pActionsNode::setIcon( const QIcon& icon )
{
	if ( !d->model ) {
		return;
	}
	
	const QModelIndex index = d->model->nodeToIndex( *this );
	d->model->setData( index, QVariant::fromValue( icon ), Qt::DecorationRole );
}

void pActionsNode::setText( const QString& text )
{
	if ( !d->model ) {
		return;
	}
	
	const QModelIndex index = d->model->nodeToIndex( *this );
	d->model->setData( index, text, Qt::DisplayRole );
}

QKeySequence pActionsNode::shortcut() const
{
	return d->type == pActionsNode::Action
		? ( d->action ? d->action.data()->shortcut() : QKeySequence() )
		: QKeySequence();
}

bool pActionsNode::setShortcut( const QKeySequence& shortcut )
{
	if ( !d->model ) {
		return false;
	}
	
	const QModelIndex index = d->model->nodeToIndex( *this );
	return d->model->setData( index, shortcut, pActionsNode::ShortcutRole );
}
	
QKeySequence pActionsNode::defaultShortcut() const
{
	return d->type == pActionsNode::Action
		? ( d->action ? d->action.data()->data().value<QKeySequence>() : QKeySequence() )
		: QKeySequence();
}

void pActionsNode::setDefaultShortcut( const QKeySequence& shortcut )
{
	if ( !d->model ) {
		return;
	}
	
	const QModelIndex index = d->model->nodeToIndex( *this );
	if ( d->model->setData( index, shortcut, pActionsNode::DefaultShortcutRole ) ) {
		if ( this->shortcut().isEmpty() ) {
			setShortcut( shortcut );
		}
	}
}

pActionsNode pActionsNode::pathNode( const QString& path, const QString& text, const QIcon& icon )
{
	pActionsNode node( pActionsNode::Path, path );
	node.d->icon = icon;
	node.d->text = text;
	
	if ( node.d->text.isEmpty() ) {
		node.d->text = path.section( '/', -1, -1 );
	}
	
	return node;
}

pActionsNode pActionsNode::actionNode( const QString& path, QAction* action )
{
	pActionsNode node( pActionsNode::Action, path );
	node.d->action = action;
	action->setObjectName( path );
	
	return node;
}
