/*
 * BoolComboBoxData.cpp
 *
 *  Created on: 17.06.2012
 *      Author: remo
 */

#include "ComboBoxData.h"

ComboBoxData::ComboBoxData(TreeModel& treeModel, const QString& path, const QString& attribute,
		QComboBox& comboBox, const QStringList& dataList)
:
	treeModel(&treeModel),
	path(path),
	attribute(attribute),
	comboBox(&comboBox)
{
	QModelIndex index = treeModel.getModelIndexByDomPath(path);
	QString value;
	if (index.isValid()) {
		value = treeModel.getTreeItem(index).getAttribute(attribute);
	}
	int position = -1;
	for (int i = 0; i < dataList.size(); i++) {
		comboBox.setItemData(i, QVariant(dataList[i]));
		if (position < 0 && dataList[i] == value) {
			position = i;
		}
	}
	if (position >= 0) {
		comboBox.setCurrentIndex(position);
	}
}

ComboBoxData::~ComboBoxData() {
}

void ComboBoxData::save() {
	QModelIndex index = treeModel->getModelIndexByDomPath(path);
	if (index.isValid()) {
		QString valueBefore = treeModel->getTreeItem(index).getAttribute(attribute);
		QString valueAfter = comboBox->itemData(comboBox->currentIndex()).toString();
		if (valueBefore != valueAfter) {
			treeModel->setAttribute(index, attribute, valueAfter);
		}
	} else {
		throw QString("Path ") + path + " has not been found!";
	}
}
