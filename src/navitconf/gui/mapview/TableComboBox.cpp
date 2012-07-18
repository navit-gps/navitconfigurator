/*
 * TableComboBox.cpp
 *
 *  Created on: 25.06.2012
 *      Author: remo
 */

#include "TableComboBox.h"

#include <QtCore/QDebug>

TableComboBox::TableComboBox(QWidget* parent)
:
	QComboBox(parent)
{
	qDebug() << "TableComboBox::TableComboBox";
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
