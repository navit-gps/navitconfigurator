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

#include "DialogGeneralSettings.h"

#include <QtCore/QModelIndex>
#include <QtCore/QStringList>

#include <QtXml/QDomNode>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QComboBox>

#include <navitconf/file/Settings.h>
#include <navitconf/data/TreeItem.h>

DialogGeneralSettings::DialogGeneralSettings(QWidget& parent, TreeModel& treeModel)
:
	QDialog(&parent),
	ui(),
	treeModel(treeModel),
	bools(),
	combos()
{
	ui.setupUi(this);
	bools.append(CheckBoxData(
			treeModel,
			"config/navit/gui[type=internal]",
			"keyboard",
			true,
			ui.checkBoxInternalKeyboard));
	QStringList vehicleIds;
	vehicleIds.append("car");
	vehicleIds.append("car_shortest");
	vehicleIds.append("car_avoid_tolls");
	vehicleIds.append("bike");
	vehicleIds.append("pedestrian");
	vehicleIds.append("horse");
	vehicleIds.append("Truck");
	combos.append(ComboBoxData(
			treeModel,
			"config/navit/vehicle[enabled=yes]",
			"profilename",
			*ui.comboBoxDefaultVehicle,
			vehicleIds
			));
	exec(); // show dialog
}

DialogGeneralSettings::~DialogGeneralSettings() {
}

void DialogGeneralSettings::accept() {
	for (int i = 0; i < bools.size(); i++) {
		try {
			bools[i].storeChanges();
		} catch(QString& e) {
			QMessageBox::critical(this, tr("Error"), e);
		}
	}
	for (int i = 0; i < combos.size(); i++) {
		combos[i].save();
	}
	setVisible(false); // close dialog
}
