/*
 * NodeEditor.cpp
 *
 *  Created on: 25.11.2011
 *      Author: remo
 */

#include "TabNode.h"

#include <cstddef>

#include <QtCore/QTextStream>
#include <QtGui/QHeaderView>
#include <QtXml/QDomNode>

#include <navitconf/data/TreeItem.h>
#include <navitconf/data/TreeModel.h>

TabNode::TabNode()
:
	QObject(),
	selectedTreeIndex(),
	attributesModel(NULL),
	treeModel(NULL),
	tab(NULL),
	txfPath(NULL),
	tbvAttributes(NULL),
	tbxComment(NULL),
	tbxSource(NULL)
{
}

TabNode::~TabNode() {
	if (tab != NULL) {
		disconnect(attributesModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
				   this,             SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
	}
	if (attributesModel != NULL) {
		delete attributesModel;
	}
}

void TabNode::setTreeModel(TreeModel& treeModel) {
	this->treeModel = &treeModel;
}

void TabNode::setupUi(QTabWidget* tabWidgetMain, QTabWidget* tab, QLineEdit* txfPath, QTableView* tbvAttributes, QPlainTextEdit* tbxComment, QPlainTextEdit* tbxSource) {
	this->tabWidgetMain = tabWidgetMain;
	this->tab = tab;
	this->txfPath = txfPath;
	this->tbvAttributes = tbvAttributes;
	this->tbxComment = tbxComment;
	this->tbxSource = tbxSource;

	// resize first column automatically
	tbvAttributes->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	// select only one row
	tbvAttributes->setSelectionMode(QAbstractItemView::SingleSelection);
	tbvAttributes->setSelectionBehavior(QAbstractItemView::SelectRows);
	// TODO: enable drag and drop in table view (does not work yet)
	tbvAttributes->setDragEnabled(true);
	tbvAttributes->setDefaultDropAction(Qt::MoveAction);
	// table data model for attributes
	attributesModel = new QStandardItemModel(0, 2, this);
	// set column names
	attributesModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Attribute")));
	attributesModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Value")));
	// set data model to table view
	tbvAttributes->setModel(attributesModel);

	// each tab shows 2 spaces
	tbxComment->setTabStopWidth(2);

	// connect after initialization
	connect(attributesModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
	        this,            SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
}

void TabNode::setSelectedItem(const QModelIndex& index) {
	tabWidgetMain->setCurrentIndex(0);
	selectedTreeIndex = QModelIndex(index);
	const TreeItem& item = treeModel->getTreeItem(index);
	txfPath->setText(item.getItemPath());
	clearTableView();
	tbxComment->clear();
	const QDomNode& node = item.getDomNode();
	if (node.isComment() || node.isCDATASection()) { // comment or data node
		tab->setTabEnabled(0, false);
		tab->setTabEnabled(1, true);
		tab->setCurrentIndex(1);
		tbxComment->setPlainText(node.nodeValue().trimmed());
		tbxComment->setEnabled(true);
	} else { // node with attributes
		tab->setTabEnabled(0, true);
		tab->setTabEnabled(1, false);
		tab->setCurrentIndex(0);
		for (int i=0; i < node.attributes().count(); i++) {
			QList<QStandardItem*> list;
			QString key = node.attributes().item(i).nodeName();
			list.append(createReadOnlyItem(key));
			if (key == "enabled") {
				list.append(createYesNoItem(node.attributes().item(i).nodeValue()));
			} else {
				list.append(new QStandardItem(node.attributes().item(i).nodeValue()));
			}
			attributesModel->appendRow(list);
		}
	}
	tbxSource->clear();
	QString text;
	QTextStream stream(&text);
	node.save(stream, 2, QDomNode::EncodingFromTextStream); // EncodingFromTextStream faster than EncodingFromDocument
	//tbxSource->appendHtml("<i>");
	tbxSource->appendPlainText(text);
}

QStandardItem* TabNode::createReadOnlyItem(QString name) const {
	QStandardItem* item = new QStandardItem(name);
	item->setEditable(false);
	return item;
}

QStandardItem* TabNode::createYesNoItem(const QString& name) const {
	QStandardItem* item = new QStandardItem();
	item->setCheckable(true);
	item->setEditable(false);
	if (name == "yes") {
		item->setCheckState(Qt::Checked);
	}
	return item;
}

void TabNode::clearTableView() {
	attributesModel->removeRows(0, attributesModel->rowCount());
}

void TabNode::onDataChanged(const QModelIndex& indexTopLeft, const QModelIndex& /*indexRightBottom */) {
//	qDebug("TabNode::onDataChanged...");
	// value of attribute has been changed by user in table view
	QString key = attributesModel->data(indexTopLeft.sibling(indexTopLeft.row(), 0)).toString();
	QString value;
	if (key == "enabled") {
		value = Qt::CheckState(qvariant_cast<int>(indexTopLeft.data(Qt::CheckStateRole))) == Qt::Checked ? "yes" : "no";
	} else {
		value = indexTopLeft.data().toString();
	}
	treeModel->setAttribute(selectedTreeIndex, key, value);
}
