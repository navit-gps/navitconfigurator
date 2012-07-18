/*
 * BoxItemDelegate.h
 *
 *  Created on: 24.06.2012
 *      Author: remo
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
