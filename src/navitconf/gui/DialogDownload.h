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

#ifndef DIALOGDOWLOAD_H
#define DIALOGDOWLOAD_H

#include <QtWidgets/QWidget>

#include "ui_DialogDownload.h"

#include <navitconf/net/Download.h>

/**
 * Dialog to download the last version of Navit for Android devices.
 */
class DialogDownload : public QDialog
{
    Q_OBJECT

public:
    /**
     * Show the download dialog window for downloading the
     * navit.xml file.
     * @param parent The parent window for a modal view.
     * @return A full path to downloaded navit.xml file,
     *         if download has been successfully,
     *         otherweise returns an empty string.
     */
    static QString download(QWidget* parent = 0);
    /**
     * @param The filename of the extracted navit.xml file, or "" if canceled.
     */
    QString getFilename();
    ~DialogDownload();

public slots:
	/** On OK clicked */
	void accept();
	void downloadProgress(float percent);
	void downloadFinished(const QString& filename);
	void downloadError(const QString& msg);

private:
    QString filename;
    Ui::DialogDownloadClass ui;
    Download netDownload;
    DialogDownload(QWidget* parent = 0);
    void extractNavitXml(const QString& zipFile);
};

#endif // DIALOGDOWLOAD_H
