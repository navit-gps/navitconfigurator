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
