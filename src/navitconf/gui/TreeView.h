/**
 * NavitConfigurator, makes it easy to configure Navit
 * Copyright (C) 2012 Raimar Bühmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
