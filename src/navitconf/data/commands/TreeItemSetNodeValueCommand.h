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

#ifndef TREEITEMSETNODEVALUECOMMAND_H_
#define TREEITEMSETNODEVALUECOMMAND_H_

#include <QtCore/QModelIndex>
#include <QtCore/QString>

#include <QtGui/QUndoCommand>

/** forward declaration */
class TreeModel;

/**
 * Provides an undo command to set the content value in a TreeItem of a TreeModel.
 */
class TreeItemSetNodeValueCommand : public QUndoCommand {
public:
	explicit TreeItemSetNodeValueCommand(TreeModel& model, const QModelIndex& index, const QString& newValue);
	virtual ~TreeItemSetNodeValueCommand();
    void redo();
    void undo();

private:
    TreeModel& model;
    QModelIndex index;
    QString otherValue;
};

#endif /* TREEITEMSETNODEVALUECOMMAND_H_ */
