/*
 * NavitMenuItem.h
 *
 *  Created on: 09.06.2012
 *      Author: remo
 */

#ifndef NAVITMENUITEM_H_
#define NAVITMENUITEM_H_

#include <QtCore/QString>

#include <QtGui/QStandardItem>

#include <QtXml/QDomNode>

/**
 * Extends the QStandardItem to represent the data of a navit menu item used in a NavitMenuModel.
 */
class NavitMenuItem: public QStandardItem {
public:
	/**
	 * Create the root menu item.
	 */
	NavitMenuItem();
	/**
	 * Create a sub menu item.
	 * @param node The XML node associated with this menu item
	 * @param parent The paren menu item.
	 */
	NavitMenuItem(QDomNode& node, const QString& iconFolder, NavitMenuItem& parent);
	virtual ~NavitMenuItem();
	/**
	 * @param node The XML node associated with this menu item.
	 *             If this is link the text is blue.
	 *             Drop is disabled for leaf menu items.
	 */
	void setDomNode(QDomNode& node, const QString& iconFolder);
	/**
	 * @return The XML node associated with this menu item.
	 */
	QDomNode& getDomNode() const;
	/**
	 * @return The name of this menu item.
	 */
	QString getText() const;
	/**
	 * @return The id of this menu item, i.e. the name-attribute of the a-tag.
	 */
	const QString& getId() const;
	/**
	 * @return The link target of this menu item. If the node is an a-tag, the href-attribute is used,
	 *         otherwise an empty string is returned.
	 */
	const QString& getLinkTarget() const;
	/**
	 * @param node The XML node of the new child for this menu item.
	 * @return The appended menu item.
	 */
	NavitMenuItem& addChild(QDomNode& node, const QString& iconFolder);
	/**
	 * @param row The child position.
	 * @return The menu item child of the given position.
	 */
	NavitMenuItem& getChild(int row);
	/**
	 * @param row The child position.
	 * @return The menu item child of the given position.
	 */
	const NavitMenuItem& getChild(int row) const;

private:
	QString id;
	QString text;
	QString linkTarget;
};

#endif /* NAVITMENUITEM_H_ */
