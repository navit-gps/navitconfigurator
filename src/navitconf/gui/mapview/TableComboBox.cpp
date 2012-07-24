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

#include "TableComboBox.h"

TableComboBox::TableComboBox(QWidget* parent)
:
	QComboBox(parent)
{
}

TableComboBox::~TableComboBox() {
}

void TableComboBox::setEditorData(const BoxItemDelegate& itemDelegate, const QModelIndex& index, const QString& text) {
	this->index = index;
	setCurrentIndex(findText(text)); // set the value if possible
	connect(this, SIGNAL(currentIndexChanged(int)),
	        this, SLOT(  emitCommitData()));
	connect(this,          SIGNAL(emitCommitData(TableComboBox&, const QModelIndex&)),
	        &itemDelegate, SLOT(  emitCommitData(TableComboBox&, const QModelIndex&)));
	showPopup(); // open the comboBox
}

void TableComboBox::hidePopup() {
	QComboBox::hidePopup();
	if (this->parentWidget()) {
		this->parentWidget()->setFocus(); // focus the changed cell in the table view to close this TableComboBox
	}
}

void TableComboBox::emitCommitData() {
	emit emitCommitData(*this, index);
}
