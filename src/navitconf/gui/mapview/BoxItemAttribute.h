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

#ifndef BOXITEMATTRIBUTE_H_
#define BOXITEMATTRIBUTE_H_

#include <navitconf/data/TreeItem.h>

#include <QtGui/QStandardItemModel>

/**
 * Data class for an attribute of a box graphics item used in the map view and the corresponding table view.
 */
class BoxItemAttribute {
public:
	BoxItemAttribute(int tableRow, const QString& type, const QString& key, const QString name, const QString& trueValue, const QString falseValue);
	virtual ~BoxItemAttribute();
	bool isString() const;
	bool isBool() const;
	bool isInteger() const;
	bool isColor() const;
	bool isVirtual() const;
	QString getKey() const;
	QString getName() const;
	void setString(const QString& value);
	QString getString() const;
	void setBool(bool isTrue);
	bool getBool() const;
	void setInteger(int value);
	int getInteger() const;
	void setValueFromTreeItem(const TreeItem& treeItem);
	void fillCellInTable(QStandardItemModel& tableModel) const;
	bool operator!=(const BoxItemAttribute& other) const;
private:
	int tableRow;
	QString type;
	QString key;
	QString name;
	QString trueValue;
	QString falseValue;
	QString value;
};

#endif /* BOXITEMATTRIBUTE_H_ */
