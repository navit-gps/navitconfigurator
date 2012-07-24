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

#include "NavitMenuItem.h"

#include <QtCore/Qt>
#include <QtCore/QVariant>

NavitMenuItem::NavitMenuItem()
:
	QStandardItem(),
	id(),
	linkTarget()
{
	setText("");
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled);
	setData(Qt::blue, Qt::TextColorRole);
}

NavitMenuItem::NavitMenuItem(QDomNode& node, const QString& iconFolder, NavitMenuItem& parent)
:
	QStandardItem(),
	id(),
	linkTarget()
{
	setDomNode(node, iconFolder);
	if (linkTarget.isEmpty()) {
		// Do not allow dropping onto actions to prevent creating childs on it.
		setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
	} else {
		setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
		setData(Qt::blue, Qt::TextColorRole);
	}
	parent.appendRow(reinterpret_cast<QStandardItem*>(this));
}

NavitMenuItem::~NavitMenuItem() {
}

void NavitMenuItem::setDomNode(QDomNode& node, const QString& iconFolder) {
	setData(QVariant::fromValue(reinterpret_cast<quintptr>(&node)));
	QDomNode tmp = node;
	while (!tmp.isNull() && !tmp.isText()) {
		tmp = tmp.firstChild();
	}
	text = tmp.nodeValue();
	setText(text);
	tmp = node.attributes().namedItem("name");
	if (!tmp.isNull()) {
		id = tmp.nodeValue();
	}
	tmp = node.attributes().namedItem("href");
	if (!tmp.isNull()) {
		linkTarget = tmp.nodeValue().remove(0, 1); // delete # at the beginning
	}
	QString iconName;
	if (node.attributes().contains("src")) {
		iconName = node.attributes().namedItem("src").nodeValue();
	} else if (node.firstChild().attributes().contains("src")) {
		iconName = node.firstChild().attributes().namedItem("src").nodeValue();
	}
	if (!iconName.isEmpty()) {
		setIcon(QIcon(iconFolder + "/res/drawable/" + iconName + "_16_16.png"));
	}
}

QDomNode& NavitMenuItem::getDomNode() const {
	return *reinterpret_cast<QDomNode*>(data().value<quintptr>());
}

QString NavitMenuItem::getText() const {
	return text;
}

const QString& NavitMenuItem::getId() const {
	return id;
}

const QString& NavitMenuItem::getLinkTarget() const {
	return linkTarget;
}

NavitMenuItem& NavitMenuItem::addChild(QDomNode& node, const QString& iconFolder) {
	// the QTreeView takes care of deleting this NavitMenuItem
	NavitMenuItem& item = *new NavitMenuItem(node, iconFolder, *this);
	return item;
}

NavitMenuItem& NavitMenuItem::getChild(int row) {
	return *reinterpret_cast<NavitMenuItem*>(child(row, 0));
}

const NavitMenuItem& NavitMenuItem::getChild(int row) const {
	return *reinterpret_cast<NavitMenuItem*>(child(row, 0));
}
