#ifndef DIALOGDOWLOAD_H
#define DIALOGDOWLOAD_H

#include <QtGui/QWidget>

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
