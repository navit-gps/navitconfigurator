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

#include "BoxItemAttribute.h"

BoxItemAttribute::BoxItemAttribute(int tableRow, const QString& type, const QString& key, const QString name, const QString& trueValue, const QString falseValue)
:
	  tableRow(tableRow)
	, type(type)
	, key(key)
	, name(name)
	, trueValue(trueValue)
	, falseValue(falseValue)
	, value()
{
}

BoxItemAttribute::~BoxItemAttribute() {
}

bool BoxItemAttribute::isString() const {
	return type == "S";
}

bool BoxItemAttribute::isBool() const {
	return type == "B";
}

bool BoxItemAttribute::isInteger() const {
	return type == "I";
}

bool BoxItemAttribute::isColor() const {
	return type == "C";
}

bool BoxItemAttribute::isVirtual() const {
	return type == "V";
}

QString BoxItemAttribute::getKey() const {
	return key;
}

QString BoxItemAttribute::getName() const {
	return name;
}

void BoxItemAttribute::setString(const QString& value) {
	this->value = value;
}

QString BoxItemAttribute::getString() const {
	return value;
}

void BoxItemAttribute::setBool(bool isTrue) {
	value = isTrue ? "1" : "0";
}

bool BoxItemAttribute::getBool() const {
	return value == "1";
}

void BoxItemAttribute::setInteger(int value) {
	this->value = QString::number(value);
}

int BoxItemAttribute::getInteger() const {
	return value.toInt();
}

void BoxItemAttribute::setValueFromTreeItem(const TreeItem& treeItem) {
	value = treeItem.getAttribute(key);
}

void BoxItemAttribute::fillCellInTable(QStandardItemModel& tableModel) const {
	if (isBool()) {
		tableModel.item(tableRow)->setCheckState(getBool() ? Qt::Checked : Qt::Unchecked);
		if (!trueValue.isEmpty()) {
			tableModel.item(tableRow)->setText(getBool() ? trueValue : falseValue);
		}
	} else if (!isVirtual()) {
		tableModel.item(tableRow)->setText(getString());
	}
}

bool BoxItemAttribute::operator!=(const BoxItemAttribute& other) const {
	return value != other.value;
}

