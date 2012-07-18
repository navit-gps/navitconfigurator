/*
 * BoxItemProperty.h
 *
 *  Created on: 01.07.2012
 *      Author: remo
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
