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

#ifndef DIALOGVIEWEDITOR_H
#define DIALOGVIEWEDITOR_H

#include <QtCore/QModelIndex>

#include <QtGui/QWidget>
#include <QtGui/QDialog>
#include <QtGui/QStandardItemModel>

#include "ui_DialogMapView.h"

#include <navitconf/data/TreeModel.h>
#include "MapViewScene.h"
#include "BoxGraphicsItem.h"
#include "BoxGraphicsItemListener.h"

/**
 * Dialog for editing the Navit view.
 */
class DialogMapView
: public QDialog,
  public BoxGraphicsItemListener
{
    Q_OBJECT

public:
	/**
	 * Show the dialog to manage the menu items. Changes are added to the undo stack.
	 * @param parent The parent window for a modal view.
	 * @param treeModel The tree model for retrieving and storing the changed menu items.
	 */
	explicit DialogMapView(QWidget& parent, TreeModel& treeModel);
	virtual ~DialogMapView();
	/**
	 * Implementation of BoxGraphicsItemListener fired by the box items, which are added to the scene.
	 * Refresh the table view with the new box item properties.
	 * @param The selected box item.
	 */
	virtual void boxItemSelected(const BoxGraphicsItem& boxItem);
	virtual void boxItemMoving(const BoxGraphicsItem& boxItem);
	virtual void boxItemResizing(const BoxGraphicsItem& boxItem);
	virtual void boxItemMoveFinished(BoxGraphicsItem& boxItem);
	virtual void boxItemResizeFinished(BoxGraphicsItem& boxItem);

public slots:
	/**
	 * Called by ui.comboBoxDisplaySize and ui.checkBoxLandscape, when display size or landscape property changed,
	 * and resize the ui.graphicsView automatically.
	 */
	void comboBoxDisplaySizeIndexChanged();
	/**
	 * Called when on of the table view box item property has changed.
	 */
	void tableViewBoxPropertiesDataChanged(const QModelIndex& modelIndex);
	/**
	 * Button events for adding / deleting of items and undo / redo the changes.
	 */
	void buttonAddClicked();
	void buttonDeleteClicked();
	void buttonUndoClicked();
	void buttonRedoClicked();
	/** Listener for QUndoStack events. */
	void undoStackStateChanged();
	/**
	 * Called by pressing the OK button. Store the current display size and landscape mode and close the dialog.
	 */
	void accept();

private:
	/** The UI for this dialog. */
	Ui::DialogMapView ui;
	/** The tree model storing the XML file. */
	TreeModel& treeModel;
	/** The graphics sceen containing all rectangles. */
	MapViewScene scene;
	/** The table view data model. */
	QStandardItemModel tableModel;
	/** The enum IDs of the check boxes. */
	QList<int> checkBoxes;
	/** True, if the table view model changes data, otherwise false. */
	bool isTableViewBoxPropertiesRefreshing;
	/**
	 * Fill the graphics view with rectangle boxes defined in the Navit view.
	 */
	void fillGraphicsView();
	/**
	 * Fill the rows of the table for box properties to show x, y, width and height of the selected box item.
	 */
	void initTableViewBoxProperties();
	void refreshHorizontalAlign(const BoxGraphicsItem& item);
	void refreshVerticalAlign(const BoxGraphicsItem& item);
	void initViewsAfterUndoRedo();

};

#endif // DIALOGVIEWEDITOR_H
