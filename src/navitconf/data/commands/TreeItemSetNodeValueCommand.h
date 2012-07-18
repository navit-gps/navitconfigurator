/*
 * TreeEditCommand.h
 *
 *  Created on: 13.06.2012
 *      Author: remo
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
