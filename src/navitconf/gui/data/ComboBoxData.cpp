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
