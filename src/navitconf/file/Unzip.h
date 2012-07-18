/*
 * Unzip.h
 */

#ifndef UNZIP_H_
#define UNZIP_H_

#include <QString>
#include <quazip/quazip.h>

/**
 * Utility class for decompressing all files in a zip-file.
 * @see DialogDownload
 */
class Unzip {
public:
	/**
	 * Opens a ZIP-file for decompression.
	 * @param zipFile   The file name of the ZIP-file.
	 * @param targetDir The target folder.
	 * @throw QString If something goes wrong.
	 */
	Unzip(const QString& zipFile, const QString& targetDir);
	virtual ~Unzip();

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

#endif /* UNZIP_H_ */
