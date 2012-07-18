/*
 * Settings.h
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QByteArray>

/**
 * Utility class for persisting the application settings in the standard QT user area - normally uses
 * <code>~/.config/NavitConfigurator.conf</code>.
 */
class Settings {
public:

	/**  IDs for boolean settings. */
	enum BoolId {
		MAXIMIZED = 100,
		DEVICE_DISPLAY_IS_LANDSCAPE
	};

	/**  IDs for integer settings. */
	enum IntId {
		WINDOW_X = 200,
		WINDOW_Y,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		DEVICE_DISPLAY_SIZE
	};

	/** IDs for QString setting.  */
	enum StringId {
	};

	/** IDs for a QStringList setting. */
	enum StringListId {
		RECENT_FILES = 300
	};

	/** IDs for a QByteArray setting. */
	enum BytesId {
		SAVE_STATE = 400
	};

	/**
	 * Get an instance of the settings class.
	 * @throw QString Creating settings folder in the home directory
	 *                failed (only on first call).
	 */
	static Settings& getInstance();
	/**
	 * @return The QDir for storing all program settings, e.g. the home directory with suffix "/.NavitConfigurator".
	 */
	QDir& getDir();
	/**
	 * @return True, if there is a persistent integer key, otherwise false.
	 */
	bool hasKey(int id);
	/**
	 * Generic method for setting a persistent boolean.
	 */
	void setBool(BoolId id, bool isTrue);
	/**
	 * Generic method for setting a persistent string list.
	 */
	bool getBool(BoolId id, bool defaultValue = false);
	/**
	 * Generic method for setting a persistent boolean.
	 */
	void setInt(IntId id, int value);
	/**
	 * Generic method for setting a persistent string list.
	 */
	int getInt(IntId id, int defaultValue = 0);
	/**
	 * Generic method for setting a persistent boolean.
	 */
	void setString(StringId id, const QString& value);
	/**
	 * Generic method for setting a persistent string list.
	 */
	QString getString(StringId id, const QString& defaultValue = QString());
	/**
	 * Generic method for setting a persistent string list.
	 */
	void setQStringList(StringListId id, const QStringList& list);
	/**
	 * Generic method for getting a persistent string list.
	 */
	QStringList getQStringList(StringListId id);
	/**
	 * Generic method for setting a persistent byte array.
	 */
	void setBytes(BytesId id, const QByteArray& bytes);
	/**
	 * Generic method for getting a persistent string list.
	 */
	QByteArray getBytes(BytesId id);
	/**
	 * @return The absolute path to the file navit.apk, e.g. getDir()+"/navit.apk".
	 */
	QString getNavitInstallationFile();
private:
	QSettings settings;
	QDir dir;
	Settings();
	virtual ~Settings();
};

#endif /* SETTINGS_H_ */
