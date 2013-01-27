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

#include "NavitConfigurator.h"

#include <QtCore/QStringList>
#include <QtCore/QUrl>

#include <QtGui/QUndoStack>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QUndoView>
#include <QtGui/QIcon>
#include <QtGui/QDesktopServices>

#include "DialogDownload.h"
#include "DialogStartingPosition.h"
#include "DialogManageNavitMenu.h"
#include "DialogGeneralSettings.h"
#include "mapview/DialogMapView.h"

#include <navitconf/file/Settings.h>

NavitConfigurator::NavitConfigurator(QWidget* parent)
:
	QMainWindow(parent),
	ui(),
	hasFileOpened(false),
	defaultWindowTitle(),
	filename(),
	recentFileMenuActions(),
	tabNode(),
	treeView(),
	document(tabNode),
	navitRunner(document)
{
	// UI
	ui.setupUi(this);
	defaultWindowTitle = windowTitle();
	recentFileMenuActions.append(ui.actionRecent1);
	recentFileMenuActions.append(ui.actionRecent2);
	recentFileMenuActions.append(ui.actionRecent3);
	tabNode.setupUi(ui.tabWidgetMain, ui.tabWidgetNode, ui.tbxNodePath, ui.tableView, ui.tbxNodeComment, ui.tbxSource);
	treeView.setupUi(*ui.widgetLeft, *ui.widgetLeftGridLayout);
	document.setupUi(treeView, *ui.tbxSource);
	refreshRecentFiles();
	canUndoChanged(false);
	canRedoChanged(false);
	refreshAll();
	// connect slots for undo state changes
	QUndoStack* undoStack = &document.getUndoStack();
	connect(undoStack, SIGNAL(canUndoChanged(bool)),
			this,      SLOT(  canUndoChanged(bool)));
	connect(undoStack, SIGNAL(canRedoChanged(bool)),
			this,      SLOT(  canRedoChanged(bool)));
	connect(undoStack, SIGNAL(cleanChanged(bool)),
			this,      SLOT(  cleanChanged(bool)));
	// restore position and size
	Settings& p = Settings::getInstance();
	if (p.hasKey(Settings::WINDOW_X)) { // position and size has been stored previously
		move(
				p.getInt(Settings::WINDOW_X),
				p.getInt(Settings::WINDOW_Y));
		resize(
				p.getInt(Settings::WINDOW_WIDTH),
				p.getInt(Settings::WINDOW_HEIGHT));
		restoreState(p.getBytes(Settings::SAVE_STATE)); // restore tool bar states (not necessary now)
	}
    setWindowIcon(QIcon(":/NavitConfigurator.ico"));
    //setWindowIcon(QIcon("/media/Dat/MyFiles/InformatikMaster/70Module/PAdI/NavitConfigurator/NavitConfigurator.png"));
	// show window
	if (p.getBool(Settings::MAXIMIZED)) {
		showMaximized();
	} else {
		show();
	}
}

NavitConfigurator::~NavitConfigurator() {
	Settings& p = Settings::getInstance();
	p.setBool(Settings::MAXIMIZED, isMaximized());
	if (!isMaximized()) { // the position and size is only valid if not maximized
		p.setInt(Settings::WINDOW_X, x());
		p.setInt(Settings::WINDOW_Y, y());
		p.setInt(Settings::WINDOW_WIDTH, width());
		p.setInt(Settings::WINDOW_HEIGHT, height());
	}
	p.setBytes(Settings::SAVE_STATE, saveState());
}

void NavitConfigurator::refreshAll() {
	ui.actionSave->setEnabled(!document.getUndoStack().isClean() && !filename.isEmpty());
	ui.actionSaveAs->setEnabled(hasFileOpened);
	ui.actionRun->setEnabled(hasFileOpened);
	ui.actionSelectStartingPosition->setEnabled(hasFileOpened);
	ui.actionManageNavitMenu->setEnabled(hasFileOpened);
	ui.actionNavitViewEditor->setEnabled(hasFileOpened);
	ui.actionGeneralSettings->setEnabled(hasFileOpened);
	refreshWindowTitle();
}

void NavitConfigurator::refreshWindowTitle() {
	QString title(defaultWindowTitle);
	if (filename.length() > 0) {
		title.append(" - ").append(filename);
	}
	if (!document.getUndoStack().isClean()) {
		title.append("*");
	}
	setWindowTitle(title);
}

void NavitConfigurator::refreshRecentFiles() {
	QStringList list = Settings::getInstance().getQStringList(Settings::RECENT_FILES);
	for (int i = 0; i < recentFileMenuActions.size(); i++) {
		bool visible = i < list.size();
		recentFileMenuActions[i]->setVisible(visible);
		if (visible) {
			recentFileMenuActions[i]->setText(QString("&%1 %2").arg(i+1).arg(list[i]));
		}
	}
}

void NavitConfigurator::addRecentFile(const QString& filename) {
	QStringList list = Settings::getInstance().getQStringList(Settings::RECENT_FILES);
	list.removeAll(filename);
	list.push_front(filename);
	while (list.size() > 3) {
		list.removeLast();
	}
	Settings::getInstance().setQStringList(Settings::RECENT_FILES, list);
	refreshRecentFiles();
}

void NavitConfigurator::setStatusText(const QString& text) {
	statusBar()->showMessage(text);
}

void NavitConfigurator::openFile(const QString& filename, bool isAppendingToRecentFiles) {
	try {
		openFileOnly(filename);
		if (isAppendingToRecentFiles) {
			addRecentFile(filename);
		}
	} catch(QString& e) {
		QMessageBox::critical(this, tr("Error"), e);
	}
}

void NavitConfigurator::openRecentFile(int i) {
	if (closeFile()) {
		openFile(Settings::getInstance().getQStringList(Settings::RECENT_FILES)[i]);
	}
}

void NavitConfigurator::openFileOnly(const QString filename) {
	document.open(filename);
	hasFileOpened = true;
	this->filename.clear();
	this->filename.append(filename);
	ui.widgetLeft->setEnabled(true);
	ui.widgetCenter->setEnabled(true);
	refreshAll();
}

void NavitConfigurator::saveFile(const QString filename) {
    if (!filename.isEmpty()) {
    	try {
    		document.save(filename);
			this->filename.clear();
			this->filename.append(filename);
			addRecentFile(filename);
			refreshAll();
			setStatusText(tr("File saved"));
    	} catch(QString& e) {
			QMessageBox::critical(this, tr("Error"), e);
		}
    }
}

bool NavitConfigurator::closeFile() {
	if (!document.getUndoStack().isClean()) {
		int result = QMessageBox::question(this,
				tr("Question"),
				tr("The current Navit configuration file has been modified. Save changes?"),
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
				QMessageBox::Save);
		if (result == QMessageBox::Cancel) {
			return false;
		} else if (result == QMessageBox::Save) {
			onMenuSave();
			if (!document.getUndoStack().isClean()) {
				return false;
			}
		}
	}
	return true;
}

void NavitConfigurator::closeEvent(QCloseEvent* event) {
	if (closeFile()) {
		event->accept();
	} else {
		event->ignore();
	}
}

void NavitConfigurator::onMenuNew() {
	if (closeFile()) {
		QString filename(DialogDownload::download(this));
		if (!filename.isEmpty()) {
			openFile(filename, false);
			this->filename.clear();
			refreshAll();
		}
	}
}

void NavitConfigurator::onMenuOpen() {
	QString filename(QFileDialog::getOpenFileName(this, tr("Choose a navit.xml file")));
	if (!filename.isEmpty()) {
		openFile(filename);
	}
}

void NavitConfigurator::onMenuSave() {
	if (filename.isEmpty()) {
		onMenuSaveAs();
	} else {
		saveFile(filename);
	}
}

void NavitConfigurator::onMenuSaveAs() {
	QString filename(QFileDialog::getSaveFileName(this, tr("Choose a navit.xml file")));
	saveFile(filename);
}

void NavitConfigurator::onMenuRecent1() {
	openRecentFile(0);
}
void NavitConfigurator::onMenuRecent2() {
	openRecentFile(1);
}
void NavitConfigurator::onMenuRecent3() {
	openRecentFile(2);
}

void NavitConfigurator::onMenuUndo() {
	document.getUndoStack().undo();
}

void NavitConfigurator::onMenuRedo() {
	document.getUndoStack().redo();
}

void NavitConfigurator::onMenuSelectStartingPosition() {
	DialogStartingPosition(*this, document.getTreeModel());
}

void NavitConfigurator::onMenuManageNavitMenu() {
	DialogManageNavitMenu(*this, document.getTreeModel());
}

void NavitConfigurator::onMenuNavitViewEditor() {
	DialogMapView(*this, document.getTreeModel());
}

void NavitConfigurator::onMenuGeneralSettings() {
	DialogGeneralSettings(*this, document.getTreeModel());
}

void NavitConfigurator::onMenuRun() {
	if (!document.getUndoStack().isClean()) {
		if (ui.actionAutoSave->isChecked()) {
			onMenuSave();
		}
	}
	if (document.getFilename().isEmpty()) {
		return;
	}
	navitRunner.uploadDocument();
	QString result = navitRunner.run();
	appendToConsole(result);
	if (result.contains("error:")) {
		QMessageBox::critical(this, tr("Error"), getHint(tr("Running Navit on device failed"), result));
	}
}

void NavitConfigurator::onMenuInstallNavit() {
	QString apkFilename = Settings::getInstance().getDir().absolutePath() + "/navit.apk";
	if (QFileInfo(apkFilename).exists()) {
		QString result = navitRunner.install(apkFilename);
		appendToConsole(result);
		if (result.contains("error:")) {
			QMessageBox::critical(this, tr("Error"), getHint(tr("Installation failed"), result));
		}
	} else {
		QMessageBox::critical(this, tr("Error"), tr("Please download a new version of Navit first with File->New."));
	}
}

QString NavitConfigurator::getHint(const QString& description, const QString& result) {
	QString hint;
	if (result.contains("insufficient permissions for device")) {
		hint.append(tr("Please restart the adb service on your computer with root previledges:\nadb kill-server\nsudo adb root"));
	} else if (result.contains("device not found")) {
		hint.append(tr("Please check USB/Bluetooth/WLAN connection between PC and device."));
	}
	return QString(description).append(":\n").append(result).append("\n\n").append(hint);
}

void NavitConfigurator::onMenuDownloadMap() {
	if (!QDesktopServices::openUrl(QUrl(tr("http://jff-webhosting.net/osm/navit/world/")))) {
		QMessageBox::critical(this, tr("Error"), tr("The URL could not be opened in a browser."));
	}
}

void NavitConfigurator::onMenuAbout() {
	QMessageBox::about(this, tr("About NavitConfigurator"),
			tr("<p><b>NavitConfigurator</b> makes it easy to edit your navit.xml file.</p>"
					"<p>Credits go to the developer of Navit, Qt, QuaZip and zlib. The icons are from <a href='http://openiconlibrary.sourceforge.net/'>openiconlibrary.sourceforge.net</a>.</p>"
					"<p>This program is developed for PAdI (Praktische Aspekte der Informatik) at the technical university in Brunswick.</p>"
					"<p>For updates see <a href='http://sourceforge.net/p/navitconfigurat'>sourceforge.net/p/navitconfigurat</a>.</p>"
					"<p>Copyright 2012 by Raimar B&uuml;hmann</p>"
			)
	);
}

void NavitConfigurator::onTabSourceTextChanged() {
//	onFileChanged();
}

/**
 * QUndoStack events
 */
void NavitConfigurator::canUndoChanged(bool canUndo) {
	ui.actionUndo->setEnabled(canUndo);
}

void NavitConfigurator::canRedoChanged(bool canRedo) {
	ui.actionRedo->setEnabled(canRedo);
}

void NavitConfigurator::cleanChanged(bool isClean) {
	ui.actionSave->setEnabled(!isClean);
	refreshWindowTitle();
}

void NavitConfigurator::appendToConsole(const QString& output) {
	QString html(output);
	html.replace("error:", "<b style='background-color:#FF8080'>Error:</b>");
	html.replace("Warning:", "<b style='background-color:yellow'>Warning:</b>");
	html.replace("\n", "<br />");
	ui.tbxConsole->appendHtml(html);
	bool showConsole = true;
	if (ui.comboBoxShowConsole->currentIndex() > 0) {
		int index = ui.comboBoxShowConsole->currentIndex();
		if (index == 3) {
			showConsole = false;
		} else { // index = 1 or 2
			if (!output.contains("error:")) {
				if (output.contains("Warning:")) {
					showConsole = index == 1;
				} else {
					showConsole = false;
				}
			} // else errors exist, so show
		}
	}
	if (showConsole) {
		ui.tabWidgetMain->setCurrentWidget(ui.tabConsole);
	}
}
