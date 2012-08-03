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

#include "UnZipFile.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <quazip/quazipfile.h>

UnZipFile::UnZipFile(const QString& zipFile, const QString& targetDir)
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

UnZipFile::~UnZipFile() {
}

QString UnZipFile::extract(const QString& fileSource) {
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

int UnZipFile::getNumberOfFiles() const {
	return numberOfFiles;
}

QString UnZipFile::getNextFilename() const {
	return nextFilename;
}

bool UnZipFile::hasNext() const {
	return !nextFilename.isEmpty();
}

QString UnZipFile::extractNext() {
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
