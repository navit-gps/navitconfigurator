/*
 * TableComboBox.h
 *
 *  Created on: 25.06.2012
 *      Author: remo
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
