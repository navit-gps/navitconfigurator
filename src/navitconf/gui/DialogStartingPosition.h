#ifndef DIALOGSTARTINGPOSITION_H
#define DIALOGSTARTINGPOSITION_H

#include <QtCore/QString>

#include <QtGui/QWidget>
#include <QtGui/QDialog>

#include <QtWebKit/QWebElement>

#include "ui_DialogStartingPosition.h"

#include <navitconf/data/TreeModel.h>

/**
 * Dialog for setting the GPS position shown after Navit has started.
 */
class DialogStartingPosition : public QDialog
{
    Q_OBJECT

public:
    /**
     * Show the dialog to select the starting position and set the new location
     * if user pressed OK. The current starting position is shown.
     * @param parent The parent window for a modal view.
     * @param treeModel The tree model for storing the changed starting position.
     */
    explicit DialogStartingPosition(QWidget& parent, TreeModel& treeModel);
    virtual ~DialogStartingPosition();

public slots:
	/** On page loading finished fill in the starting position into the form for later navigation. */
	void loadFinished(bool isOK);
	/**
	 * On OK clicked check if starting position has been selected.
	 * If there is a starting position, set it into tree model.
	 * Close the dialog if the starting position has been set.
	 */
	void accept();

private:
	static const char* const CONFIG_NAVIT;
	static const char* const CENTER_ATTRIBUTE;
	TreeModel& treeModel;
    Ui::DialogStartingPosition ui;
    /**
     * @param The id name of the input tag.
     * @return The value of the input tag with the given id name.
     */
    QString getInputValue(const QString& idName);
    /**
     * @param idName The id of the input tag.
     * @param idValue The value to set into the input tag.
     */
    void setInputValue(const QString& idName, const QString& idValue);
};

#endif // DIALOGSTARTINGPOSITION_H
