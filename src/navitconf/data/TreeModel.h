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

#ifndef TREEMODEL_H_
#define TREEMODEL_H_

#include <QtCore/Qt>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QVariant>
#include <QtCore/QModelIndex>
#include <QtCore/QList>
#include <QtCore/QMap>

#include <QtWidgets/QUndoStack>

#include "TreeItem.h"

/**
 * Data model for the QTreeView representing the Navit XML file.
 * An index with the type QModelIndex has two columns for the tag name and the concatenated
 * attribute-value pairs. Each item could have childrens with a position specified by the row in the index.
 * Use {@link TreeModel#getTreeItem(index) to convert an index to a tree item.
 * @see http://qt-project.org/doc/qt-5.0/itemviews-simpletreemodel.html
 * @see http://qt-project.org/doc/qt-5.0/itemviews-simpledommodel.html
 * @see http://qt-project.org/doc/qt-5.0/qabstractitemmodel.html
 * @see http://www.qtforum.de/forum/viewtopic.php?p=61617
 * @see http://doc.trolltech.com/main-snapshot/model-view-programming.html#model-subclassing-reference
 */
class TreeModel: public QAbstractItemModel {
	Q_OBJECT
	friend class AbstractTreeNodeCommand; // for calling emitDataChanged()
	friend class TreeItemRenameCommand;
	friend class TreeItemSetAttributeCommand;
	friend class TreeItemSetNodeValueCommand;
public:
	/**
	 * @param domDocument The DOM document this model is working on.
	 * @param undoStack The undo stack for this model, which is indirectly cleared by this constructor.
	 */
	TreeModel(QDomDocument& domDocument, QUndoStack& undoStack);
	virtual ~TreeModel();
	QUndoStack& getUndoStack();
	/**
	 * @return The tree item corresponding to the given index, if index.isValid().
	 *         Otherwise the root tree item.
	 */
	const TreeItem& getTreeItem(const QModelIndex& modelIndex) const;
	TreeItem& getTreeItem(const QModelIndex& modelIndex);
	/**
	 * @param parent The parent index. No parameter means the root index as parent.
	 * @return The number of children from the given parent index.
	 */
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex& childIndex) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	/**
	 * Create model index.
	 * @return Model index with given parameters. If parentIndex is not given,
	 *         the root index is returned.
	 */
	QModelIndex index(int row, int column, const QModelIndex& parentIndex = QModelIndex()) const;
	QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;
	/**
	 * @param name The new node name as a QString.
	 * @return True, if index has Qt::EditRole,
	 *         the new name is not equal to the existing name and
	 *         the name without spaces is not empty.
	 */
	bool setData(const QModelIndex& index, const QVariant& name, int role = Qt::EditRole);
	bool hasTreeItem(const QModelIndex& modelIndex) const;

	/**
	 * Insert one node named "NewNode" at the given row with undo support.
	 * @param count Not supported.
	 */
    bool insertRows(int row, int count, const QModelIndex& parentIndex = QModelIndex());
    /**
     * Remove one node at the given position with undo support.
     * @param count Not supported yet.
     */
    bool removeRows(int row, int count, const QModelIndex& parentIndex = QModelIndex());

    /**
     * Insert a tree item at a given position with undo support.
     * @param name The new tag name of the new tree item.
     * @param row The child row of the new tree item.
     * @param parentIndex The parent index of the new tree item.
     * @return Reference to the inserted tree item.
     * @throws QString If the name without spaces is empty.
     */
    TreeItem& insertTreeItem(const QString& name, int row, const QModelIndex& parentIndex);

    /**
     * Copy a tree item to a given position with undo support.
     * @param sourceIndex The source tree item given by an index.
     * @param row The child row of the new tree item.
     * @param parentIndex The parent index of the new tree item.
     * @return Reference to the inserted tree item.
     * @throws QString If the name without spaces is empty.
     */
    TreeItem& copyTreeItem(const QModelIndex& sourceIndex, int row, const QModelIndex& parentIndex);
    /**
     * Set the value of an attribute with name key with undo support.
     * @param index The tree node index of corresponding tag node.
     * @param key The attribute key.
     * @param value The value of the attribute.
     */
    void setAttribute(const QModelIndex& index, const QString& key, const QString& value);

    /**
     * Set the value of a node with undo support.
     * @param index The tree node index of corresponding tag node.
     * @param value The value of the node.
     */
    void setNodeValue(const QModelIndex& index, const QString& value);

    /**
     * @param pathName The path name like "config/navit/gui[enabled='yes']".
     * @return The model index given by pathName or an invalid model index, if the path has not been found.
     */
    QModelIndex getModelIndexByDomPath(const QString& pathName) const;

signals:
	void dataChanged(const QModelIndex& indexTopLeft, const QModelIndex& indexBottomRight);

protected:
	void emitDataChanged(const QModelIndex& indexTopLeft, const QModelIndex& indexBottomRight);

	QDomElement createDomElement(const QString& name);
	QDomAttr createDomAttribute(const QString& name);

private:
	QDomDocument& domDocument;
	QUndoStack& undoStack;
	TreeItem rootTreeItem;
	/**
	 * Takes constant tree item, but it gives indirectly access to it.
	 */
	QModelIndex createIndexFromTreeItem(int row, int column, const TreeItem& treeItem) const;
	static QMap<QString, QString> getAttributeMap(const QString& attributes);
	/**
	 * @param node The node looking at.
	 * @param attributeMap The attribute map containing pairs of attribute name and value searching for.
	 * @return True, if the given node contains all given pairs of attribute name and value, otherwise false.
	 */
	static bool hasAttributes(const QDomNode& node, const QMap<QString, QString>& attributeMap);
};

#endif /* TREEMODEL_H_ */
