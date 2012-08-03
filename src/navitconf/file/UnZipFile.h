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

#ifndef UNZIPFILE_H_
#define UNZIPFILE_H_

#include <QtCore/QString>
#include <quazip/quazip.h>

/**
 * Utility class for decompressing all files in a zip-file.
 * @see DialogDownload
 */
class UnZipFile {
public:
	/**
	 * Opens a ZIP-file for decompression.
	 * @param zipFile   The file name of the ZIP-file.
	 * @param targetDir The target folder.
	 * @throw QString If something goes wrong.
	 */
	UnZipFile(const QString& zipFile, const QString& targetDir);
	virtual ~UnZipFile();

	/**
	 * Uncompress fileSource from the given ZIP-file in the constructor and
	 * the given output directory with the same file name.
	 * @param fileSource The file path relative to the root in the ZIP-file.
	 * @return The absolute filename of the extracted file.
	 * @throw QString If something goes wrong.
	 */
	QString extract(const QString& fileSource);

	/**
	 * @return The number of files the zip file contains.
	 */
	int getNumberOfFiles() const;

	/**
	 * @return The next file name in the zip file to extract with {@link #extractNext()}.
	 */
	QString getNextFilename() const;

	/**
	 * @return True if there are one or more files left in the zip file to extract.
	 */
	bool hasNext() const;

	/**
	 * Extract the next file from the zip file into the targetDir given in the constructor.
	 * Folder names contained in the zip file are created automatically,
	 * @return The absolute file name of the extracted file.
	 * @throw QString If something goes wrong.
	 */
	QString extractNext();

private:
	QuaZip zip;
	QString targetDir;
	int numberOfFiles;
	QString nextFilename;
};

#endif /* UNZIPFILE_H_ */
