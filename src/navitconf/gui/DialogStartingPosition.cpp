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

#include "DialogStartingPosition.h"

#include <QtCore/QUrl>
#include <QtCore/QStringList>
#include <QtGui/QMessageBox>
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>

#include <navitconf/data/TreeItem.h>

const char* const DialogStartingPosition::CONFIG_NAVIT = "config/navit";
const char* const DialogStartingPosition::CENTER_ATTRIBUTE = "center";

DialogStartingPosition::DialogStartingPosition(QWidget& parent, TreeModel& treeModel)
:
		QDialog(&parent),
		treeModel(treeModel)
{
	ui.setupUi(this);
	// only signal linkClicked() is emitted (only program can change URL)
	ui.webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	ui.webView->setUrl(QUrl("http://itouchmap.com/latlong.html"));
	// connect listener for loaded page
	connect(ui.webView, SIGNAL(loadFinished(bool)),
	        this,       SLOT  (loadFinished(bool)));
	exec(); // show dialog
}

DialogStartingPosition::~DialogStartingPosition() {
}

void DialogStartingPosition::loadFinished(bool isOK) {
	if (isOK) { // page has successfully loaded
		QModelIndex modelIndex = treeModel.getModelIndexByDomPath(CONFIG_NAVIT);
		const TreeItem& item = treeModel.getTreeItem(modelIndex);
		QStringList tokens = item.getAttribute(CENTER_ATTRIBUTE).split(' ');
		if (tokens.size() == 4) {
			double lat = tokens[0].toDouble() / 100;
			double lon = tokens[2].toDouble() / 100;
			if (tokens[1] == "S") {
				lat *= -1;
			}
			if (tokens[3] == "W") {
				lon *= -1;
			}
			QString latString = QString::number(lat, 'f', 6);
			QString lonString = QString::number(lon, 'f', 6);
			setInputValue("latbox1", latString);
			setInputValue("lonbox1", lonString);
		}
	}
	// FIXME: do not allow back with Back-Space key nor context menu Back (does not work!!!)
	ui.webView->pageAction(QWebPage::Back)->setEnabled(false);
	ui.webView->pageAction(QWebPage::Forward)->setEnabled(false);
}

void DialogStartingPosition::accept() {
	QModelIndex modelIndex = treeModel.getModelIndexByDomPath(CONFIG_NAVIT);
	QString latitude = getInputValue("latbox");
	QString longitude = getInputValue("lonbox");
	if (!modelIndex.isValid()) {
		QMessageBox::critical(this, tr("Error"),
				tr("The path %1 has not been found in the navit configuration file.").arg(CONFIG_NAVIT));
	} else if (latitude.isEmpty() || longitude.isEmpty()) {
		QMessageBox::critical(this, tr("Error"),
				tr("Please choose a starting position first.").arg(latitude).arg(longitude));
	} else {
		// latitude and longitude are multiplied by 100
		double lat = latitude.toDouble() * 100;
		double lon = longitude.toDouble() * 100;
		// N/S and W/E chars
		char latChar = 'N';
		char lonChar = 'E';
		if (lat < 0) {
			latChar = 'S';
			lat *= -1; // absolute value
		}
		if (lon < 0) {
			lonChar = 'W';
			lon *= -1; // absolute value
		}
		QString lat100times = QString::number(lat, 'f', 4); // round number (4 digits after point)
		QString lon100times = QString::number(lon, 'f', 4); // round number (4 digits after point)
		treeModel.setAttribute(modelIndex, CENTER_ATTRIBUTE,
				QString("%1 %2 %3 %4").arg(lat100times).arg(latChar).arg(lon100times).arg(lonChar));
		setVisible(false); // close dialog
	}
}

QString DialogStartingPosition::getInputValue(const QString& idName) {
	QString javascriptCommand = QString("document.getElementById('%1').value").arg(idName);
	return ui.webView->page()->mainFrame()->
			evaluateJavaScript(javascriptCommand).toString();
}

void DialogStartingPosition::setInputValue(const QString& idName, const QString& idValue) {
	QString javascriptCommand = QString("document.getElementById('%1').value='%2'").arg(idName).arg(idValue);
	ui.webView->page()->mainFrame()->evaluateJavaScript(javascriptCommand);
}
