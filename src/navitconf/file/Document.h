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

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <string>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QModelIndex>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>

#include <QtGui/QStandardItemModel>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QUndoStack>

#include <navitconf/data/TreeModel.h>
#include <navitconf/gui/TabNode.h>

/**
 * Represents the navit.xml file.
 */
class Document : public QObject {
	Q_OBJECT
public:
	explicit Document(TabNode& tabNode);
	virtual ~Document();
	void setupUi(QTreeView& treeView, QPlainTextEdit& plainTextEdit);
	/**
	 * @return The filename of the current opened file, or an empty string, if no file has been opened.
	 */
	const QString& getFilename() const;
	QUndoStack& getUndoStack();
	/**
	 * Open the given xml file.
	 * @param filename The filename of the xml file to open.
	 * @throw QString Something goes wrong on opening the file.
	 */
	void open(const QString& filename);
	/**
	 * Save the navit xml file with the given name. If the name is not given and
	 * the same file is saved with the file name used in the open method.
	 * @param filename The file name for saving.
	 */
	void save(const QString& filename = QString());

	TreeModel& getTreeModel() {
		return *treeModel;
	}

public slots:
	void onSelectionChanged(const QItemSelection& selected);
//	void onDataChanged(const QModelIndex& indexTopLeft, const QModelIndex& indexRightBottom);

signals:
	void onDocumentChanged(const QModelIndex& index);

private:
	TabNode& tabNode;
	QString filename;
	QUndoStack undoStack;
	QDomDocument* domDocument;
	TreeModel* treeModel;
	QTreeView* treeView;
	QPlainTextEdit* plainTextEdit;
	void addChild(QDomNode& xmlNode, QStandardItem* node);
	void connectTreeModel();
	void disconnectTreeModel();

};

#endif /* DOCUMENT_H_ */
