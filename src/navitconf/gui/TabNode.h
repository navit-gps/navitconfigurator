/*
 * NodeEditor.h
 *
 *  Created on: 25.11.2011
 *      Author: remo
 */

#ifndef TABNODE_H_
#define TABNODE_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QModelIndex>

#include <QtGui/QLineEdit>
#include <QtGui/QTableView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QStandardItem>
#include <QtGui/QStandardItemModel>

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
