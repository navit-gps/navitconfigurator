/*
 * BoolCheckBoxData.cpp
 *
 *  Created on: 17.06.2012
 *      Author: remo
 */

#include "CheckBoxData.h"

CheckBoxData::CheckBoxData(TreeModel& treeModel, const QString& path, const QString& attribute, bool isTrueDefault, QCheckBox* checkBox)
:
	treeModel(&treeModel),
	path(path),
	attribute(attribute),
	_isTrueDefault(isTrueDefault),
	checkBox(checkBox)
{
	QModelIndex index = treeModel.getModelIndexByDomPath(path);
	bool isTrue;
	if (index.isValid()) {
		QString value = treeModel.getTreeItem(index).getAttribute(attribute);
		if (value.isEmpty()) {
			isTrue = _isTrueDefault;
		} else {
			isTrue = value != "false" && value != "0";
		}
	} else {
		isTrue = _isTrueDefault;
	}
	checkBox->setChecked(isTrue);
}

CheckBoxData::~CheckBoxData() {
}

void CheckBoxData::storeChanges() {
	QModelIndex index = treeModel->getModelIndexByDomPath(path);
	if (index.isValid()) {
		QString valueBefore = treeModel->getTreeItem(index).getAttribute(attribute);
		bool isTrue = checkBox->isChecked();
		QString valueAfter;
		if (isTrue != _isTrueDefault) {
			valueAfter = (isTrue ? "1" : "0");
		}
		if (valueBefore != valueAfter) {
			treeModel->setAttribute(index, attribute, valueAfter);
		}
	} else {
		throw QString("Path ") + path + " has not been found!";
	}
}
