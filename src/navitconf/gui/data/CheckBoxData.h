/*
 * BoolCheckBoxData.h
 *
 *  Created on: 17.06.2012
 *      Author: remo
 */

#ifndef CHECKBOXDATA_H_
#define CHECKBOXDATA_H_

#include <QtCore/QString>
#include <QtGui/QCheckBox>

#include <navitconf/data/TreeModel.h>

/**
 * Connection between a check box and an attribute with a bool value for initializing from
 * and storing into a TreeModel.
 */
class CheckBoxData {
public:
	/**
	 * @param treeModel The tree model.
	 * @parem path the path describint the node in the tree model.
	 * @param attribute The attribute name for the check box state.
	 *        Only "0" or "false" represents false, otherwise true.
	 * @param isTrueDefault If true, the default value for a non existing attribute is the true state, otherwise the false state.
	 * @param checkBox The check box widget.
	 */
	CheckBoxData(TreeModel& treeModel, const QString& path, const QString& attribute, bool isTrueDefault, QCheckBox* checkBox);
	virtual ~CheckBoxData();
	/**
	 * Stores a changed check box state into the tree model with undo support.
	 */
	void storeChanges();

private:
	TreeModel* treeModel;
	QString path;
	QString attribute;
	bool _isTrueDefault;
	QCheckBox* checkBox; // pointer needed, because copying QCheckBox is disabled!!!
};

#endif /* CHECKBOXDATA_H_ */
