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

#include "TreeItemSetNodeValueCommand.h"

/** forward declaration */
#include <navitconf/data/TreeModel.h>

TreeItemSetNodeValueCommand::TreeItemSetNodeValueCommand(TreeModel& model, const QModelIndex& index, const QString& newValue)
:
	QUndoCommand(),
	model(model),
	index(index),
	otherValue(newValue)
{
// redo() is called by adding an instance of this class to the undo stack
}

TreeItemSetNodeValueCommand::~TreeItemSetNodeValueCommand() {
}

void TreeItemSetNodeValueCommand::redo() {
	QDomNode& node = model.getTreeItem(index).getDomNode();
	QString tmpValue(node.nodeValue());
	node.setNodeValue(otherValue);
	otherValue = tmpValue;
	model.emitDataChanged(index, index.sibling(index.row(), 1));
}

void TreeItemSetNodeValueCommand::undo() {
	redo();
}
