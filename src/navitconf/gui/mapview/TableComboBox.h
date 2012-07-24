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

#ifndef TABLECOMBOBOX_H_
#define TABLECOMBOBOX_H_

#include <QtCore/QMetaType>

#include <QtGui/QComboBox>
#include <QtGui/QWidget>

#include "BoxItemDelegate.h"

/**
 * Extends the normal QComboBox used in a QTableView to modify the default behavior.
 */
class TableComboBox: public QComboBox {
Q_OBJECT
public:
	TableComboBox(QWidget* parent = 0);
	virtual ~TableComboBox();
	void setEditorData(const BoxItemDelegate& itemDelegate, const QModelIndex& index, const QString& text);
	void hidePopup();
signals:
	void emitCommitData(TableComboBox& comboBox, const QModelIndex& index);
public slots:
	void emitCommitData();
private:
	QModelIndex index;
};

#endif /* TABLECOMBOBOX_H_ */
