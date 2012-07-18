/*
 * BoolComboBoxData.h
 *
 *  Created on: 17.06.2012
 *      Author: remo
 */

#ifndef COMBOBOXDATA_H_
#define COMBOBOXDATA_H_

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtGui/QComboBox>

#include <navitconf/data/TreeModel.h>

/**
 * Connection between a combo box and an attribute for initializing with a
 * list of strings and storing changes back to a TreeModel.
 */
class ComboBoxData {
public:
	/**
	 * @param treeModel The tree data model.
	 * @param path The path in the tree data model to the value of this ComboBox.
	 * @param attribute The attribute of the node with the given path containing the value.
	 * @param comboBox The ComboBox for selecting the data.
	 *        It is normally created with the Qt-Designer and filled with a visible list of strings.
	 * @param dataList The hidden list of data. Should be as long as the visible list of strings.
	 *        If not given, the implicit list is ignored.
	 */
	ComboBoxData(TreeModel& treeModel, const QString& path, const QString& attribute,
			QComboBox& comboBox, const QStringList& dataList = QStringList());
	virtual ~ComboBoxData();
	/**
	 * Stores the value of the ComboBox in the tree data model, if there are changes.
	 */
	void save();

private:
	TreeModel* treeModel;
	QString path;
	QString attribute;
	QComboBox* comboBox; // pointer needed, because copy of QComboBox is disabled!!!
};

#endif /* COMBOBOXDATA_H_ */
