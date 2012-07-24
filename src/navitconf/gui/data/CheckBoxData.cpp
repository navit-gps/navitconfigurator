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
