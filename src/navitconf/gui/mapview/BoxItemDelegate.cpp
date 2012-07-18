/*
 * BoxItemDelegate.cpp
 *
 *  Created on: 24.06.2012
 *      Author: remo
 */

#include "BoxItemDelegate.h"

#include <cstddef>

#include <QtCore/QDebug>
#include <QtCore/QMetaProperty>

#include <QtGui/QColorDialog>

#include "TableComboBox.h"
#include "BoxGraphicsItem.h"

BoxItemDelegate::BoxItemDelegate(QAbstractItemModel& model, QTableView& parent)
:
	QStyledItemDelegate(&parent)
	, model(model)
	, typeNamesMap()
	, resultData(new QString())
{
	typeNamesMap.insert("auxmap",                        tr("Auxiliary Map"));
	typeNamesMap.insert("button",                        tr("Button"));
	typeNamesMap.insert("cmd_interface",                 tr("Command Interface"));
	typeNamesMap.insert("compass",                       tr("Compass"));
	typeNamesMap.insert("gps_status",                    tr("GPS Status"));
	typeNamesMap.insert("image",                         tr("Image"));
	typeNamesMap.insert("navigation",                    tr("Navigation"));
	typeNamesMap.insert("navigation_distance_to_next",   tr("Navigation Distance to Next"));
	typeNamesMap.insert("navigation_distance_to_target", tr("Navigation Distance to Target"));
	typeNamesMap.insert("navigation_next_turn",          tr("Navigation Next Turn"));
	typeNamesMap.insert("odometer",                      tr("Odometer"));
	typeNamesMap.insert("route_guard",                   tr("Route Guard"));
	typeNamesMap.insert("scale",                         tr("Scale"));
	typeNamesMap.insert("speed_cam",                     tr("Speed Camera"));
	typeNamesMap.insert("speed_warner",                  tr("Speed Warner"));
	typeNamesMap.insert("stopwatch",                     tr("Stopwatch"));
	typeNamesMap.insert("text",                          tr("Text"));
	typeNamesMap.insert("toggle_announcer",              tr("Toggle Announcer"));
	typeNamesMap.insert("volume",                        tr("Volume"));
}

BoxItemDelegate::~BoxItemDelegate() {
	delete resultData;
}

QWidget* BoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	switch (index.row()) {
		case BoxGraphicsItem::OSD_TYPE_NAME: {
			TableComboBox& comboBox = *new TableComboBox(parent);
			QMap<QString, QString>::const_iterator it = typeNamesMap.begin();
			while (it != typeNamesMap.end()) {
				comboBox.addItem(it.value(), it.key());
				it++;
			}
			return &comboBox;
		}
		case BoxGraphicsItem::OSD_TEXT_COLOR:
		case BoxGraphicsItem::OSD_BACKGROUND_COLOR: {
			QString colorString = index.model()->data(index).toString();
			if (colorString.isEmpty()) {
				if (index.row() == BoxGraphicsItem::OSD_TEXT_COLOR) {
					colorString = "#ffffff";
				} else {
					colorString = "#00000060";
				}
			}
			QColor c = QColor(colorString.mid(0, 6));
			QColorDialog::ColorDialogOptions options = 0;
			if (index.row() == BoxGraphicsItem::OSD_BACKGROUND_COLOR) {
				options = QColorDialog::ShowAlphaChannel;
				c.setAlpha(colorString.mid(7).toInt(0, 16));
			}
			// show color dialog
			QColor color = QColorDialog::getColor(c, parent, tr("Color Chooser"), options);
			if (color.isValid()) {
				QString colorString = color.name();
				if (index.row() == BoxGraphicsItem::OSD_BACKGROUND_COLOR) {
					colorString.append(QString::number(color.alpha(), 16));
				}
				model.setData(index, QVariant::fromValue(colorString));
			}
			return NULL;
		}
	}
	return QStyledItemDelegate::createEditor(parent, option, index);
}

void BoxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
	switch (index.row()) {
		case BoxGraphicsItem::OSD_TYPE_NAME: {
			QString text = typeNamesMap[index.model()->data(index).toString()];
			TableComboBox& comboBox = *dynamic_cast<TableComboBox*>(editor);
			comboBox.setEditorData(*this, index, text);
			break;
		}
		case BoxGraphicsItem::OSD_TEXT_COLOR:
		case BoxGraphicsItem::OSD_BACKGROUND_COLOR:
			break;
		default:
			QStyledItemDelegate::setEditorData(editor, index);
			break;
	}
}

void BoxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
	QString newData;
	switch (index.row()) {
		case BoxGraphicsItem::OSD_TYPE_NAME: {
			TableComboBox& comboBox = *dynamic_cast<TableComboBox*>(editor);
			newData = comboBox.itemData(comboBox.currentIndex()).toString();
			break;
		}
		case BoxGraphicsItem::OSD_TEXT_COLOR:
		case BoxGraphicsItem::OSD_BACKGROUND_COLOR: {
			qDebug() << "BoxItemDelegate::setModelData()";
			return;
		}
		default:
			QStyledItemDelegate::setModelData(editor, model, index);
			return;
	}
	model->setData(index, QVariant::fromValue(newData));
}

void BoxItemDelegate::emitCommitData(TableComboBox& comboBox, const QModelIndex& index) {
	setModelData(&comboBox, &model, index);
}
