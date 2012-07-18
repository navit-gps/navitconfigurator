/*
 * TreeView.h
 *
 *  Created on: 20.05.2012
 *      Author: remo
 */

#ifndef TREEVIEW_H_
#define TREEVIEW_H_

#include <QtCore/QPoint>
#include <QtCore/QAbstractItemModel>

#include <QtGui/QTreeView>
#include <QtGui/QWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QAction>

/**
 * Extends the normal QTreeView for enabling the context menu.
 */
class TreeView : public QTreeView {
	Q_OBJECT
public:
	explicit TreeView();
	virtual ~TreeView();
	void setupUi(QWidget& parent, QGridLayout& gridLayout);
    void setModel(QAbstractItemModel* model);
    void dataChanged(const QModelIndex& indexTopLeft, const QModelIndex& indexRightBottom);
private:
	enum MyContextAction {
		ADD,
		RENAME_NODE,
		DELETE,
		EXPAND_ALL_CHILDREN,
		COLLAPS_ALL_CHILDREN,
		DEBUG_TREE
	};
	QAction* createAction(const QString& name, MyContextAction action, const QString& shortcut = QString());
	QAction* createSeparator();
	void expandAllChildren(const QModelIndex& parentIndex);
	void collapseAllChildren(const QModelIndex& parentIndex);
	void show(const QModelIndex& index);
private slots:
    void showContextMenu(const QPoint& pos);
};

#endif /* TREEVIEW_H_ */
