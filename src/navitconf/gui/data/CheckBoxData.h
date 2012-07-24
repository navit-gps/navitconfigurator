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
