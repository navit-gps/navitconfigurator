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

#ifndef BOXITEMDELEGATE_H_
#define BOXITEMDELEGATE_H_

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QModelIndex>

#include <QtGui/QWidget>
#include <QtGui/QItemDelegate>
#include <QtGui/QStyleOptionViewItem>
#include <QtGui/QStyledItemDelegate>
#include <QtGui/QComboBox>
#include <QtGui/QTableView>

class TableComboBox;

/**
 * Delegate class for a combo box widget shown for selecting the type name.
 */
class BoxItemDelegate: public QStyledItemDelegate {
Q_OBJECT
public:
	/**
	 * @param model The model of the table view.
	 * @param parent The parent widget, i. e. the QTableView.
	 */
	explicit BoxItemDelegate(QAbstractItemModel& model, QTableView& parent);
	virtual ~BoxItemDelegate();

	/**
	 * @return The widget for editing the given index, e.g.
	 * <ol>
	 *  <li>QComboBox for the possible type names,</li>
	 *  <li>color chooser for foreground and background color.</li>
	 * </ol>
	 */
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
	/**
	 * Set the data in the delegated widgets.
	 */
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
	/**
	 * Set the type name to the data model.
	 */
	void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
public slots:
	void emitCommitData(TableComboBox& comboBox, const QModelIndex& index);
private:
	QAbstractItemModel& model;
	QMap<QString, QString> typeNamesMap;
	/** The result given by the user into the editor widget */
	QString* resultData;
};

#endif /* BOXITEMDELEGATE_H_ */
