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

#ifndef DIALOGGENERALSETTINGS_H
#define DIALOGGENERALSETTINGS_H

#include <QtGui/QWidget>
#include <QtGui/QDialog>
#include <QtGui/QCheckBox>

#include "ui_DialogGeneralSettings.h"

#include <navitconf/data/TreeModel.h>
#include <navitconf/gui/data/CheckBoxData.h>
#include <navitconf/gui/data/ComboBoxData.h>

/**
 * Dialog for the general Navit settings.
 */
class DialogGeneralSettings : public QDialog
{
    Q_OBJECT

public:
	/**
	 * Show the dialog to manage the menu items. Pressing the cancel button closes the dialog
	 * and ignores changes of the menu items.
	 * @param parent The parent window for a modal view.
	 * @param treeModel The tree model for retrieving and storing the changed menu items.
	 */
	explicit DialogGeneralSettings(QWidget& parent, TreeModel& treeModel);
	virtual ~DialogGeneralSettings();

public:
	/**
	 * Called by pressing the OK button. Store the changed menu items in the given tree model and close the dialog.
	 */
	void accept();

private:
	/** The UI for this dialog */
	Ui::DialogGeneralSettings ui;
	/** The tree model storing the XML file */
	TreeModel& treeModel;
	QList<CheckBoxData> bools;
	QList<ComboBoxData> combos;
	void fillComboBox(QComboBox& comboBox, const QString& path, const QString& attribute, const QStringList& dataList);
};

#endif // DIALOGGENERALSETTINGS_H
