#include "DialogGeneralSettings.h"

#include <QtCore/QDebug>
#include <QtCore/QModelIndex>
#include <QtCore/QStringList>

#include <QtXml/QDomNode>

#include <QtGui/QMessageBox>
#include <QtGui/QComboBox>

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
