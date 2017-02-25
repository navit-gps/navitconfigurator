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

#ifndef TREEITEMRENAMECOMMAND_H_
#define TREEITEMRENAMECOMMAND_H_

#include <QtCore/QModelIndex>
#include <QtCore/QVariant>
#include <QtCore/QString>

#include <QtWidgets/QUndoCommand>

/** forward declaration */
class TreeModel;

/**
 * Provides an undo command to rename a TreeItem of a TreeModel.
 */
class TreeItemRenameCommand : public QUndoCommand {
public:
	explicit TreeItemRenameCommand(TreeModel& model, const QModelIndex& index, const QString& newTagName);
	virtual ~TreeItemRenameCommand();
    void redo();
    void undo();

private:
    TreeModel& model;
    QModelIndex index;
    QString otherTagName;
};

#endif /* TREEITEMRENAMECOMMAND_H_ */
