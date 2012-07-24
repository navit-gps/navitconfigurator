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

#include "Document.h"

#include <cstddef>

#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <QtGui/QMessageBox>
#include <QtGui/QItemSelectionModel>

Document::Document(TabNode& tabNode)
:
	QObject(),
	tabNode(tabNode),
	filename(),
	undoStack(),
	domDocument(new QDomDocument()),
	treeModel(new TreeModel(*domDocument, undoStack)),
	treeView(NULL),
	plainTextEdit(NULL)
{
//	connectTreeModel();
}

Document::~Document() {
	// disconnectTreeModel(); // treeView does not exist anymore at this time
	delete treeModel;
	delete domDocument;
}

void Document::setupUi(QTreeView& treeView, QPlainTextEdit& plainTextEdit) {
	this->treeView = &treeView;
	this->plainTextEdit = &plainTextEdit;
}

const QString& Document::getFilename() const {
	return filename;
}

QUndoStack& Document::getUndoStack() {
	return treeModel->getUndoStack();
}

void Document::open(const QString& filename) {
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		throw QString(tr("Opening file failed: "))+filename+": "+file.errorString();
	}
	QDomDocument* newXml = new QDomDocument();
	QString errMsg;
	int errorLine;
	int errorColumn;
	if (newXml->setContent(&file, &errMsg, &errorLine, &errorColumn)) {
		file.close();
		disconnectTreeModel();
		delete treeModel;
		delete domDocument;
		domDocument = newXml;
		this->filename = filename;
		treeModel = new TreeModel(*domDocument, undoStack);
		treeView->setModel(treeModel);
		tabNode.setTreeModel(*treeModel);
		connectTreeModel();
	} else {
		file.close();
		delete newXml;
		throw tr("Error in file '%1' line %2 column %3: %4")
				.arg(filename).arg(errorLine).arg(errorColumn).arg(errMsg);
	}
}

void Document::save(const QString& filename) {
	QString currentFilename;
	if (filename.isEmpty()) {
		currentFilename.append(this->filename);
	} else {
		currentFilename.append(filename);
	}
	QFile file(currentFilename);
	if (!file.open(QIODevice::WriteOnly)) {
		throw QString(tr("Opening file for writing failed: "))+file.fileName()+": "+file.errorString();
	}
//	QTextStream textStream(&file);
//	domDocument->save(textStream, 3);
	if (!file.write(domDocument->toString().toAscii())) {
		file.close();
		throw QString(tr("Writing file failed: "))+file.fileName()+": "+file.errorString();
	}
	file.close();
	this->filename = filename;
	// mark the current saved position in undo stack as a clean state (no *)
	treeModel->getUndoStack().setClean();
}

//QDomNodeList Document::getElementsByTagName(const QString& tagName) {
//	return domDocument->documentElement().elementsByTagName(tagName);
//}

/**
 * public slots
 */
void Document::onSelectionChanged(const QItemSelection& selected) {
	if (!selected.isEmpty()) {
		tabNode.setSelectedItem(selected.first().topLeft());
	}
}

//void Document::onDataChanged(const QModelIndex& indexTopLeft, const QModelIndex& /* indexRightBottom */) {
//	// deselect row(s)
//	QModelIndexList list = treeView->selectionModel()->selectedIndexes();
//	for (int i = 0; i < list.size(); i++) {
//		treeView->selectionModel()->select(list[i], QItemSelectionModel::Deselect | QItemSelectionModel::Clear);
//	}
//	// select changed row
//	treeView->selectionModel()->select(indexTopLeft, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
//	// ensure selected row is visible
//	treeView->scrollTo(indexTopLeft);
//}

//void Document::onFileChanged(const QString& filename) {
//	if (QMessageBox::question(
//			treeView,
//			tr("Question"),
//			tr("The file \"%1\" has been changed by an other program. Do you want to reload the file?").arg(filename),
//			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
//		open(this->filename);
//	}
//}

/**
 * private methods
 */
void Document::connectTreeModel() {
	// register select events on tree view
	connect(treeView->selectionModel(), SIGNAL(  selectionChanged(const QItemSelection&, const QItemSelection&)),
			this,                       SLOT(  onSelectionChanged(const QItemSelection&)));
}

void Document::disconnectTreeModel() {
	if (treeView != NULL) {
		if (treeView->selectionModel() != NULL) {
			disconnect(treeView->selectionModel(), SIGNAL(  selectionChanged(const QItemSelection&, const QItemSelection&)),
					   this,                       SLOT(  onSelectionChanged(const QItemSelection&)));
		}
	}
}
