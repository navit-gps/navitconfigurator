#ifndef NAVITCONFIGURATOR_H
#define NAVITCONFIGURATOR_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QModelIndex>

#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QCloseEvent>
#include <QtGui/QAction>

#include <ui_NavitConfigurator.h>

#include <navitconf/file/Document.h>
#include <navitconf/gui/TreeView.h>
#include <navitconf/gui/TabNode.h>
#include <navitconf/android/NavitRunner.h>

/**
 * Main window handling the menu bar actions and the XML tree view.
 */
class NavitConfigurator : public QMainWindow {
	Q_OBJECT
public:
	explicit NavitConfigurator(QWidget *parent = 0);
	~NavitConfigurator();

public slots:
	/**
	 * Menu items clicked.
	 */
	void onMenuNew();
	void onMenuOpen();
	void onMenuSave();
	void onMenuSaveAs();
	void onMenuRecent1();
	void onMenuRecent2();
	void onMenuRecent3();
	void onMenuUndo();
	void onMenuRedo();
	void onMenuSelectStartingPosition();
	void onMenuManageNavitMenu();
	void onMenuNavitViewEditor();
	void onMenuGeneralSettings();
	void onMenuRun();
	void onMenuInstallNavit();
	void onMenuAbout();

	void onTabSourceTextChanged();

	/**
	 * QUndoStack events
	 */
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);
    void cleanChanged(bool isClean);

private:
	Ui::NavitConfiguratorClass ui;
	bool hasFileOpened;
	QString defaultWindowTitle;
	QString filename;
	QList<QAction*> recentFileMenuActions;
	TabNode tabNode;
	TreeView treeView;
	Document document;
	NavitRunner navitRunner;
    void refreshWindowTitle();
	void refreshAll();
	void refreshRecentFiles();
	void addRecentFile(const QString& filename);
    void setStatusText(const QString& text);
    /**
     * @param filename The filename.
     * @param isAppendingToRecentFiles True, if the file has to add to the recent list of files, otherwise false.
     */
    void openFile(const QString& filename, bool isAppendingToRecentFiles = true);
    void openRecentFile(int i);
	void openFileOnly(const QString filename);
	void saveFile(const QString filename);
    /**
     * @return True, if file has been closed successfully, otherwise false.
     */
    bool closeFile();
	void closeEvent(QCloseEvent* event);
	void appendToConsole(const QString& output);

	QString getHint(const QString& description, const QString& result);

};

#endif // NAVITCONFIGURATOR_H
