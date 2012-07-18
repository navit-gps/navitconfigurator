/*
 * NavitMenuModel.cpp
 *
 *  Created on: 09.06.2012
 *      Author: remo
 */

#include "NavitMenuModel.h"

#include <cstddef> // NULL

#include <QDebug>

NavitMenuModel::NavitMenuModel()
{
}

NavitMenuModel::~NavitMenuModel() {
}

Qt::ItemFlags NavitMenuModel::flags(const QModelIndex& index) const {
	Qt::ItemFlags f = QStandardItemModel::flags(index);
	if (f & Qt::ItemIsDropEnabled) {
		if (!index.isValid()) {
			f ^= Qt::ItemIsDropEnabled; // disable drop before or after main menu
		}
	}
	return f;
}
