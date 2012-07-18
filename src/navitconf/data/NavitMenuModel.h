/*
 * NavitMenuModel.h
 *
 *  Created on: 09.06.2012
 *      Author: remo
 */

#ifndef NAVITMENUMODEL_H_
#define NAVITMENUMODEL_H_

#include <QtGui/QStandardItemModel>

/**
 * Extends the QStandardItemModel to represent a data model for NavitMenuItem.
 * Overrides only the method {@link #flags(const QModelIndex&)} for valid drag and drop support.
 */
class NavitMenuModel: public QStandardItemModel {
public:
	NavitMenuModel();
	virtual ~NavitMenuModel();

	/**
	 * @param index The index for giving the flags.
	 * @return Same result as the parent class QStandardItemModel gives.
	 *         Only drop before or after root item is not allowed.
	 *         Drop onto the root item is allowed if the inherited class
	 *         QStandardItemModel allow it. This is defined by each item itself.
	 */
    Qt::ItemFlags flags(const QModelIndex &index) const;
};

#endif /* NAVITMENUMODEL_H_ */
