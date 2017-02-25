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

#ifndef DIALOGMANAGENAVITMENU_H
#define DIALOGMANAGENAVITMENU_H

#include <QtCore/QString>
#include <QtCore/QList>

#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>

#include <QtXml/QDomNode>

#include "ui_DialogManageNavitMenu.h"

#include <navitconf/data/TreeModel.h>
#include <navitconf/data/TreeItem.h>
#include <navitconf/data/NavitMenuModel.h>
#include <navitconf/data/NavitMenuItem.h>

/**
 * Dialog for reordering the Navit menu items.
 */
class DialogManageNavitMenu : public QDialog
{
    Q_OBJECT

public:
	/**
	 * Show the dialog to manage the menu items. Pressing the cancel button closes the dialog
	 * and ignores changes of the menu items.
	 * @param parent The parent window for a modal view.
	 * @param treeModel The tree model for retrieving and storing the changed menu items.
	 */
	explicit DialogManageNavitMenu(QWidget& parent, TreeModel& treeModel);
	virtual ~DialogManageNavitMenu();

public:
	/**
	 * Called by pressing the OK button. Store the changed menu items in the given tree model and close the dialog.
	 */
	void accept();

private:
	static const char* const MENU_DATA_PATH;
	/** The UI for this dialog */
    Ui::DialogManageNavitMenu ui;
    /** The tree model storing the XML file */
	TreeModel& treeModel;
	/** The model index of the tree model that stores the menu data */
	QModelIndex treeItemIndex;
    /** The inernal XML document storing the menu CDATA. */
    QDomDocument menuDocument;
    /** The Node list of all XML nodes for referencing in the NavitMenuItem class. */
    QList<QDomNode> nodeList;
    /** The data model storing NavitMenuItem used in the QTreeView. */
    NavitMenuModel model;
    /** The root menu item used in the QTreeView. */
    NavitMenuItem rootItem;
    /** The folder containing all menu icons */
    QString iconFolder;
    /**
     * Creates the internal XML document based on the given menu CDATA.
     */
	void init();
	/**
	 * Search for the menu data tree item. Stores it in treeItem. If it does not exist, the root tree item is stored.
	 */
	void findTreeItem();
	/**
	 * Fills the QTreeView with the internal XML document.
	 */
    void fillDataModel();
    /**
     * @param The XML node to add.
     * @return The reference to the new created XML node.
     */
    QDomNode& addToNodeList(const QDomNode& node);
    /**
     * @param The node to associated with the new menu item.
     * @return The reference to the new created menu item.
     */
    NavitMenuItem& appendMenuToDataModel(QDomNode& node);
    /**
     * @param item The current menu item in the recursive loop.
     * @param searchItem The menu item searching for.
     * @return The menu item with the link target same as the id defined by searchItem,
     *         or the root item, if no such menu item exists.
     */
    NavitMenuItem& findMenu(NavitMenuItem& item, const NavitMenuItem& searchItem);

    QString getHtmlStringFromDataModel() const;

    void appendMenuToDomDocument(const NavitMenuItem& itemParent, QDomDocument& newDocumet) const;

};

#endif // DIALOGMANAGENAVITMENU_H
