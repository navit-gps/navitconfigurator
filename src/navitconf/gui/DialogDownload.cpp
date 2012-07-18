#include "DialogDownload.h"

#include <QtCore/QDebug>

#include <QtGui/QMessageBox>

#include <navitconf/file/Settings.h>
#include <navitconf/file/Unzip.h>

QString DialogDownload::download(QWidget* parent) {
	DialogDownload dialog(parent);
	dialog.exec();
	return dialog.getFilename();
}

DialogDownload::DialogDownload(QWidget* parent)
:
		QDialog(parent),
		netDownload(this)
{
	ui.setupUi(this);
	bool hasLastDownload = false;
	try {
		hasLastDownload = QFile::exists(Settings::getInstance().getNavitInstallationFile());
	} catch(QString& e) {
		QMessageBox::critical(parent, tr("Error"), e);
	}
	ui.rbUseLastDownload->setEnabled(hasLastDownload);
	ui.rbUseLastDownload->setChecked(hasLastDownload);
}

DialogDownload::~DialogDownload() {
}

void DialogDownload::accept() {
	ui.buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
	QString zipFile;
	try {
		zipFile = Settings::getInstance().getNavitInstallationFile();
	} catch(QString& e) {
		QMessageBox::critical(this, tr("Error"), e);
	}
	if (!zipFile.isEmpty()) {
		if (ui.rbDownloadCurrentVersion->isChecked()) {
			QString url(ui.tbxDownloadURL->text());
			ui.progressBar->setMaximum(ui.progressBar->width());
			netDownload.download(ui.tbxDownloadURL->text(), zipFile); // asyn download
			filename.clear();
		} else {
			extractNavitXml(zipFile);
		}
	}
}

void DialogDownload::downloadProgress(float percent) {
	ui.progressBar->setValue(ui.progressBar->maximum() * percent);
}

void DialogDownload::downloadFinished(const QString& filename) {
	extractNavitXml(filename);
}

void DialogDownload::downloadError(const QString& msg) {
	QMessageBox::critical(this, tr("Error"), msg);
	ui.buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
}

QString DialogDownload::getFilename() {
	return filename;
}

void DialogDownload::extractNavitXml(const QString& zipFile) {
	char resolution = 'm';
	if (ui.rbHighResolution->isChecked()) {
		resolution = 'h';
	} else if (ui.rbLowResolution->isChecked()) {
		resolution = 'l';
	}
	try {
		QString dirSettings(Settings::getInstance().getDir().absolutePath());
		// extract all files from archive
		Unzip unzip(zipFile, dirSettings);
		for (int i = 0; i < unzip.getNumberOfFiles(); i++) {
			if (!unzip.getNextFilename().startsWith("lib/")) { // ignore files in the lib folder
				unzip.extractNext();
			}
		}
		filename = dirSettings + "/res/raw/navit" + resolution + "dpi.xml";
		setVisible(false);
	} catch(QString& e) {
		QMessageBox::critical(this, tr("Error"), e);
	}
}
