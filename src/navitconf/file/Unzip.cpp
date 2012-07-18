/*
 * Unzip.cpp
 */

#include "Unzip.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <quazip/quazipfile.h>

Unzip::Unzip(const QString& zipFile, const QString& targetDir)
:
		zip(zipFile),
		targetDir(targetDir),
		numberOfFiles(-1),
		nextFilename()
{
	if (!zip.open(QuaZip::mdUnzip)) {
		throw QString("Opening ZIP-file failed: ")+zip.getZipName();
	}
	if (!targetDir.endsWith('/') && !targetDir.endsWith('\\')) {
		this->targetDir.append('/');
	}
	if (!zip.goToFirstFile()) {
		throw QString("Goto first file in archive failed:") + zip.getZipError();
	}
	numberOfFiles = zip.getEntriesCount();
	nextFilename = zip.getCurrentFileName();
}

Unzip::~Unzip() {
}

QString Unzip::extract(const QString& fileSource) {
	if (!zip.setCurrentFile(fileSource)) {
		throw QString("Opening file in ZIP-file failed: ")+zip.getZipName()+"/"+fileSource;
	}
	QuaZipFile quaZipFile(&zip);
	if (!quaZipFile.open(QIODevice::ReadOnly)) {
		throw QString(zip.getZipError());
	}
	QString extractedFile(targetDir+QFileInfo(fileSource).fileName());
	QFile file(extractedFile);
	if (!file.open(QIODevice::WriteOnly)) {
		throw QString("Opening file for writing failed: ")+extractedFile;
	}
	const QByteArray sdata = quaZipFile.readAll();
	file.write(sdata);
	file.close();
	return extractedFile;
}

int Unzip::getNumberOfFiles() const {
	return numberOfFiles;
}

QString Unzip::getNextFilename() const {
	return nextFilename;
}

bool Unzip::hasNext() const {
	return !nextFilename.isEmpty();
}

QString Unzip::extractNext() {
	QuaZipFile quaZipFile(&zip);
	if (!quaZipFile.open(QIODevice::ReadOnly)) {
		throw QString(zip.getZipError());
	}
	QDir dir(targetDir);
	QFile file(dir.filePath(nextFilename)); // append file defined in zip file
	QFileInfo fileInfo(file);
	if (!dir.exists(fileInfo.absolutePath())) { // folder does not
		if (!dir.mkpath(fileInfo.absolutePath())) { // create folder
			throw QString("Creating folder failed: ") + fileInfo.absolutePath();
		}
	}
	if (!file.open(QIODevice::WriteOnly)) {
		throw QString("Opening file for writing failed: ") + file.fileName();
	}
	const QByteArray sdata = quaZipFile.readAll();
	file.write(sdata);
	file.close();
	if (zip.goToNextFile()) {
		nextFilename = zip.getCurrentFileName();
	} else {
		nextFilename.clear();
	}
	return file.fileName();
}
