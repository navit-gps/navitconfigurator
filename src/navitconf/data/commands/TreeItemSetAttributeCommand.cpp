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

#include "TreeItemSetAttributeCommand.h"

/** forward declaration */
#include <navitconf/data/TreeModel.h>

TreeItemSetAttributeCommand::TreeItemSetAttributeCommand(TreeModel& model, const QModelIndex& index, const QString& key, const QString& newValue)
:
	QUndoCommand(),
	model(model),
	index(index),
	key(key),
	otherValue(newValue)
{
// redo() is called by adding an instance of this class to the undo stack
}

TreeItemSetAttributeCommand::~TreeItemSetAttributeCommand() {
}

void TreeItemSetAttributeCommand::redo() {
	QDomNode& node = model.getTreeItem(index).getDomNode();
	QString tmpValue;
	if (otherValue.isEmpty()) { // delete attribute
		if (node.attributes().contains(key)) { // attribute exists
			tmpValue = node.attributes().namedItem(key).nodeValue();
			node.attributes().removeNamedItem(key);
		}
	} else { // set attribute
		if (node.attributes().contains(key)) { // rename
			tmpValue = node.attributes().namedItem(key).nodeValue();
		} else { // add new attribute
			node.attributes().setNamedItem(model.createDomAttribute(key));
		}
		node.attributes().namedItem(key).setNodeValue(otherValue);
	}
	otherValue = tmpValue;
	model.emitDataChanged(index.sibling(index.row(), 0), index.sibling(index.row(), 1));
}

void TreeItemSetAttributeCommand::undo() {
	redo();
}
