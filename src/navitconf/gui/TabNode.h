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

#ifndef TABNODE_H_
#define TABNODE_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QModelIndex>

#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTableView>
#include <QtWidgets/QPlainTextEdit>

#include "../data/TreeItem.h"
#include "../data/TreeModel.h"

/**
 * Connects the label of a path, the table of attributes and a text field of textual representation with each other.
 */
class TabNode : public QObject {
	Q_OBJECT
public:
	explicit TabNode();
	virtual ~TabNode();
	void setupUi(QTabWidget* tabWidgetMain, QTabWidget* tab, QLineEdit* txfPath, QTableView* tbvAttributes, QPlainTextEdit* tbxComment, QPlainTextEdit* tbxSource);
	void setTreeModel(TreeModel& treeModel);
	/**
	 * Used by {@link navitconf/file/Document#onSelectionChanged(QItemSelection)} and informs of the current selected tree item
	 * to refresh the associated attributes in the table.
	 * @param The model index.
	 */
	void setSelectedItem(const QModelIndex& index);

public slots:
	/**
	 * Called if Value of attribute in table has been changed.
	 */
	void onDataChanged(const QModelIndex& indexTopLeft, const QModelIndex& indexRightBottom);

private:
	QModelIndex selectedTreeIndex;
	QStandardItemModel* attributesModel;
	TreeModel* treeModel;
	QTabWidget* tabWidgetMain;
	QTabWidget* tab;
	QLineEdit* txfPath;
	QTableView* tbvAttributes;
	QPlainTextEdit* tbxComment;
	QPlainTextEdit* tbxSource;
	QStandardItem* createReadOnlyItem(QString name) const;
	QStandardItem* createYesNoItem(const QString& name) const;
	void clearTableView();
};

#endif /* TABNODE_H_ */
