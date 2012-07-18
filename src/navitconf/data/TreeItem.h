/*
 * TreeItem.h
 *
 *  Created on: 09.05.2012
 *      Author: Raimar
 */

#ifndef TREEITEM_H_
#define TREEITEM_H_

#include <QtCore/QString>
#include <QtCore/QVector>

#include <QtXml/QDomElement>

/**
 * Connect the XML node with this tree node used by the TreeModel.
 */
class TreeItem {
public:
	/**
	 * Constructor for root tree item.
	 */
	TreeItem(const QDomNode& node, int row);
	/**
	 * Constructor for child of given parent tree item.
	 */
	TreeItem(const QDomNode& node, int row, TreeItem& parent);
	/**
	 * Deletes all children recursively.
	 */
	virtual ~TreeItem();

	const QDomNode& getDomNode() const;
	QDomNode& getDomNode();

	/**
	 * @param attributeName The attribute name.
	 * @param The value of the given attribute name or an empty string if the attribute does not exist.
	 */
	QString getAttribute(const QString& attributeName) const;

	int getRow() const;
	/**
	 * @return True if this item has exactly one children, which is a text node, otherwise False.
	 */
	bool hasOnlyTextChild() const;
	bool hasParent() const;
	const TreeItem& getParent() const;
	TreeItem& getParent();
	bool hasChild(int row) const;
	const TreeItem& getChild(int row) const;
	/**
	 * @param row The 0-based row position of the child.
	 * @return The child item with the given row.
	 */
	TreeItem& getChild(int row);
	/**
	 * @param row The 0-based row position of the child to remove.
	 * @return The pointer to the TreeItem (caller must delete this object).
	 */
	TreeItem* removeChild(int row);
	void insertChild(int row, TreeItem& childItem);
	int countChilds() const;
	/**
	 * @return -1 if no other sister with this node name exists,
	 *         or the position beginning with 0 in list of sisters with the same node name.
	 */
	int getCounterInSistersWithSameName() const;
	/**
	 * @return If {@link #getCounterInSistersWithSameName()} < 0, only the node name, otherwise
	 *         the node name appended by space and {@link #getCounterInSistersWithSameName()}
	 */
	QString getNameWithCounter() const;
	/**
	 * @return The concatenated names of all node names beginning at the root node separated by '/'.
	 */
	QString getItemPath() const;
	void println() const;

private:
	QDomNode domNode;
	int row;
	TreeItem* parent;
	QVector<TreeItem*> childs;
	void init();
	void insertItemPath(QString& currentPath) const;
};

#endif /* TREEITEM_H_ */
